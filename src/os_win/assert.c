
#include <assert.h>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "DbgHelp.lib")

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


void _assert(
    char const *cond,
    char const *func,
    char const *file,
    int line
) {
    if(GetConsoleWindow() == NULL) {
        // For GUI application we display the info into a messagebox
        char buf[1024];
        int i = 0;
        i += snprintf(buf+i, sizeof buf-i, "Assertion failed: %s\n", cond);
        i += snprintf(buf+i, sizeof buf-i, "  Function: %s\n", func);
        i += snprintf(buf+i, sizeof buf-i, "  File: %s\n", file);
        i += snprintf(buf+i, sizeof buf-i, "  Line: %d\n", line);
display_msg:
        int reaction = MessageBoxA(NULL, buf, "Assertion Failed", 0x00000032L);
        switch(reaction) {
            case IDABORT:    abort();
            case IDRETRY:    goto display_msg;
            case IDCONTINUE: return;
        }
    }
    else {
        // For console application we print the info to console
        printf("Assertion failed: %s\n", cond);
        printf("  Function: %s\n", func);
        printf("  File: %s\n", file);
        printf("  Line: %d\n", line);
        printf("Trace:\n");
        _print_stack_trace();
        abort();
    }
}
