// Copyright 2011 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "runtime.h"
#include "defs_GOOS_GOARCH.h"
#include "os_GOOS.h"
#include "signal_unix.h"
#include "stack.h"
#include "textflag.h"

// dynimport is broken in 386: one needs the address instead of the value
// contained in each imported variable
#ifdef GOARCH_386
#define FUNC(a) ((uintptr) &a)
#else
#define FUNC(a) (a)
#endif

#pragma dynexport runtime·end _end
#pragma dynexport runtime·etext _etext
#pragma dynexport runtime·edata _edata

#pragma dynimport libc·_errnop _errnop "libroot.so"
#pragma dynimport libc·clock_gettime clock_gettime "libroot.so"
#pragma dynimport libc·close close "libroot.so"
#pragma dynimport libc·exit exit "libroot.so"
#pragma dynimport libc·fstat fstat "libroot.so"
#pragma dynimport libc·getcontext getcontext "libroot.so"
#pragma dynimport libc·getrlimit getrlimit "libroot.so"
#pragma dynimport libc·malloc malloc "libroot.so"
#pragma dynimport libc·mmap mmap "libroot.so"
#pragma dynimport libc·munmap munmap "libroot.so"
#pragma dynimport libc·open open "libroot.so"
#pragma dynimport libc·pthread_attr_destroy pthread_attr_destroy "libroot.so"
#pragma dynimport libc·pthread_attr_getstacksize pthread_attr_getstacksize "libroot.so"
#pragma dynimport libc·pthread_attr_init pthread_attr_init "libroot.so"
#pragma dynimport libc·pthread_attr_setdetachstate pthread_attr_setdetachstate "libroot.so"
#pragma dynimport libc·pthread_attr_setstack pthread_attr_setstack "libroot.so"
#pragma dynimport libc·pthread_create pthread_create "libroot.so"
#pragma dynimport libc·raise raise "libroot.so"
#pragma dynimport libc·read read "libroot.so"
#pragma dynimport libc·select select "libroot.so"
#pragma dynimport libc·sched_yield sched_yield "libroot.so"
#pragma dynimport libc·sem_init sem_init "libroot.so"
#pragma dynimport libc·sem_post sem_post "libroot.so"
#pragma dynimport libc·sem_timedwait sem_timedwait "libroot.so"
#pragma dynimport libc·sem_wait sem_wait "libroot.so"
#pragma dynimport libc·setitimer setitimer "libroot.so"
#pragma dynimport libc·sigaction sigaction "libroot.so"
#pragma dynimport libc·sigaltstack sigaltstack "libroot.so"
#pragma dynimport libc·sigprocmask sigprocmask "libroot.so"
#pragma dynimport libc·sysconf sysconf "libroot.so"
#pragma dynimport libc·usleep usleep "libroot.so"
#pragma dynimport libc·write write "libroot.so"
#pragma dynimport libc·area_for area_for "libroot.so"

extern uintptr libc·_errnop;
extern uintptr libc·clock_gettime;
extern uintptr libc·close;
extern uintptr libc·exit;
extern uintptr libc·fstat;
extern uintptr libc·getcontext;
extern uintptr libc·getrlimit;
extern uintptr libc·malloc;
extern uintptr libc·mmap;
extern uintptr libc·munmap;
extern uintptr libc·open;
extern uintptr libc·pthread_attr_destroy;
extern uintptr libc·pthread_attr_getstacksize;
extern uintptr libc·pthread_attr_init;
extern uintptr libc·pthread_attr_setdetachstate;
extern uintptr libc·pthread_attr_setstack;
extern uintptr libc·pthread_create;
extern uintptr libc·raise;
extern uintptr libc·read;
extern uintptr libc·sched_yield;
extern uintptr libc·select;
extern uintptr libc·sem_init;
extern uintptr libc·sem_post;
extern uintptr libc·sem_timedwait;
extern uintptr libc·sem_wait;
extern uintptr libc·setitimer;
extern uintptr libc·sigaction;
extern uintptr libc·sigaltstack;
extern uintptr libc·sigprocmask;
extern uintptr libc·sysconf;
extern uintptr libc·usleep;
extern uintptr libc·write;
extern uintptr libc·area_for;

