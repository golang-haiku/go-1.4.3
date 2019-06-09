// Copyright 2010 The Go Authors.  All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "runtime.h"
#include "arch_GOARCH.h"
#include "defs_GOOS_GOARCH.h"
#include "os_GOOS.h"
#include "malloc.h"
#include "textflag.h"

uintptr runtime·area_for(void* addr);

static int32
addrspace_free(void *v, uintptr n)
{
	uintptr errval;
	uintptr chunk;
	uintptr off;

	for(off = 0; off < n; off += chunk) {
		chunk = B_PAGE_SIZE;
		if(chunk > (n - off))
			chunk = n - off;
		errval = runtime·area_for((void*)((uintptr) v + off));
		// B_ERROR means unmapped, which is what we want.
		// Anything else we assume means the pages are mapped.
		if (errval != B_ERROR)
			return 0;
	}
	return 1;
}

static void *
mmap_fixed(byte *v, uintptr n, int32 prot, int32 flags, int32 fd, uint32 offset)
{
	void *p;

	p = runtime·mmap(v, n, prot, flags, fd, offset);
	if(p != v && addrspace_free(v, n)) {
		// On some systems, mmap ignores v without
		// MAP_FIXED, so retry if the address space is free.
		if(p > (void*)4096)
			runtime·munmap(p, n);
		p = runtime·mmap(v, n, prot, flags|MAP_FIXED, fd, offset);
	}
	return p;
}

#pragma textflag NOSPLIT
void*
runtime·sysAlloc(uintptr n, uint64 *stat)
{
	void *p;

	p = runtime·mmap(nil, n, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
	if(p < (void*)4096) {
		if(p == (void*)EACCES) {
			runtime·printf("runtime: mmap: access denied\n");
			runtime·printf("if you're running SELinux, enable execmem for this process.\n");
			runtime·exit(2);
		}
		if(p == (void*)EAGAIN) {
			runtime·printf("runtime: mmap: too much locked memory (check 'ulimit -l').\n");
			runtime·exit(2);
		}
		return nil;
	}
	runtime·xadd64(stat, n);
	return p;
}

void
runtime·SysUnused(void *v, uintptr n)
{
	USED(v);
	USED(n);

	/*runtime·madvise(v, n, MADV_DONTNEED);*/
}

void
runtime·SysUsed(void *v, uintptr n)
{
	USED(v);
	USED(n);
}

void
runtime·SysFree(void *v, uintptr n, uint64 *stat)
{
	runtime·xadd64(stat, -(uint64)n);
	runtime·munmap(v, n);
}

void
runtime·SysFault(void *v, uintptr n)
{
	runtime·mmap(v, n, PROT_NONE, MAP_ANON|MAP_PRIVATE|MAP_FIXED, -1, 0);
}

int32 runtime·kern_reserve_address_range(void* v, int32 address_spec, uintptr n);

void*
runtime·SysReserve(void *v, uintptr n, bool *reserved)
{
	void *p;

	// On 64-bit, people with ulimit -v set complain if we reserve too
	// much address space.  Instead, assume that the reservation is okay
	// if we can reserve at least 64K and check the assumption in SysMap.
	// Only user-mode Linux (UML) rejects these requests.
	if(sizeof(void*) == 8 && n > 1LL<<32) {
		p = mmap_fixed(v, 64<<10, PROT_NONE, MAP_ANON|MAP_PRIVATE, -1, 0);
		if (p != v) {
			if(p >= (void*)4096)
				runtime·munmap(p, 64<<10);
			return nil;
		}
		runtime·munmap(p, 64<<10);
		*reserved = false;
		return v;
	}

	//int32 status = runtime·kern_reserve_address_range(&p, 0x7 /* B_RANDOMIZED_BASE_ADDRESS */, 64<<10);
	*reserved = false;
	p = runtime·mmap(v, n, PROT_NONE, MAP_ANON|MAP_PRIVATE, -1, 0);
	//that doesn't work: PROT_NONE still reserves physical memory
	//so immediately unmap it and mark it as unreserved
	if((uintptr)p < 4096)
		return nil;
	//if (status) {
	//	return nil;
	//}
	runtime·munmap(p, n);
	return p;
}

void
runtime·SysMap(void *v, uintptr n, bool reserved, uint64 *stat)
{
	void *p;

	runtime·xadd64(stat, n);

	// On 64-bit, we don't actually have v reserved, so tread carefully.
	if(!reserved) {
		p = mmap_fixed(v, n, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
		if(p == (void*)ENOMEM)
			runtime·throw("runtime: out of memory");
		if(p != v) {
			runtime·printf("runtime: address space conflict: map(%p) = %p\n", v, p);
			runtime·throw("runtime: address space conflict");
		}
		return;
	}

	p = runtime·mmap(v, n, PROT_READ|PROT_WRITE, MAP_ANON|MAP_FIXED|MAP_PRIVATE, -1, 0);
	if(p == (void*)ENOMEM)
		runtime·throw("runtime: out of memory");
	if(p != v)
		runtime·throw("runtime: cannot map pages in arena address space");
}
