
#include <stdlib.h>
#include <signal.h>

#include <_os.h>

typedef void (*ExitRoutine)(void);

// The implementation shall support the registration
// of at least 32 functions.
static ExitRoutine _exit_routines[64];
static int _exit_routine_count;

_Noreturn void abort(void) {
    raise(SIGABRT);
    _os_exit(-69);
}

int atexit(void (*func)(void)) {
    if (_exit_routine_count >= COUNTOF(_exit_routines)) {
        return 0;
    }

    _exit_routines[_exit_routine_count++] = func;
    return 1;
}

_Noreturn void exit(int status) {
    // doing them in reverse seems nicer
    for (int i = _exit_routine_count; i--;) {
        _exit_routines[i]();
    }

    _os_exit(status);
}

_Noreturn void _Exit(int status) {
    // doesn't run atexit routines
    _os_exit(status);
}