void	runtime·getcontext(Ucontext *context);
int32	runtime·pthread_attr_destroy(PthreadAttr* attr);
int32	runtime·pthread_attr_init(PthreadAttr* attr);
//TODO: uint32/uint64 problem needs to be solved
int32	runtime·pthread_attr_getstacksize(PthreadAttr* attr, uintptr* size);
int32	runtime·pthread_attr_setdetachstate(PthreadAttr* attr, int32 state);
int32	runtime·pthread_attr_setstack(PthreadAttr* attr, void* addr, uint32 size);
int32	runtime·pthread_create(Pthread* thread, PthreadAttr* attr, void(*fn)(void), void *arg);
uint32	runtime·tstart_sysvicall(M *newm);
int32	runtime·sem_init(SemT* sem, int32 pshared, uint32 value);
int32	runtime·sem_post(SemT* sem);
//int32	runtime·sem_reltimedwait(SemT* sem, Timespec* timeout);
int32	runtime·sem_wait(SemT* sem);
int64	runtime·sysconf(int32 name);

extern SigTab runtime·sigtab[];
static Sigset sigset_none = 0;
static Sigset sigset_all = 0;

static int32
getncpu(void)
{
	int32 n;

	n = (int32)runtime·sysconf(_SC_NPROCESSORS_ONLN);
	if(n < 1)
		return 1;
	return n;
}

void
runtime·osinit(void)
{
	runtime·ncpu = getncpu();
}

void
runtime·newosproc(M *mp, void *stk)
{
	PthreadAttr attr;
	Sigset oset;
	Pthread tid;
	int32 ret;
	uint64 size;

	USED(stk);
	if(runtime·pthread_attr_init(&attr) != 0)
		runtime·throw("pthread_attr_init");
	//if(runtime·pthread_attr_setstack(&attr, 0, 0x200000) != 0)
	//	runtime·throw("pthread_attr_setstack");
	//if(runtime·pthread_attr_getstack(&attr, (void**)&mp->g0->stackbase, &mp->g0->stacksize) != 0)
	//	runtime·throw("pthread_attr_getstack");
	
	if(runtime·pthread_attr_getstacksize(&attr, &mp->g0->stack.hi) != 0)
		runtime·throw("pthread_attr_getstacksize");
	if(runtime·pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0)
		runtime·throw("pthread_attr_setdetachstate");

	// Disable signals during create, so that the new thread starts
	// with signals disabled.  It will enable them in minit.
	runtime·sigprocmask(SIG_SETMASK, &sigset_all, &oset);
	mp->g0->stack.hi = 0xdeadf00d;
	ret = runtime·pthread_create(&tid, &attr, (void (*)(void))runtime·tstart_sysvicall, mp);
	runtime·sigprocmask(SIG_SETMASK, &oset, nil);
	if(ret != 0) {
		runtime·printf("runtime: failed to create new OS thread (have %d already; errno=%d)\n", runtime·mcount(), ret);
		runtime·throw("runtime.newosproc");
	}
}

#pragma textflag NOSPLIT
void
runtime·get_random_data(byte **rnd, int32 *rnd_len)
{
	static byte urandom_data[HashRandomBytes];
	int32 fd;
	fd = runtime·open("/dev/urandom", 0 /* O_RDONLY */, 0);
	if(runtime·read(fd, urandom_data, HashRandomBytes) == HashRandomBytes) {
		*rnd = urandom_data;
		*rnd_len = HashRandomBytes;
	} else {
		*rnd = nil;
		*rnd_len = 0;
	}
	runtime·close(fd);
}

void
runtime·goenvs(void)
{
	runtime·goenvs_unix();
}

// Called to initialize a new m (including the bootstrap m).
// Called on the parent thread (main thread in case of bootstrap), can allocate memory.
void
runtime·mpreinit(M *mp)
{
	mp->gsignal = runtime·malg(32*1024);
	mp->gsignal->m = mp;
}

