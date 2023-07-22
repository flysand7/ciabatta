
void __libc_start_main(
    int (*main)(int, char**, char**),
    int argc, char **argv,
    int (*init)(int, char**, char**),
    void (*fini)(void),
    void (*rtld_fini)(void),
    void *stack_end
) {
    syscall_exit(0);
}
