
#include <stdlib.h>
#include <signal.h>

#include <_os.h>

_Noreturn void abort(void) {
    raise(SIGABRT);
    _os_exit(-69);
}

// TODO: at_exit handling

_Noreturn void exit(int status) {
    _os_exit(status);
}

_Noreturn void _Exit(int status) {
    _os_exit(status);
}

