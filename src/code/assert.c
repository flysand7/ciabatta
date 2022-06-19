
#include <assert.h>

#include <stdio.h>  // printf
#include <stdlib.h> // abort
#include <_os.h>    // stack trace

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
    _os_print_stack_trace();
    abort();
}
