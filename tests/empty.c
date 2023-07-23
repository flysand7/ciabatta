
#include <cia_definitions.h>

#define STDOUT_FILENO 1
#define SYS_write 1

static __inline i64 __syscall3(i64 n, i64 a1, i64 a2, i64 a3) {
    i64 ret;
    __asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
                          "d"(a3) : "rcx", "r11", "memory");
    return ret;
}

static inline i64 syscall_write(u32 fd, char const *buf, u64 count) {
    return __syscall3(SYS_write, (i64)fd, (i64)buf, (u64)count);
}

int main(int argc, char **argv, char **envp) {
    char string[] = "Hello, world!\n";
    syscall_write(STDOUT_FILENO, string, sizeof string);
    return 0;
}
