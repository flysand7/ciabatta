
#include <unistd.h>
#include <stddef.h>

#include <stdlib.h>
#include <signal.h>
#include <locale.h>

// Exit routines
#define ATEXIT_FUNC_COUNT  64
#define ATQEXIT_FUNC_COUNT 64
static void (*atexit_funcs [ATEXIT_FUNC_COUNT])(void);
static void (*atqexit_funcs[ATQEXIT_FUNC_COUNT])(void);
static int atexit_func_count;
static int atqexit_func_count;

extern int main(int argc, char** argv);

void _start() {
    
    srand(0);
    setlocale(LC_ALL, "C");

    int argc = 0;
    char *argv[1] = {NULL};
    int code = main(argc, argv);
    _exit(code);
}

_Noreturn void quick_exit(int status) {
    while(atqexit_func_count--) {
        atqexit_funcs[atqexit_func_count]();
    }
    _exit(status);
}

_Noreturn void exit(int status) {
    while(atexit_func_count--) {
        atexit_funcs[atqexit_func_count]();
    }
    // _close_io();
    _exit(status);
}

_Noreturn void _Exit(int status) {
    _exit(status);
}

_Noreturn void abort(void) {
    // raise(SIGABRT);
    _exit(-69);
}

int atexit(void (*func)(void)) {
    if (atexit_func_count >= ATEXIT_FUNC_COUNT) {
        return 0;
    }
    atexit_funcs[atexit_func_count++] = func;
    return 1;
}

int at_quick_exit(void (*func)(void)) {
    if(atqexit_func_count >= ATQEXIT_FUNC_COUNT) {
        return 0;
    }
    atqexit_funcs[atqexit_func_count++] = func;
    return 1;
}
