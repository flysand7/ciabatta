#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

#include <_os.h>

#define __STDC_WANT_LIB_EXT1__ 1
#include <string.h>

#include <_os.h>

typedef void(*OutputFunc)(void* ctx, size_t n, const char str[]);

///////////////////////////////////////////////
// Instantiate formatted print functions
///////////////////////////////////////////////
// TODO: instantiate wide char variants of print
#define FMT_FUNC_NAME fmt_print_char
#define FMT_CHAR_TYPE char
#define FMT_STRLEN_S(s, maxsize) strnlen_s(s, maxsize)
#include "printf.h"

typedef struct {
    size_t used, capacity;
    char* string;
} StrPrintCtx;

FILE *stdout, *stderr, *stdin;

#define CALL_PRINTF(fmt_func, ctx, out, fmt)       \
va_list args;                                  \
va_start(args, fmt);                           \
int result = fmt_func(ctx, out, fmt, args);    \
va_end(args)

static void string_write(void *ctx, size_t n, const char *restrict str) {
    StrPrintCtx *c = ctx;
    memcpy(c->string + c->used, str, n);
    c->used += n;
}

int fprintf(FILE *file, const char *restrict fmt, ...) {
	CALL_PRINTF(fmt_print_char, file, _os_file_write, fmt);
	return result;
}

int printf(const char *restrict fmt, ...) {
    CALL_PRINTF(fmt_print_char, stdout, _os_file_write, fmt);
	return result;
}

int snprintf(char *restrict s, size_t n, const char *restrict fmt, ...) {
    StrPrintCtx ctx = { 0, n, s };
    CALL_PRINTF(fmt_print_char, &ctx, string_write, fmt);
	return result;
}

int sprintf(char *restrict s, const char *restrict fmt, ...) {
    StrPrintCtx ctx = { 0, SIZE_MAX, s };
    CALL_PRINTF(fmt_print_char, &ctx, string_write, fmt);
	return result;
}

int vfprintf(FILE *file, const char *restrict fmt, va_list args) {
	return fmt_print_char(file, _os_file_write, fmt, args);
}

int vprintf(const char *restrict fmt, va_list args) {
	return fmt_print_char(stdout, _os_file_write, fmt, args);
}

int vsnprintf(char *restrict s, size_t n, const char *restrict fmt, va_list args) {
    StrPrintCtx ctx = { 0, n, s };
    return fmt_print_char(&ctx, string_write, fmt, args);
}

int vsprintf(char *restrict s, const char *restrict fmt, va_list args) {
    StrPrintCtx ctx = { 0, SIZE_MAX, s };
	return fmt_print_char(&ctx, string_write, fmt, args);
}