// Called to initialize a new m (including the bootstrap m).
// Called on the new thread, can not allocate memory.
void
runtime·minit(void)
{
	runtime·asmcgocall(runtime·miniterrno, (void *) FUNC(libc·_errnop));
	// Initialize signal handling
	runtime·signalstack((byte*)g->m->gsignal->stack.lo, 32*1024);
	runtime·sigprocmask(SIG_SETMASK, &sigset_none, nil);
}

// Called from dropm to undo the effect of an minit.
void
runtime·unminit(void)
{
	runtime·signalstack(nil, 0);
}

#pragma textflag NOSPLIT
int8*
runtime·signame(int32 sig)
{
	return runtime·sigtab[sig].name;
}

uintptr
runtime·memlimit(void)
{
	/*Rlimit rl;
	extern byte runtime·text[], runtime·end[];
	uintptr used;

	if(runtime·getrlimit(RLIMIT_AS, &rl) != 0)
		return 0;
	if(rl.rlim_cur >= 0x7fffffff)
		return 0;

	// Estimate our VM footprint excluding the heap.
	// Not an exact science: use size of binary plus
	// some room for thread stacks.
	used = runtime·end - runtime·text + (64<<20);
	if(used >= rl.rlim_cur)
		return 0;

	// If there's not at least 16 MB left, we're probably
	// not going to be able to do much.  Treat as no limit.
	rl.rlim_cur -= used;
	if(rl.rlim_cur < (16<<20))
		return 0;

	return rl.rlim_cur - used;
	*/
	return 0;
}

void
runtime·setprof(bool on)
{
	USED(on);
}

extern void runtime·sigtramp(void);

void
runtime·setsig(int32 i, GoSighandler *fn, bool restart)
{
	SigactionT sa;

	runtime·memclr((byte*)&sa, sizeof sa);
	sa.sa_flags = SA_SIGINFO|SA_ONSTACK;
	if(restart)
		sa.sa_flags |= SA_RESTART;
	sa.sa_mask = ~(uint64)0;
	if(fn == runtime·sighandler)
		fn = (void*)runtime·sigtramp;
	*((void**)&sa.anon0[0]) = (void*)fn;
	runtime·sigaction(i, &sa, nil);
}

GoSighandler*
runtime·getsig(int32 i)
{
	SigactionT sa;

	runtime·memclr((byte*)&sa, sizeof sa);
	runtime·sigaction(i, nil, &sa);
	if(*((void**)&sa.anon0[0]) == runtime·sigtramp)
		return runtime·sighandler;
	return *((void**)&sa.anon0[0]);
}

void
runtime·signalstack(byte *p, int32 n)
{
	StackT st;

	st.ss_sp = (void*)p;
	st.ss_size = n;
	st.ss_flags = 0;
	if(p == nil)
		st.ss_flags = SS_DISABLE;
	runtime·sigaltstack(&st, nil);
}

void
runtime·unblocksignals(void)
{
	runtime·sigprocmask(SIG_SETMASK, &sigset_none, nil);
}

#pragma textflag NOSPLIT
uintptr
runtime·semacreate(void)
{
	SemT* sem;

	// Call libc's malloc rather than runtime·malloc.  This will
	// allocate space on the C heap.  We can't call runtime·malloc
	// here because it could cause a deadlock.
	g->m->libcall.fn = (uintptr)(void*) FUNC(libc·malloc);
	g->m->libcall.n = 1;
	runtime·memclr((byte*)&g->m->scratch, sizeof(g->m->scratch));
	g->m->scratch.v[0] = (uintptr)sizeof(*sem);
	g->m->libcall.args = (uintptr)(uintptr*)&g->m->scratch;
	runtime·asmcgocall(runtime·asmsysvicall6, &g->m->libcall);
	sem = (void*)g->m->libcall.r1;
	if(runtime·sem_init(sem, 0, 0) != 0)
		runtime·throw("sem_init");
	return (uintptr)sem;
}

int32   _div64by32(int64 numerator, int32 denominator, int32* pointerToRemainder);

