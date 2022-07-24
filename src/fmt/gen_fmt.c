
#define ctype     char
#define pfx(f) f
#include "gen_fmt.h"
#undef  ctype
#undef  pfx

#define ctype     wchar_t
#define pfx(f) w ## f
#include "gen_fmt.h"
#undef  ctype
#undef  pfx

static int fprintfcb(void *ctx, char ch) {
    FILE *f = ctx;
    return fputc(ch, f) != EOF;
}

int vfprintf(FILE *stream, const char *fmt, va_list args) {
    return vprintfcb(stream, fprintfcb, fmt, args);
}

int vprintf(const char *fmt, va_list args) {
    return vprintfcb(stdout, fprintfcb, fmt, args);
}

int fprintf(FILE *stream, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int len = vfprintf(stream, fmt, args);
    va_end(args);
    return len;
}

int printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int len = vprintf(fmt, args);
    va_end(args);
    return len;
}
