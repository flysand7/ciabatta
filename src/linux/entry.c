

int __stack_chk_fial() {
    // TODO: implement proper stack protector support
    return 0;
}

void __libc_start_main(
    int (*main)(int, char**, char**),
    int argc, char **argv,
    int (*init)(int, char**, char**),
    void (*fini)(void),
    void (*rtld_fini)(void),
    void *stack_end
) {
    static char string[] = "Hello, world!\n";
    syscall_write(STDOUT_FILENO, string, sizeof string);
    syscall_exit(0);
}
