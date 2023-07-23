

int __stack_chk_fail() {
    // TODO: implement proper stack protector support
    return 0;
}

void __libc_start_main(
    int (*main)(int, char**, char**),
    int argc, char **argv,
    int (*init)(int, char**, char**),
    void (*fini)(void),
    void (*dl_fini)(void),
    void *stack_end
) {
    // Get the envp
    char **envp = argv + (argc + 1);
    init(argc, argv, envp);
    main(argc, argv, envp);
    fini();
    // glibc bug
    dl_fini();
    syscall_exit(0);
}
