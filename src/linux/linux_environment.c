
#include <fcntl.h>
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

static char **get_command_args(int *argcp);

// TODO: Instead of using static arrays, allocate this memory dynamically
static char cmdline[4096];
static char *cmdargs[1024];
extern int main(int argc, char** argv);


void _start() {
    srand(0);
    setlocale(LC_ALL, "C");

    int argc;
    char **argv = get_command_args(&argc);
    int exit_code = main(argc, argv);

    exit(exit_code);
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

static char **get_command_args(int *argcp) {
    int fd = open("/proc/self/cmdline", O_RDONLY);
    ssize_t size = read(fd, cmdline, sizeof cmdline);
    ssize_t i = 0;
    ssize_t cmd_idx = 0;
    while(i != size) {
        cmdargs[cmd_idx] = &cmdline[i];
        while(cmdline[i] != 0) {
            ++i;
        }
        ++i;
    }
    return cmdargs;
}
