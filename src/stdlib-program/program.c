
#define MAX_ATEXIT_HANDLERS 32
#define MAX_AT_QUICK_EXIT_HANDLERS 32

static void (*atexit_handlers[MAX_ATEXIT_HANDLERS])(void);
static void (*at_quick_exit_handlers[MAX_AT_QUICK_EXIT_HANDLERS])(void);
static u64 n_atexit_handlers = 0;
static u64 n_at_quick_exit_handlers = 0;

int atexit(void (*func)(void)) {
    if(n_atexit_handlers == MAX_ATEXIT_HANDLERS) {
        return MAX_ATEXIT_HANDLERS;
    }
    atexit_handlers[n_atexit_handlers] = func;
    n_atexit_handlers += 1;
    return 0;
}

int at_quick_exit(void (*func)(void)) {
    if(n_at_quick_exit_handlers == MAX_AT_QUICK_EXIT_HANDLERS) {
        return MAX_AT_QUICK_EXIT_HANDLERS;
    }
    at_quick_exit_handlers[n_at_quick_exit_handlers] = func;
    n_at_quick_exit_handlers += 1;
    return 0;
}

_Noreturn void abort(void) {
    // TODO: Ideally do a debug trap if the process is being debugged
    _rt_program_exit(1);
    __builtin_unreachable();
}

_Noreturn void exit(int code) {
    for(i64 i = n_atexit_handlers-1; i-- > 0; ) {
        void (*handler)(void) = atexit_handlers[i];
        handler();
    }
    // TODO(bumbread): flush all the unflushed file streams
    // TODO(bumbread): close all file streams and delete temporary files
    _rt_program_exit(code);
    __builtin_unreachable();
}

_Noreturn void _Exit(int code) {
    _rt_program_exit(code);
    __builtin_unreachable();
}

_Noreturn void quick_exit(int code) {
    for(i64 i = n_at_quick_exit_handlers-1; i-- > 0; ) {
        void (*handler)(void) = at_quick_exit_handlers[i];
        handler();
    }
    _rt_program_exit(code);
    __builtin_unreachable();
}

