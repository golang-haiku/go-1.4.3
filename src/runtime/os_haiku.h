// Copyright 2009 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

// Linux-specific system calls
//int32	runtime·futex(uint32*, int32, uint32, Timespec*, uint32*, uint32);
//int32	runtime·clone(int32, void*, M*, G*, void(*)(void));

struct SigactionT;
void	runtime·sigaction(int32, struct SigactionT*, struct SigactionT*);

void	runtime·sigaltstack(SigaltstackT*, SigaltstackT*);
void	runtime·sigpanic(void);
void runtime·setitimer(int32, Itimerval*, Itimerval*);

enum {
	SS_DISABLE = 2,
	NSIG = 65,
	SI_USER = 0,
	SIG_SETMASK = 3, // Modified for Haiku support
	RLIMIT_AS = 6, // Modified for Haiku support
};

void	runtime·sigprocmask(int32, Sigset*, Sigset*);
void	runtime·unblocksignals(void);

typedef struct Rlimit Rlimit;
struct Rlimit {
	uintptr	rlim_cur;
	uintptr	rlim_max;
};
int32	runtime·getrlimit(int32, Rlimit*);

// Call an external library function described by {fn, a0, ..., an}, with
// SysV conventions, switching to os stack during the call, if necessary.

uintptr	runtime·sysvicall0(uintptr fn);
uintptr	runtime·sysvicall1(uintptr fn, uintptr a1);
uintptr	runtime·sysvicall2(uintptr fn, uintptr a1, uintptr a2);
uintptr	runtime·sysvicall3(uintptr fn, uintptr a1, uintptr a2, uintptr a3);
uintptr	runtime·sysvicall4(uintptr fn, uintptr a1, uintptr a2, uintptr a3, uintptr a4);
uintptr	runtime·sysvicall5(uintptr fn, uintptr a1, uintptr a2, uintptr a3, uintptr a4, uintptr a5);
uintptr	runtime·sysvicall6(uintptr fn, uintptr a1, uintptr a2, uintptr a3, uintptr a4, uintptr a5, uintptr a6);
void	runtime·asmsysvicall6(void *c);
void	runtime·miniterrno(void *fn);
