
#include <syscall.h>

static char stack_chk_fail_msg[] =
    "Stack check failed. "
    "You've got a stack corruption somewhere. "
    "Sorry these guys didn't tell me where\n";

void __stack_chk_fail(void) {
    sys_write(STDERR_FILENO, stack_chk_fail_msg, sizeof stack_chk_fail_msg);
    sys_exit(1);
}

extern int main(int argc, char **argv, char **envp);
static void _fileapi_init();

void _start(_LD_CRT_Params *params) {
    cia_stack_size = params->stack_size;
    cia_tls_image_size = params->tls_image_size;
    cia_tls_image_base = params->tls_image_base;
    // Get the envp
    // char **envp = argv + (argc + 1);
    // init(argc, argv, envp);
    _fileapi_init();
    int code = main(0, NULL, NULL);
    // fini();
    // glibc bug
    // dl_fini();
    sys_exit(code);
}
