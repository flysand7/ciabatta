
#include <assert.h>

static void _print_stack_trace() {
    HANDLE process = GetCurrentProcess();
    SymInitialize(process, NULL, TRUE);

    void *stack[128];
    USHORT frames = CaptureStackBackTrace(2, 128, stack, NULL);

    SYMBOL_INFO* symbol = calloc(sizeof(SYMBOL_INFO)+256, 1);
    symbol->MaxNameLen   = 255;
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    for(size_t i = 0; i < frames; i++) {
        SymFromAddr(process, (DWORD64)stack[i], 0, symbol);
        // if(strcmp(symbol->Name, "BaseThreadInitThunk") == 0) break;
        // if(strcmp(symbol->Name, "mainCRTStartup") == 0)      break;
        printf(//"  %u: 0x%"PRIx64" (%s)\n",
            "  %d: %s\n",
            (int)(frames-i-1),
            //symbol->Address,
            symbol->Name
        );
    }
    free(symbol);
}


void _Noreturn _assert(
    char const *cond,
    char const *func,
    char const *file,
    int line
) {
    printf("Assertion failed: %s\n", cond);
    printf("  Function: %s\n", func);
    printf("  File: %s\n", file);
    printf("  Line: %d\n", line);
    printf("Trace:\n");
    _print_stack_trace();
    abort();
}
