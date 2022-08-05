
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

struct str_ctx_t typedef str_ctx_t;
struct str_ctx_t {
    char *str;
    size_t n;
    size_t i;
};

static int sprintfcb(void *ctx, char ch) {
    str_ctx_t *stream = ctx;
    stream->str[stream->i++] = ch;
    return 1;
}

static int snprintfcb(void *ctx, char ch) {
    str_ctx_t *stream = ctx;
    if(stream->i >= stream->n) {
        return 0;
    }
    stream->str[stream->i++] = ch;
    return 1;
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


int vsprintf(char *buf, char const *fmt, va_list args) {
    str_ctx_t ctx = {0};
    ctx.str = buf;
    return vprintfcb(&ctx, sprintfcb, fmt, args);
}

int vsnprintf(char *buf, size_t sz, char const *fmt, va_list args) {
    str_ctx_t ctx = {0};
    ctx.str = buf;
    ctx.n = sz;
    return vprintfcb(&ctx, snprintfcb, fmt, args);
}

int sprintf(char *buf, char const *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int len = vsprintf(buf, fmt, args);
    va_end(args);
    return len;
}

int snprintf(char *buf, size_t sz, char const *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(buf, sz, fmt, args);
    va_end(args);
    return len;
}
