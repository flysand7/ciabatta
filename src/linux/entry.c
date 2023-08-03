
#include <syscall.h>

static char stack_chk_fail_msg[] =
    "Stack check failed. "
    "You've got a stack corruption somewhere. "
    "Sorry these guys didn't tell me where\n";

void __stack_chk_fail(void) {
    sys_write(STDERR_FILENO, stack_chk_fail_msg, sizeof stack_chk_fail_msg);
    sys_exit(1);
}

static void _fileapi_init();

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
    _fileapi_init();
    main(argc, argv, envp);
    fini();
    // glibc bug
    // dl_fini();
    sys_exit(0);
}
