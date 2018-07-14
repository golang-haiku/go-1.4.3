// Copyright 2009 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

// +build ignore

/*
Input to cgo.

GOARCH=amd64 go tool cgo -cdefs defs_haiku.go >defs_haiku_amd64.h
GOARCH=386 go tool cgo -cdefs defs_haiku.go >defs_haiku_386.h
GOARCH=arm go tool cgo -cdefs defs_haiku.go >defs_haiku_arm.h
*/

package runtime

/*
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <errno.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <pthread.h>

#include <kernel/OS.h>

#ifdef __x86_64__
struct extended_regs {
	// not used anywhere; just to make this file compile (stub of struct used in the 386 version)
};
#else
struct xmm_regs {
	// not used anywhere, just to make this compile (stub of struct used in x86_64)
}
struct fpu_state {
	// ditto
}
#endif
*/
import "C"

const (
	EINTR     = C.EINTR
	EFAULT    = C.EFAULT
	EAGAIN    = C.EAGAIN
	ENOMEM    = C.ENOMEM
	ETIMEDOUT = C.ETIMEDOUT
	EACCES    = C.EACCES

	PROT_NONE  = C.PROT_NONE
	PROT_READ  = C.PROT_READ
	PROT_WRITE = C.PROT_WRITE
	PROT_EXEC  = C.PROT_EXEC

	MAP_ANON    = C.MAP_ANON
	MAP_PRIVATE = C.MAP_PRIVATE
	MAP_FIXED   = C.MAP_FIXED

	SA_SIGINFO = C.SA_SIGINFO
	SA_RESTART = C.SA_RESTART
	SA_ONSTACK = C.SA_ONSTACK

	SIGHUP    = C.SIGHUP
	SIGINT    = C.SIGINT
	SIGQUIT   = C.SIGQUIT
	SIGILL    = C.SIGILL
	SIGTRAP   = C.SIGTRAP
	SIGABRT   = C.SIGABRT
	SIGFPE    = C.SIGFPE
	SIGKILL   = C.SIGKILL
	SIGBUS    = C.SIGBUS
	SIGSEGV   = C.SIGSEGV
	SIGSYS    = C.SIGSYS
	SIGPIPE   = C.SIGPIPE
	SIGALRM   = C.SIGALRM
	SIGTERM   = C.SIGTERM
	SIGURG    = C.SIGURG
	SIGSTOP   = C.SIGSTOP
	SIGTSTP   = C.SIGTSTP
	SIGCONT   = C.SIGCONT
	SIGCHLD   = C.SIGCHLD
	SIGTTIN   = C.SIGTTIN
	SIGTTOU   = C.SIGTTOU
	SIGXCPU   = C.SIGXCPU
	SIGXFSZ   = C.SIGXFSZ
	SIGVTALRM = C.SIGVTALRM
	SIGPROF   = C.SIGPROF
	SIGWINCH  = C.SIGWINCH
	SIGUSR1   = C.SIGUSR1
	SIGUSR2   = C.SIGUSR2

	FPE_INTDIV = C.FPE_INTDIV
	FPE_INTOVF = C.FPE_INTOVF
	FPE_FLTDIV = C.FPE_FLTDIV
	FPE_FLTOVF = C.FPE_FLTOVF
	FPE_FLTUND = C.FPE_FLTUND
	FPE_FLTRES = C.FPE_FLTRES
	FPE_FLTINV = C.FPE_FLTINV
	FPE_FLTSUB = C.FPE_FLTSUB

	BUS_ADRALN = C.BUS_ADRALN
	BUS_ADRERR = C.BUS_ADRERR
	BUS_OBJERR = C.BUS_OBJERR

	SEGV_MAPERR = C.SEGV_MAPERR
	SEGV_ACCERR = C.SEGV_ACCERR

	ITIMER_REAL    = C.ITIMER_REAL
	ITIMER_VIRTUAL = C.ITIMER_VIRTUAL
	ITIMER_PROF    = C.ITIMER_PROF

	PTHREAD_CREATE_DETACHED = C.PTHREAD_CREATE_DETACHED

	HOST_NAME_MAX = C.HOST_NAME_MAX

	O_NONBLOCK = C.O_NONBLOCK
	FD_CLOEXEC = C.FD_CLOEXEC
	F_GETFL    = C.F_GETFL
	F_SETFL    = C.F_SETFL
	F_SETFD    = C.F_SETFD

	_SC_NPROCESSORS_ONLN = C._SC_NPROCESSORS_ONLN

	_MAXHOSTNAMELEN = C.MAXHOSTNAMELEN

	B_PAGE_SIZE = C.B_PAGE_SIZE

	B_ERROR = C.B_ERROR
)

type SemT C.sem_t

type Sigset C.sigset_t
type StackT C.stack_t
type SigaltstackT StackT

type Siginfo C.siginfo_t
type Sigaction C.struct_sigaction

// needed by Mcontext
// for 386
type ExtendedRegs C.struct_extended_regs

// for amd64
type XmmRegs C.struct_xmm_regs
type FpuState C.struct_fpu_state

type Mcontext C.mcontext_t
type Ucontext C.ucontext_t

type Timespec C.struct_timespec
type Timeval C.struct_timeval
type Itimerval C.struct_itimerval

type Pthread C.pthread_t
type PthreadAttr C.pthread_attr_t

// depends on Timespec, must appear below
type Stat C.struct_stat
