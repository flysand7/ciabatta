
#include <asm/unistd.h>
#include <asm/signal.h>
#include <asm/ioctls.h>
#include <asm/mman.h>
#include <linux/fs.h>
#include <linux/loop.h>
#include <linux/time.h>
#include <errno.h>

#if !defined(STDIN_FILENO)
    #define STDIN_FILENO  0
    #define STDOUT_FILENO 1
    #define STDERR_FILENO 2
#endif

#if !defined(MAP_SHARED)
    #define MAP_SHARED          0x01
    #define MAP_PRIVATE         0x02
    #define MAP_SHARED_VALIDATE 0x03
#endif

#if !defined(MAP_FAILED)
    #define MAP_FAILED ((void *)-1)
#endif

// NOTE(bumbread): These are architecture-specific
#if !defined(O_RDONLY)
    #define O_RDONLY            0
    #define O_WRONLY            1
    #define O_RDWR              2
    #define O_CREAT          0x40
    #define O_EXCL           0x80
    #define O_NOCTTY        0x100
    #define O_TRUNC         0x200
    #define O_APPEND        0x400
    #define O_NONBLOCK      0x800
    #define O_DIRECTORY   0x10000
#endif

#define _SYSCALL_read           0
#define _SYSCALL_write          1
#define _SYSCALL_open           2
#define _SYSCALL_close          3
#define _SYSCALL_stat           4
#define _SYSCALL_fstat          5
#define _SYSCALL_lstat          6
#define _SYSCALL_poll           7
#define _SYSCALL_lseek          8
#define _SYSCALL_mmap           9
#define _SYSCALL_mprotect       10
#define _SYSCALL_munmap         11
#define _SYSCALL_brk            12
#define _SYSCALL_rt_sigaction   13
#define _SYSCALL_rt_sigprocmask 14
#define _SYSCALL_rt_sigreturn   15
#define _SYSCALL_ioctl          16
#define _SYSCALL_pread64        17
#define _SYSCALL_pwrite64       18
#define _SYSCALL_readv          19
#define _SYSCALL_writev         20
#define _SYSCALL_access         21
#define _SYSCALL_pipe           22
#define _SYSCALL_select         23
#define _SYSCALL_sched_yield    24
#define _SYSCALL_mremap         25
#define _SYSCALL_msync          26
#define _SYSCALL_mincore        27
#define _SYSCALL_madvise        28
#define _SYSCALL_shmget         29
#define _SYSCALL_shmat          30
#define _SYSCALL_shmctl         31
#define _SYSCALL_dup            32
#define _SYSCALL_dup2           33
#define _SYSCALL_pause          34
#define _SYSCALL_nanosleep      35
#define _SYSCALL_getitimer      36
#define _SYSCALL_alarm          37
#define _SYSCALL_setitimer      38
#define _SYSCALL_getpid         39
#define _SYSCALL_sendfile       40
#define _SYSCALL_socket         41
#define _SYSCALL_connect        42
#define _SYSCALL_accept         43
#define _SYSCALL_sendto         44
#define _SYSCALL_recvfrom       45
#define _SYSCALL_sendmsg        46
#define _SYSCALL_recvmsg        47
#define _SYSCALL_shutdown       48
#define _SYSCALL_bind           49
#define _SYSCALL_listen         50
#define _SYSCALL_getsockname    51
#define _SYSCALL_getpeername    52
#define _SYSCALL_socketpair     53
#define _SYSCALL_setsockopt     54
#define _SYSCALL_getsockopt     55
#define _SYSCALL_clone          56
#define _SYSCALL_fork           57
#define _SYSCALL_vfork          58
#define _SYSCALL_execve         59
#define _SYSCALL_exit           60

#define _SYSCALL_arch_prctl     158

// Syscall stubs

static inline i64 _syscall0(i64 n) {
    i64 ret;
    asm volatile("syscall" : "=a"(ret) : "a"(n) : "rcx", "r11", "memory");
    return ret;
}

static inline i64 _syscall1(i64 n, i64 a1) {
    i64 ret;
    asm volatile("syscall" : "=a"(ret) : "a"(n), "D"(a1) : "rcx", "r11", "memory");
    return ret;
}

static inline i64 _syscall2(i64 n, i64 a1, i64 a2) {
    i64 ret;
    asm volatile("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2)
                          : "rcx", "r11", "memory");
    return ret;
}

static inline i64 _syscall3(i64 n, i64 a1, i64 a2, i64 a3) {
    i64 ret;
    asm volatile("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
                          "d"(a3) : "rcx", "r11", "memory");
    return ret;
}

static inline i64 _syscall4(i64 n, i64 a1, i64 a2, i64 a3, i64 a4) {
    i64 ret;
    register i64 r10 asm("r10") = a4;
    asm volatile("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
                          "d"(a3), "r"(r10): "rcx", "r11", "memory");
    return ret;
}

static inline i64 _syscall5(i64 n, i64 a1, i64 a2, i64 a3, i64 a4, i64 a5) {
    i64 ret;
    register i64 r10 asm("r10") = a4;
    register i64 r8 asm("r8") = a5;
    asm volatile("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
                          "d"(a3), "r"(r10), "r"(r8) : "rcx", "r11", "memory");
    return ret;
}

static inline i64 _syscall6(i64 n, i64 a1, i64 a2, i64 a3, i64 a4, i64 a5, i64 a6) {
    i64 ret;
    register i64 r10 asm("r10") = a4;
    register i64 r8 asm("r8") = a5;
    register i64 r9 asm("r9") = a6;
    asm volatile("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
                          "d"(a3), "r"(r10), "r"(r8), "r"(r9) : "rcx", "r11", "memory");
    return ret;
}

// Syscall wrappers

static inline i64 _syscall_read(u32 fd, char *buf, u64 count) {
    return _syscall3(_SYSCALL_read, (i64)fd, (i64)buf, (i64)count);
}

static inline i64 _syscall_write(u32 fd, char const *buf, u64 count) {
    return _syscall3(_SYSCALL_write, (i64)fd, (i64)buf, (u64)count);
}

static inline i64 _syscall_open(char const *filename, int flags, int mode) {
    return _syscall3(_SYSCALL_open, (i64)filename, (i64)flags, (i64)mode);
}

static inline i64 _syscall_close(u32 fd) {
    return _syscall1(_SYSCALL_close, fd);
}

static inline void *_syscall_mmap(u64 addr, u64 len, u64 prot, u64 flags, u64 fd, u64 offset) {
    return (void *)_syscall6(_SYSCALL_mmap, addr, len, prot, flags, fd, offset);
}

static inline i64 _syscall_munmap(void *addr, u64 len) {
    return _syscall2(_SYSCALL_munmap, (u64)addr, len);
}

_Noreturn static inline void _syscall_exit(int code) {
    _syscall1(_SYSCALL_exit, (i64)code);
    __builtin_unreachable();
}

static inline i64 _syscall_arch_prctl_set(int code, u64 value) {
    return _syscall2(_SYSCALL_arch_prctl, code, (i64)value);
}

static inline i64 _syscall_arch_prctl_get(int code, u64 *value) {
    return _syscall2(_SYSCALL_arch_prctl, code, (i64)value);
}
