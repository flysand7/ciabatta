
#define _countof(arr) (sizeof (arr) / sizeof ((arr)[0]))

typedef struct SignalMapping {
    DWORD code;
    int signal;
} SignalMapping;

static SignalMapping map[] = {
    {EXCEPTION_ACCESS_VIOLATION,      SIGSEGV},
    {EXCEPTION_IN_PAGE_ERROR,         SIGSEGV},
    {EXCEPTION_ARRAY_BOUNDS_EXCEEDED, SIGSEGV},
    {EXCEPTION_DATATYPE_MISALIGNMENT, SIGALIGN},
    {EXCEPTION_BREAKPOINT,            SIGBREAK},
    {EXCEPTION_FLT_DENORMAL_OPERAND,  SIGFPE},
    {EXCEPTION_FLT_DIVIDE_BY_ZERO,    SIGFPE},
    {EXCEPTION_FLT_INEXACT_RESULT,    SIGFPE},
    {EXCEPTION_FLT_INVALID_OPERATION, SIGFPE},
    {EXCEPTION_FLT_OVERFLOW,          SIGFPE},
    {EXCEPTION_FLT_STACK_CHECK,       SIGFPE},
    {EXCEPTION_FLT_UNDERFLOW,         SIGFPE},
    {EXCEPTION_ILLEGAL_INSTRUCTION,   SIGILL},
    {EXCEPTION_INT_DIVIDE_BY_ZERO,    SIGFPE},
    {EXCEPTION_INT_OVERFLOW,          SIGFPE},
    {EXCEPTION_PRIV_INSTRUCTION,      SIGILL},
    {EXCEPTION_SINGLE_STEP,           SIGSTEP},
};

static LONG _win32_handler(EXCEPTION_POINTERS *ExceptionInfo) {
    EXCEPTION_RECORD *exception = ExceptionInfo->ExceptionRecord;
    DWORD code = exception->ExceptionCode;
    int signal = -1;
    for(int mapping = 0; mapping != _countof(map); ++mapping) {
        if(code == map[mapping].code) {
            signal = map[mapping].signal;
        }
    }
    if(signal != -1) {
        raise(signal);
    }
    return EXCEPTION_CONTINUE_SEARCH;
}


static void _setup_eh() {
    void *res = AddVectoredExceptionHandler(1, &_win32_handler);
    if(res == NULL) {
        ExitProcess(-69420);
    }
}

void _signal_default_handler(int sig){}
void _signal_ignore_handler(int sig){}

static void (*(handlers[]))(int) = {
    [SIGINT]   = _signal_ignore_handler,
    [SIGILL]   = _signal_ignore_handler,
    [SIGFPE]   = _signal_ignore_handler,
    [SIGSEGV]  = _signal_ignore_handler,
    [SIGTERM]  = _signal_ignore_handler,
    [SIGABRT]  = _signal_ignore_handler,
    [SIGBREAK] = _signal_ignore_handler,
    [SIGALIGN] = _signal_ignore_handler,
    [SIGSTEP]  = _signal_ignore_handler,
};

void (*signal(int sig, void (*func)(int)))(int) {
    if(_SIG_MIN <= sig && sig <= _SIG_MAX) {
        handlers[sig] = func;
        return func;
    }
    return SIG_ERR;
}

int raise(int sig)
{
    if(_SIG_MIN <= sig && sig <= _SIG_MAX) {
        handlers[sig](sig);
        if(sig == SIGFPE || sig == SIGILL || sig == SIGSEGV) {
            ExitProcess(-69420);
        }
        return 1;
    }
    return 0;
}