#pragma textflag NOSPLIT
int32
runtime·semasleep(int64 ns)
{
	M *m;

	m = g->m;

	if(ns >= 0) {
#ifdef GOARCH_amd64
		m->ts.tv_sec = ns / 1000000000LL;
		m->ts.tv_nsec = ns % 1000000000LL;
#else
		// because 64-bit divide generates a call to a split stack method, which we can't call
		m->ts.tv_sec = _div64by32(ns, 1000000000, &(m->ts.tv_nsec));
#endif

		m->libcall.fn = (uintptr)(void*) FUNC(libc·sem_timedwait);
		m->libcall.n = 2;
		runtime·memclr((byte*)&m->scratch, sizeof(m->scratch));
		m->scratch.v[0] = m->waitsema;
		m->scratch.v[1] = (uintptr)&m->ts;
		m->libcall.args = (uintptr)(uintptr*)&m->scratch;
		runtime·asmcgocall(runtime·asmsysvicall6, &m->libcall);
		if(*m->perrno != 0) {
			if(*m->perrno == ETIMEDOUT || *m->perrno == EAGAIN || *m->perrno == EINTR)
				return -1;
			runtime·throw("sem_reltimedwait");
		}
		return 0;
	}
	for(;;) {
		m->libcall.fn = (uintptr)(void*) FUNC(libc·sem_wait);
		m->libcall.n = 1;
		runtime·memclr((byte*)&m->scratch, sizeof(m->scratch));
		m->scratch.v[0] = m->waitsema;
		m->libcall.args = (uintptr)(uintptr*)&m->scratch;
		runtime·asmcgocall(runtime·asmsysvicall6, &m->libcall);
		if(m->libcall.r1 == 0)
			break;
		if(*m->perrno == EINTR)
			continue;
		runtime·throw("sem_wait");
	}
	return 0;
}

#pragma textflag NOSPLIT
void
runtime·semawakeup(M *mp)
{
	SemT* sem = (SemT*)mp->waitsema;
	if(runtime·sem_post(sem) != 0)
		runtime·throw("sem_post");
}

int32
runtime·close(int32 fd)
{
	return runtime·sysvicall1(FUNC(libc·close), (uintptr)fd);
}

void
runtime·exit(int32 r)
{
	runtime·sysvicall1(FUNC(libc·exit), (uintptr)r);
}

/* int32 */ void
runtime·getcontext(Ucontext* context)
{
	runtime·sysvicall1(FUNC(libc·getcontext), (uintptr)context);
}

int32
runtime·getrlimit(int32 res, Rlimit* rlp)
{
	return runtime·sysvicall2(FUNC(libc·getrlimit), (uintptr)res, (uintptr)rlp);
}

uint8*
runtime·mmap(byte* addr, uintptr len, int32 prot, int32 flags, int32 fildes, uint32 off)
{
	return (uint8*)runtime·sysvicall6(FUNC(libc·mmap), (uintptr)addr, (uintptr)len, (uintptr)prot, (uintptr)flags, (uintptr)fildes, (uintptr)off);
}

void
runtime·munmap(byte* addr, uintptr len)
{
	runtime·sysvicall2(FUNC(libc·munmap), (uintptr)addr, (uintptr)len);
}

extern void runtime·nanotime1(void);
#pragma textflag NOSPLIT
int64
runtime·nanotime(void)
{
	M *m;

	m = g->m;

	runtime·sysvicall0((uintptr)runtime·nanotime1);
	if (sizeof(void*) == 8) { // on 64-bit the assembly wrapper does the multiplication
		return m->libcall.r1;
	}
	return (m->libcall.r1 * 1000000000LL) + m->libcall.r2;
}

// void
// time·now(int64 sec, int32 usec)
// {
// 	int64 ns;

// 	ns = runtime·nanotime();
// 	sec = ns / 1000000000LL;
// 	usec = ns - sec * 1000000000LL;
// 	FLUSH(&sec);
// 	FLUSH(&usec);
// }

int32
runtime·open(int8* path, int32 oflag, int32 mode)
{
	return runtime·sysvicall3(FUNC(libc·open), (uintptr)path, (uintptr)oflag, (uintptr)mode);
}

int32
runtime·pthread_attr_destroy(PthreadAttr* attr)
{
	return runtime·sysvicall1(FUNC(libc·pthread_attr_destroy), (uintptr)attr);
}

