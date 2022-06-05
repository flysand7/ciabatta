#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <errno.h>

int main(int argc, char** argv) {
    const char *p = "10 200000000000000000000000000000 30 -40 junk";
    // printf("Parsing '%s':\n", p);
 
    for (;;)
    {
        // errno can be set to any non-zero value by a library function call
        // regardless of whether there was an error, so it needs to be cleared
        // in order to check the error set by strtol
        errno = 0;
        char *end;
        const long i = strtol(p, &end, 10);
        if (p == end)
            break;
 
        const bool range_error = errno == ERANGE;
        // printf("Extracted '%.*s', strtol returned %ld.", (int)(end-p), p, i);
        p = end;
 
        // if (range_error)
        //     printf(" Range error occurred.");
 
        // putchar('\n');
    }
 
    // printf("Unextracted leftover: '%s'\n\n", p);
 
    // parsing without error handling
    long v1 = strtol("1010", NULL, 2);
    long v2 = strtol("12",   NULL, 8);
    long v3 = strtol("A",    NULL, 16);
    long v4 = strtol("junk", NULL, 36);
    long v5 = strtol("012",  NULL, 0);
    long v6 = strtol("0xA",  NULL, 0);
    long v7 = strtol("junk", NULL, 0);
    return 0;
}
