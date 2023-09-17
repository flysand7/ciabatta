
#pragma once

#define SIGHUP          1
#define SIGINT          2
#define SIGQUIT         3
#define SIGILL          4
#define SIGTRAP         5
#define SIGABRT         6
#define SIGBUS          7
#define SIGFPE          8
#define SIGKILL         9
#define SIGUSR1        10
#define SIGSEGV        11
#define SIGUSR2        12
#define SIGPIPE        13
#define SIGALRM        14
#define SIGTERM        15
#define SIGSTKFLT      16
#define SIGCHLD        17
#define SIGCONT        18
#define SIGSTOP        19
#define SIGTSTP        20
#define SIGTTIN        21
#define SIGTTOU        22
#define SIGURG         23
#define SIGXCPU        24
#define SIGXFSZ        25
#define SIGVTALRM      26
#define SIGPROF        27
#define SIGWINCH       28
#define SIGPOLL        29
#define SIGPWR         30
#define SIGSYS         31
#define SIGCLD    SIGCHLD
#define SIGIO     SIGPOLL
#define SIGIOT    SIGABRT

// Custom guy
#define SIGCANCEL      33

#define SA_NOCLDSTOP 1
#define SA_NOCLDWAIT 2
#define SA_SIGINFO   4
#define SA_ONSTACK   0x08000000
#define SA_RESTART   0x10000000
#define SA_INTERRUPT 0x20000000
#define SA_NODEFER   0x40000000
#define SA_RESETHAND 0x80000000
#define SA_NOMASK    SA_NODEFER
#define SA_ONESHOT   SA_RESETHAND
#define SA_STACK     SA_ONSTACK

#define SIG_BLOCK 0
#define SIG_SETMASK 2
#define SIG_UNBLOCK 1

union sigval {
    i32   sival_int;
    void *sival_ptr;
};

struct siginfo_t typedef siginfo_t;
struct siginfo_t {
    i32 si_signo;
    i32 si_errno;
    i32 si_code;
    i32 si_trapno;
    u32 si_pid;
    u32 si_uid;
    i32 si_status;
    u64 si_utime;
    u64 si_stime;
    union sigval si_value;
    i32    si_int;
    void  *si_ptr;
    i32    si_overrun;
    i32    si_timerid;
    void  *si_addr;
    long   si_band;
    i32    si_fd;
    short  si_addr_lsb;
    void  *si_lower;
    void  *si_upper;
    i32    si_pkey;
    void  *si_call_addr;
    i32    si_syscall;
    u32    si_arch;
};


struct sigaction {
    union {
        void (*sa_handler)(i32);
        void (*sa_sigaction)(i32, siginfo_t *, void *);
    };
    u64 sa_mask;
    i32 sa_flags;
    void (*sa_restorer)(void);
};

static inline i32 sys_sigaction(i32 signum, const struct sigaction *restrict act, struct sigaction *restrict oldact) {
    return (i32)syscall(SYS_rt_sigaction, signum, act, oldact, 1);
}

