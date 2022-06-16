
#include <_os.h>

#define WIN32_LEAN_AND_MEAN
#include<Windows.h>

#include <signal.h>
#include <stddef.h>

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

void _os_init_eh() {
    void *res = AddVectoredExceptionHandler(1, &_win32_handler);
    if(res == NULL) {
        ExitProcess(-69420);
    }
}
