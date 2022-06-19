
#include <_os.h>

#define WIN32_LEAN_AND_MEAN
#include<Windows.h>
#include<DbgHelp.h>

#include <signal.h>
#include <stddef.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

void _os_print_stack_trace() {
    HANDLE process = GetCurrentProcess();
    SymInitialize(process, NULL, TRUE);

    void *stack[128];
    USHORT frames = CaptureStackBackTrace(2, 128, stack, NULL);

    SYMBOL_INFO* symbol = calloc(sizeof(SYMBOL_INFO)+256, 1);
    symbol->MaxNameLen   = 255;
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    for(size_t i = 0; i < frames; i++) {
        SymFromAddr(process, (DWORD64)stack[i], 0, symbol);
        if(strcmp(symbol->Name, "BaseThreadInitThunk") == 0) break;
        if(strcmp(symbol->Name, "mainCRTStartup") == 0)      break;
        printf("  %u: 0x%"PRIx64" (%s)\n",
            (int)(frames-i-1),
            symbol->Address,
            symbol->Name
        );
    }
    free(symbol);
}

void _os_init_eh() {
    void *res = AddVectoredExceptionHandler(1, &_win32_handler);
    if(res == NULL) {
        ExitProcess(-69420);
    }
}
