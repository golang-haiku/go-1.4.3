// Created by cgo -cdefs - DO NOT EDIT
// cgo -cdefs defs_haiku.go


enum {
	EINTR		= -0x7ffffff6,
	EFAULT		= -0x7fffecff,
	EAGAIN		= -0x7ffffff5,
	ENOMEM		= -0x80000000,
	ETIMEDOUT	= -0x7ffffff7,
	EACCES		= -0x7ffffffe,

	PROT_NONE	= 0x0,
	PROT_READ	= 0x1,
	PROT_WRITE	= 0x2,
	PROT_EXEC	= 0x4,

	MAP_ANON	= 0x8,
	MAP_PRIVATE	= 0x2,
	MAP_FIXED	= 0x4,

	SA_SIGINFO	= 0x40,
	SA_RESTART	= 0x10,
	SA_ONSTACK	= 0x20,

	SIGHUP		= 0x1,
	SIGINT		= 0x2,
	SIGQUIT		= 0x3,
	SIGILL		= 0x4,
	SIGTRAP		= 0x16,
	SIGABRT		= 0x6,
	SIGFPE		= 0x8,
	SIGKILL		= 0x9,
	SIGBUS		= 0x1e,
	SIGSEGV		= 0xb,
	SIGSYS		= 0x19,
	SIGPIPE		= 0x7,
	SIGALRM		= 0xe,
	SIGTERM		= 0xf,
	SIGURG		= 0x1a,
	SIGSTOP		= 0xa,
	SIGTSTP		= 0xd,
	SIGCONT		= 0xc,
	SIGCHLD		= 0x5,
	SIGTTIN		= 0x10,
	SIGTTOU		= 0x11,
	SIGXCPU		= 0x1c,
	SIGXFSZ		= 0x1d,
	SIGVTALRM	= 0x1b,
	SIGPROF		= 0x18,
	SIGWINCH	= 0x14,
	SIGUSR1		= 0x12,
	SIGUSR2		= 0x13,

	FPE_INTDIV	= 0x14,
	FPE_INTOVF	= 0x15,
	FPE_FLTDIV	= 0x16,
	FPE_FLTOVF	= 0x17,
	FPE_FLTUND	= 0x18,
	FPE_FLTRES	= 0x19,
	FPE_FLTINV	= 0x1a,
	FPE_FLTSUB	= 0x1b,

	BUS_ADRALN	= 0x28,
	BUS_ADRERR	= 0x29,
	BUS_OBJERR	= 0x2a,

	SEGV_MAPERR	= 0x1e,
	SEGV_ACCERR	= 0x1f,

	ITIMER_REAL	= 0x1,
	ITIMER_VIRTUAL	= 0x2,
	ITIMER_PROF	= 0x3,

	PTHREAD_CREATE_DETACHED	= 0x1,

	HOST_NAME_MAX	= 0xff,

	O_NONBLOCK	= 0x80,
	FD_CLOEXEC	= 0x1,
	F_GETFL		= 0x8,
	F_SETFL		= 0x10,
	F_SETFD		= 0x4,

	_SC_NPROCESSORS_ONLN	= 0x23,

	_MAXHOSTNAMELEN	= 0x100,

	B_PAGE_SIZE	= 0x1000,

	B_ERROR	= -0x1,
};

typedef struct SemT SemT;
typedef struct StackT StackT;
typedef struct Siginfo Siginfo;
typedef struct SigactionT SigactionT;
typedef struct ExtendedRegs ExtendedRegs;
typedef struct XmmRegs XmmRegs;
typedef struct FpuState FpuState;
typedef struct Mcontext Mcontext;
typedef struct Ucontext Ucontext;
typedef struct Timespec Timespec;
typedef struct Timeval Timeval;
typedef struct Itimerval Itimerval;
typedef struct Stat Stat;

#pragma pack on

struct SemT {
	int32	id;
	int32	_padding[3];
};

typedef	uint64	Sigset;
struct StackT {
	byte	*ss_sp;
	uint64	ss_size;
	int32	ss_flags;
	byte	Pad_cgo_0[4];
};
typedef	StackT	SigaltstackT;

struct Siginfo {
	int32	si_signo;
	int32	si_code;
	int32	si_errno;
	int32	si_pid;
	uint32	si_uid;
	byte	Pad_cgo_0[4];
	byte	*si_addr;
	int32	si_status;
	byte	Pad_cgo_1[4];
	int64	si_band;
	byte	si_value[8];
};
struct SigactionT {
	byte	anon0[8];
	uint64	sa_mask;
	int32	sa_flags;
	byte	Pad_cgo_0[4];
	byte	*sa_userdata;
};


struct XmmRegs {
	uint8	xmm0[16];
	uint8	xmm1[16];
	uint8	xmm2[16];
	uint8	xmm3[16];
	uint8	xmm4[16];
	uint8	xmm5[16];
	uint8	xmm6[16];
	uint8	xmm7[16];
	uint8	xmm8[16];
	uint8	xmm9[16];
	uint8	xmm10[16];
	uint8	xmm11[16];
	uint8	xmm12[16];
	uint8	xmm13[16];
	uint8	xmm14[16];
	uint8	xmm15[16];
};
struct FpuState {
	uint16	control;
	uint16	status;
	uint16	tag;
	uint16	opcode;
	uint64	rip;
	uint64	rdp;
	uint32	mxcsr;
	uint32	mscsr_mask;
	byte	anon0[128];
	XmmRegs	xmm;
	uint8	_reserved_416_511[96];
};

struct Mcontext {
	uint64	rax;
	uint64	rbx;
	uint64	rcx;
	uint64	rdx;
	uint64	rdi;
	uint64	rsi;
	uint64	rbp;
	uint64	r8;
	uint64	r9;
	uint64	r10;
	uint64	r11;
	uint64	r12;
	uint64	r13;
	uint64	r14;
	uint64	r15;
	uint64	rsp;
	uint64	rip;
	uint64	rflags;
	FpuState	fpu;
};
struct Ucontext {
	Ucontext	*uc_link;
	uint64	uc_sigmask;
	StackT	uc_stack;
	Mcontext	uc_mcontext;
};

struct Timespec {
	int32	tv_sec;
	byte	Pad_cgo_0[4];
	int64	tv_nsec;
};
struct Timeval {
	int32	tv_sec;
	int32	tv_usec;
};
struct Itimerval {
	Timeval	it_interval;
	Timeval	it_value;
};

typedef	void	*Pthread;
typedef	void	*PthreadAttr;

struct Stat {
	int32	st_dev;
	byte	Pad_cgo_0[4];
	int64	st_ino;
	uint32	st_mode;
	int32	st_nlink;
	uint32	st_uid;
	uint32	st_gid;
	int64	st_size;
	int32	st_rdev;
	int32	st_blksize;
	Timespec	st_atim;
	Timespec	st_mtim;
	Timespec	st_ctim;
	Timespec	st_crtim;
	uint32	st_type;
	byte	Pad_cgo_1[4];
	int64	st_blocks;
};


#pragma pack off