int32
runtime·pthread_attr_getstacksize(PthreadAttr* attr, uintptr* size)
{
	return runtime·sysvicall2(FUNC(libc·pthread_attr_getstacksize), (uintptr)attr, (uintptr)size);
}

int32
runtime·pthread_attr_init(PthreadAttr* attr)
{
	return runtime·sysvicall1(FUNC(libc·pthread_attr_init), (uintptr)attr);
}

int32
runtime·pthread_attr_setdetachstate(PthreadAttr* attr, int32 state)
{
	return runtime·sysvicall2(FUNC(libc·pthread_attr_setdetachstate), (uintptr)attr, (uintptr)state);
}

int32
runtime·pthread_attr_setstack(PthreadAttr* attr, void* addr, uint32 size)
{
	return runtime·sysvicall3(FUNC(libc·pthread_attr_setstack), (uintptr)attr, (uintptr)addr, (uintptr)size);
}

int32
runtime·pthread_create(Pthread* thread, PthreadAttr* attr, void(*fn)(void), void *arg)
{
	return runtime·sysvicall4(FUNC(libc·pthread_create), (uintptr)thread, (uintptr)attr, (uintptr)fn, (uintptr)arg);
}

/* int32 */ void
runtime·raise(int32 sig)
{
	runtime·sysvicall1(FUNC(libc·raise), (uintptr)sig);
}

int32
runtime·read(int32 fd, void* buf, int32 nbyte)
{
	return runtime·sysvicall3(FUNC(libc·read), (uintptr)fd, (uintptr)buf, (uintptr)nbyte);
}

#pragma textflag NOSPLIT
int32
runtime·sem_init(SemT* sem, int32 pshared, uint32 value)
{
	return runtime·sysvicall3(FUNC(libc·sem_init), (uintptr)sem, (uintptr)pshared, (uintptr)value);
}

#pragma textflag NOSPLIT
int32
runtime·sem_post(SemT* sem)
{
	return runtime·sysvicall1(FUNC(libc·sem_post), (uintptr)sem);
}

#pragma textflag NOSPLIT
int32
runtime·sem_wait(SemT* sem)
{
	return runtime·sysvicall1(FUNC(libc·sem_wait), (uintptr)sem);
}

/* int32 */ void
runtime·setitimer(int32 which, Itimerval* value, Itimerval* ovalue)
{
	runtime·sysvicall3(FUNC(libc·setitimer), (uintptr)which, (uintptr)value, (uintptr)ovalue);
}

/* int32 */ void
runtime·sigaction(int32 sig, struct SigactionT* act, struct SigactionT* oact)
{
	runtime·sysvicall3(FUNC(libc·sigaction), (uintptr)sig, (uintptr)act, (uintptr)oact);
}

/* int32 */ void
runtime·sigaltstack(SigaltstackT* ss, SigaltstackT* oss)
{
	runtime·sysvicall2(FUNC(libc·sigaltstack), (uintptr)ss, (uintptr)oss);
}

/* int32 */ void
runtime·sigprocmask(int32 how, Sigset* set, Sigset* oset)
{
	runtime·sysvicall3(FUNC(libc·sigprocmask), (uintptr)how, (uintptr)set, (uintptr)oset);
}

int64
runtime·sysconf(int32 name)
{
	return runtime·sysvicall1(FUNC(libc·sysconf), (uintptr)name);
}

void
runtime·usleep(uint32 us)
{
	runtime·sysvicall1(FUNC(libc·usleep), (uintptr)us);
}

int32
runtime·write(uintptr fd, void* buf, int32 nbyte)
{
	return runtime·sysvicall3(FUNC(libc·write), (uintptr)fd, (uintptr)buf, (uintptr)nbyte);
}

void
runtime·osyield(void)
{
	runtime·sysvicall0(FUNC(libc·sched_yield));
}

uintptr
runtime·area_for(void* addr)
{
	return (uintptr)runtime·sysvicall1(FUNC(libc·area_for), (uintptr)addr);
}

#ifndef GOARCH_amd64
// STUB
void
runtime·pipe1(void)
{
	runtime·throw("Pipe not implemented");
}
#endif
