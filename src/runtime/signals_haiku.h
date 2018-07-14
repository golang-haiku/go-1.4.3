// Copyright 2009 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "textflag.h"

#define N SigNotify
#define K SigKill
#define T SigThrow
#define P SigPanic
#define D SigDefault

#pragma dataflag NOPTR
SigTab runtime·sigtab[] = {
	/* 0 */	0, "SIGNONE: no trap",
	/* 1 */	N+K, "SIGHUP: hangup",
	/* 2 */	N+K, "SIGINT: interrupt",
	/* 3 */	N+T, "SIGQUIT: quit",
	/* 4 */	T, "SIGILL: illegal instruction",
	/* 5 */	N, "SIGCHLD: child exited",
	/* 6 */	N+T, "SIGABRT: abort",
	/* 7 */	N, "SIGPIPE: broken pipe",
	/* 8 */	P, "SIGFPE: floating point exception",
	/* 9 */	0, "SIGKILL: killed (by death)",
	/* 10 */	0, "SIGSTOP: stopped",
	/* 11 */	P, "SIGSEGV: segmentation violation",
	/* 12 */	0, "SIGCONT: continued",
	/* 13 */	N+D, "SIGTSTP: stopped (tty output)",
	/* 14 */	N, "SIGALRM: alarm",
	/* 15 */	N+K, "SIGTERM: termination requested",
	/* 16 */	N+D, "SIGTTIN: stopped (tty input)",
	/* 17 */	N+D, "SIGTTOU: stopped (tty output)",
	/* 18 */	N, "SIGUSR1: user defined signal 1",
	/* 19 */	N, "SIGUSR2: user defined signal 2",
	/* 20 */	N, "SIGWINCH: window size changed",
	/* 21 */	N+K, "SIGKILLTHR: kill Thread",
	/* 22 */	T, "SIGTRAP: trace/breakpoint trap",
	/* 23 */	N, "SIGPOLL: pollable event",
	/* 24 */	N, "SIGPROF: profiling timer expired",
	/* 25 */	N, "SIGSYS: bad system call",
	/* 26 */	N, "SIGURG: high bandwidth data is available at socket",
	/* 27 */	N, "SIGVTALRM: virtual timer expired",
	/* 28 */	N, "SIGXCPU: CPU time limit exceeded",
	/* 29 */	N, "SIGXFSZ: file size limit exceeded",
	/* 30 */	P, "SIGBUS: bus error",
	/* 31 */	N, "SIGRESERVED1: reserved signal 1",
	/* 32 */	N, "SIGRESERVED2: reserved signal 2",
	/* 33 */	N, "signal 33",
	/* 34 */	N, "signal 34",
	/* 35 */	N, "signal 35",
	/* 36 */	N, "signal 36",
	/* 37 */	N, "signal 37",
	/* 38 */	N, "signal 38",
	/* 39 */	N, "signal 39",
	/* 40 */	N, "signal 40",
	/* 41 */	N, "signal 41",
	/* 42 */	N, "signal 42",
	/* 43 */	N, "signal 43",
	/* 44 */	N, "signal 44",
	/* 45 */	N, "signal 45",
	/* 46 */	N, "signal 46",
	/* 47 */	N, "signal 47",
	/* 48 */	N, "signal 48",
	/* 49 */	N, "signal 49",
	/* 50 */	N, "signal 50",
	/* 51 */	N, "signal 51",
	/* 52 */	N, "signal 52",
	/* 53 */	N, "signal 53",
	/* 54 */	N, "signal 54",
	/* 55 */	N, "signal 55",
	/* 56 */	N, "signal 56",
	/* 57 */	N, "signal 57",
	/* 58 */	N, "signal 58",
	/* 59 */	N, "signal 59",
	/* 60 */	N, "signal 60",
	/* 61 */	N, "signal 61",
	/* 62 */	N, "signal 62",
	/* 63 */	N, "signal 63",
	/* 64 */	N, "signal 64",
};

#undef N
#undef K
#undef T
#undef P
#undef D
