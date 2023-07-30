
// Standard handles file descriptors
#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

// arch_prctl() syscall codes
#define ARCH_SET_GS    0x1001
#define ARCH_SET_FS    0x1002
#define ARCH_GET_FS    0x1003
#define ARCH_GET_GS    0x1004
#define ARCH_GET_CPUID 0x1011
#define ARCH_SET_CPUID 0x1012

// open() syscall modes
#define O_ACCMODE   0003
#define O_RDONLY    00
#define O_WRONLY    01
#define O_RDWR      02
#define O_CREAT     0100     /* not fcntl */
#define O_EXCL      0200     /* not fcntl */
#define O_NOCTTY    0400     /* not fcntl */
#define O_TRUNC     01000    /* not fcntl */
#define O_APPEND    02000
#define O_NONBLOCK  04000
#define O_NDELAY    O_NONBLOCK
#define O_SYNC      010000
#define O_FSYNC     O_SYNC
#define O_ASYNC     020000

// mmap() protection modes, flags and constants
#define PROT_READ             0x1
#define PROT_WRITE            0x2
#define PROT_EXEC             0x4
#define PROT_NONE             0x0
#define PROT_GROWSDOWN 0x01000000
#define PROT_GROWSUP   0x02000000
#define MAP_FILE                0
#define MAP_ANONYMOUS        0x20
#define MAP_32BIT            0x40
#define MAP_FAILED (void *)()

#define SYS_read           0
#define SYS_write          1
#define SYS_open           2
#define SYS_close          3
#define SYS_stat           4
#define SYS_fstat          5
#define SYS_lstat          6
#define SYS_poll           7
#define SYS_lseek          8
#define SYS_mmap           9
#define SYS_mprotect       10
#define SYS_munmap         11
#define SYS_brk            12
#define SYS_rt_sigaction   13
#define SYS_rt_sigprocmask 14
#define SYS_rt_sigreturn   15
#define SYS_ioctl          16
#define SYS_pread64        17
#define SYS_pwrite64       18
#define SYS_readv          19
#define SYS_writev         20
#define SYS_access         21
#define SYS_pipe           22
#define SYS_select         23
#define SYS_sched_yield    24
#define SYS_mremap         25
#define SYS_msync          26
#define SYS_mincore        27
#define SYS_madvise        28
#define SYS_shmget         29
#define SYS_shmat          30
#define SYS_shmctl         31
#define SYS_dup            32
#define SYS_dup2           33
#define SYS_pause          34
#define SYS_nanosleep      35
#define SYS_getitimer      36
#define SYS_alarm          37
#define SYS_setitimer      38
#define SYS_getpid         39
#define SYS_sendfile       40
#define SYS_socket         41
#define SYS_connect        42
#define SYS_accept         43
#define SYS_sendto         44
#define SYS_recvfrom       45
#define SYS_sendmsg        46
#define SYS_recvmsg        47
#define SYS_shutdown       48
#define SYS_bind           49
#define SYS_listen         50
#define SYS_getsockname    51
#define SYS_getpeername    52
#define SYS_socketpair     53
#define SYS_setsockopt     54
#define SYS_getsockopt     55
#define SYS_clone          56
#define SYS_fork           57
#define SYS_vfork          58
#define SYS_execve         59
#define SYS_exit           60

#define SYS_arch_prctl     158

// Syscall stubs

static __inline i64 __syscall0(i64 n) {
    i64 ret;
    __asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n) : "rcx", "r11", "memory");
    return ret;
}

static __inline i64 __syscall1(i64 n, i64 a1) {
    i64 ret;
    __asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1) : "rcx", "r11", "memory");
    return ret;
}

static __inline i64 __syscall2(i64 n, i64 a1, i64 a2) {
    i64 ret;
    __asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2)
                          : "rcx", "r11", "memory");
    return ret;
}

static __inline i64 __syscall3(i64 n, i64 a1, i64 a2, i64 a3) {
    i64 ret;
    __asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
                          "d"(a3) : "rcx", "r11", "memory");
    return ret;
}

static __inline i64 __syscall4(i64 n, i64 a1, i64 a2, i64 a3, i64 a4) {
    i64 ret;
    register i64 r10 __asm__("r10") = a4;
    __asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
                          "d"(a3), "r"(r10): "rcx", "r11", "memory");
    return ret;
}

static __inline i64 __syscall5(i64 n, i64 a1, i64 a2, i64 a3, i64 a4, i64 a5) {
    i64 ret;
    register i64 r10 __asm__("r10") = a4;
    register i64 r8 __asm__("r8") = a5;
    __asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
                          "d"(a3), "r"(r10), "r"(r8) : "rcx", "r11", "memory");
    return ret;
}

static __inline i64 __syscall6(i64 n, i64 a1, i64 a2, i64 a3, i64 a4, i64 a5, i64 a6) {
    i64 ret;
    register i64 r10 __asm__("r10") = a4;
    register i64 r8 __asm__("r8") = a5;
    register i64 r9 __asm__("r9") = a6;
    __asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
                          "d"(a3), "r"(r10), "r"(r8), "r"(r9) : "rcx", "r11", "memory");
    return ret;
}

// Syscall wrappers

static inline i64 syscall_read(u32 fd, char *buf, u64 count) {
    return __syscall3(SYS_read, (i64)fd, (i64)buf, (i64)count);
}

static inline i64 syscall_write(u32 fd, char const *buf, u64 count) {
    return __syscall3(SYS_write, (i64)fd, (i64)buf, (u64)count);
}

static inline i64 syscall_open(char const *filename, int flags, int mode) {
    return __syscall3(SYS_open, (i64)filename, (i64)flags, (i64)mode);
}

static inline i64 syscall_close(u32 fd) {
    return __syscall1(SYS_close, fd);
}

static inline void *syscall_mmap(u64 addr, u64 len, u64 prot, u64 flags, u64 fd, u64 offset) {
    return (void *)__syscall6(SYS_mmap, addr, len, prot, flags, fd, offset);
}

static inline i64 syscall_munmap(void *addr, u64 len) {
    return __syscall2(SYS_munmap, (u64)addr, len);
}

_Noreturn static inline void syscall_exit(int code) {
    __syscall1(SYS_exit, (i64)code);
    __builtin_unreachable();
}

static inline i64 syscall_arch_prctl_set(int code, u64 value) {
    return __syscall2(SYS_arch_prctl, code, (i64)value);
}

static inline i64 syscall_arch_prctl_get(int code, u64 *value) {
    return __syscall2(SYS_arch_prctl, code, (i64)value);
}
