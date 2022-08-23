
#define FLAG_ZERO   0x0001 // "0"
#define FLAG_LEFT   0x0002 // "-"
#define FLAG_PLUS   0x0004 // "+"
#define FLAG_SPACE  0x0008 // " "
#define FLAG_HASH   0x0010 // "#"
#define FLAG_PREC   0x0020 // ".precision"
#define FLAG_LONG   0x0040 // "l"
#define FLAG_LLONG  0x0080 // "ll"
#define FLAG_SHORT  0x0100 // "h"
#define FLAG_CHAR   0x0200 // "hh"
#define FLAG_UPPER  0x0400 // for hex digits 0xa-0xf

struct fmt_t typedef fmt_t;
struct fmt_t {
    int prec;
    int width;
    int flags;
};

#define ctype  char
#define pfx(f) f
#include "fmt_string.h"
#include "fmt_stream.h"
#include "ints.h"
#include "fmt.h"
#undef  ctype
#undef  pfx

#define ctype  wchar_t
#define pfx(f) w ## f
#include "fmt_string.h"
#include "fmt_stream.h"
#include "ints.h"
#include "fmt.h"
#undef  ctype
#undef  pfx

// fprintf family

static inline int fstream_outc(void *ctx, char c) {
    FILE *file = ctx;
    int result = fputc(c, file);
    if(result == EOF) {
        return -1;
    }
    return 1;
}

static inline int fstream_outw(void *ctx, wchar c) {
    FILE *file = ctx;
    int result = fputc((char)c, file); // TODO: utf8
    if(result == EOF) {
        return -1;
    }
    return 1;
}

static inline fmt_stream_t fstream_new(FILE *file) {
    fmt_stream_t stream;
    stream.w = 0;
    stream.ctx = file;
    stream.out_ctype = fstream_outc;
    stream.out_wchar = fstream_outw;
    stream.out_char = fstream_outc;
    return stream;
}

int vfprintf(FILE *restrict file, const char *restrict string, va_list va) {
    fmt_stream_t stream = fstream_new(file);
    return fmt(&stream, string, va);
}

int vprintf(const char *restrict string, va_list va) {
    return vfprintf(stdout, string, va);
}

int fprintf(FILE *restrict file, const char *restrict string, ...) {
    va_list va;
    va_start(va, string);
    int result = vfprintf(file, string, va);
    va_end(va);
    return result;
}

int printf(const char *restrict string, ...) {
    va_list va;
    va_start(va, string);
    int result = vfprintf(stdout, string, va);
    va_end(va);
    return result;
}

// s(n)printf family

struct str_slice_t typedef str_slice_t;
struct str_slice_t {
    char *str;
    size_t max_size;
    size_t written;
};

static inline int sstream_outc(void *ctx, char c) {
    str_slice_t *slice = ctx;
    if(slice->written+1 < slice->max_size) {
        return -1;
    }
    slice->str[slice->written++] = c;
    return 1;
}

static inline int sstream_outw(void *ctx, wchar c) {
    str_slice_t *slice = ctx;
    // TODO: utf8
    if(slice->written+1 < slice->max_size) {
        return -1;
    }
    slice->str[slice->written++] = c;
    return 1;
}

static inline fmt_stream_t sstream_new(str_slice_t *slice) {
    fmt_stream_t stream;
    stream.w = 0;
    stream.ctx = slice;
    stream.out_ctype = sstream_outc;
    stream.out_wchar = sstream_outw;
    stream.out_char  = sstream_outc;
    return stream;
}

int vsnprintf(char *restrict buf, size_t cbbuf, const char *restrict str, va_list va) {
    str_slice_t slice;
    slice.str = buf;
    slice.max_size = cbbuf;
    slice.written = 0;
    fmt_stream_t stream = sstream_new(&slice);
    return fmt(&stream, str, va);
}

int vsprintf(char *restrict buf, const char *restrict str, va_list va) {
    str_slice_t slice;
    slice.str = buf;
    slice.max_size = SIZE_MAX;
    slice.written = 0;
    fmt_stream_t stream = sstream_new(&slice);
    return fmt(&stream, str, va);
}

int snprintf(char *restrict buf, size_t cbbuf, const char *restrict str, ...) {
    va_list va;
    va_start(va, str);
    int result = vsnprintf(buf, cbbuf, str, va);
    va_end(va);
    return result;
}

int sprintf(char *restrict buf, const char *restrict str, ...) {
    va_list va;
    va_start(va, str);
    int result = vsprintf(buf, str, va);
    va_end(va);
    return result;
}
