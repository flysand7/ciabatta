#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

#include <_os.h>

#define __STDC_WANT_LIB_EXT1__ 1
#include <string.h>

// Instantiate generic printf function for byte strings
// TODO: instantiate wide char variants of print
#define suffix(name) name ## _char
#define fchar char
#define fstrlen(s, maxsize) strnlen_s(s, maxsize)
#include "fmt_print.h"

typedef struct {
    size_t used, capacity;
    char* string;
} StrPrintCtx;

static void string_write(void *ctx, char ch) {
    StrPrintCtx *c = ctx;
    c->string[c->used++] = ch;
}

int fprintf(FILE *file, const char *restrict fmt, ...) {
	CALL_PRINTF(fmt_print_char, file, _os_file_write_char, fmt);
	return result;
}

int printf(const char *restrict fmt, ...) {
    CALL_PRINTF(fmt_print_char, stdout, _os_file_write_char, fmt);
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
	return fmt_print_char(file, _os_file_write_char, fmt, args);
}

int vprintf(const char *restrict fmt, va_list args) {
	return fmt_print_char(stdout, _os_file_write_char, fmt, args);
}

int vsnprintf(char *restrict s, size_t n, const char *restrict fmt, va_list args) {
    StrPrintCtx ctx = { 0, n, s };
    return fmt_print_char(&ctx, string_write, fmt, args);
}

int vsprintf(char *restrict s, const char *restrict fmt, va_list args) {
    StrPrintCtx ctx = { 0, SIZE_MAX, s };
	return fmt_print_char(&ctx, string_write, fmt, args);
}
