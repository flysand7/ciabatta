
#pragma once

#include <locale.h>
#include <stdarg.h>

#if !defined(WEOF)
    #define WEOF -1
#endif

#if !defined(NULL)
    #define NULL ((void *)0)
#endif

#if defined(_WIN32)
    #if !defined(WCHAR_MIN)
        #define WCHAR_MIN 0x0000
    #endif
    #if !defined(WCHAR_MAX)
        #define WCHAR_MAX 0xffff
    #endif
    typedef unsigned short wchar_t;
#else
    #if !defined(WCHAR_MIN)
        #define WCHAR_MIN -0x80000000
    #endif
    #if !defined(WCHAR_MAX)
        #define WCHAR_MAX +0x7fffffff
    #endif
    typedef int wchar_t;
#endif

typedef int wint_t;
typedef int wint_t;
typedef int (*wctrans_t)(wint_t wc);
typedef int (*wctype_t)(wint_t wc);

#if !defined(_mbstate_t_defined)
    #define _mbstate_t_defined
    typedef struct mbstate_t mbstate_t;
    struct mbstate_t {
        union {
            unsigned short leftover;
            unsigned short high_surrogate;
        };
    };
#endif

#if !defined(_tm_defined)
    #define _tm_defined
    struct tm {
        int tm_sec;
        int tm_min;
        int tm_hour;
        int tm_mon;
        int tm_mday;
        int tm_year;
        int tm_wday;
        int tm_yday;
        int tm_isdst;
    };
#endif

typedef struct FILE FILE;

// String length
size_t wcslen(const wchar_t *s);

// Copying
wchar_t *wmemset (wchar_t *s,           wchar_t c,         size_t n);
wchar_t *wmemmove(wchar_t *s1,          wchar_t const *s2, size_t n);
wchar_t *wmemcpy (wchar_t *restrict s1, wchar_t const *restrict s2, size_t n);
wchar_t *wcsncpy (wchar_t *restrict s1, wchar_t const *restrict s2, size_t n);
wchar_t *wcscpy  (wchar_t *restrict s1, wchar_t const *restrict s2);

// Concat
wchar_t *wcscat (wchar_t *restrict s1, wchar_t const *restrict s2);
wchar_t *wcsncat(wchar_t *restrict s1, wchar_t const *restrict s2, size_t n);

// Comparison
int wcscmp (wchar_t const *s1, wchar_t const *s2);
int wcsncmp(wchar_t const *s1, wchar_t const *s2, size_t n);
int wmemcmp(wchar_t const *s1, wchar_t const *s2, size_t n);

// Localization
int wcscoll    (wchar_t const *s1,    wchar_t const *s2);
size_t wcsxfrm (wchar_t *restrict s1, wchar_t const *restrict s2, size_t n);

// Tokenization and stuff
wchar_t *wmemchr(wchar_t const *s,  wchar_t c, size_t n);
wchar_t *wcschr (wchar_t const *s,  wchar_t c);
wchar_t *wcsrchr(wchar_t const *s,  wchar_t c);
wchar_t *wcspbrk(wchar_t const *s1, wchar_t const *s2);
size_t   wcscspn(wchar_t const *s1, wchar_t const *s2);
size_t   wcsspn (wchar_t const *s1, wchar_t const *s2);
wchar_t *wcsstr (wchar_t const *s1, wchar_t const *s2);
wchar_t *wcstok (wchar_t *restrict s1, wchar_t const *restrict s2, wchar_t **restrict ptr);

// Formatted print
int vfwprintf(FILE *restrict stream, const wchar_t *restrict format, va_list arg);
int fwprintf (FILE *restrict stream, const wchar_t *restrict format, ...);
int vswprintf(wchar_t *restrict s, size_t n, const wchar_t *restrict format, va_list arg);
int swprintf (wchar_t *restrict s, size_t n, const wchar_t *restrict format, ...);
int vwprintf (wchar_t const *restrict format, va_list arg);
int wprintf  (wchar_t const *restrict format, ...);

// Formatted scan
int vfwscanf(FILE *restrict stream, const wchar_t *restrict format, va_list arg);
int fwscanf (FILE *restrict stream, const wchar_t *restrict format, ...);
int vswscanf(const wchar_t *restrict s, const wchar_t *restrict format, va_list arg);
int swscanf (const wchar_t *restrict s, const wchar_t *restrict format, ...);
int vwscanf (const wchar_t *restrict format, va_list arg);
int wscanf  (const wchar_t *restrict format, ...);

// File reading
wint_t   fgetwc(FILE *stream);
wchar_t *fgetws(wchar_t *restrict s, int n, FILE *restrict stream);
wint_t   getwc(FILE *stream);
wint_t   getwchar(void);
wint_t   ungetwc(wint_t c, FILE *stream);
wint_t   fputwc(wchar_t c, FILE *stream);

// File writing
int      fputws(const wchar_t *restrict s, FILE *restrict stream);
wint_t   putwc(wchar_t c, FILE *stream);
wint_t   putwchar(wchar_t c);

// Switch stream mode to wide
int      fwide(FILE *stream, int mode);

// Float conversions
double                 wcstod  (wchar_t const *restrict nptr, wchar_t **restrict endptr);
float                  wcstof  (wchar_t const *restrict nptr, wchar_t **restrict endptr);

// Integer conversions
long double            wcstold (wchar_t const *restrict nptr, wchar_t **restrict endptr);
long int               wcstol  (wchar_t const *restrict nptr, wchar_t **restrict endptr, int base);
long long int          wcstoll (wchar_t const *restrict nptr, wchar_t **restrict endptr, int base);
unsigned long int      wcstoul (wchar_t const *restrict nptr, wchar_t **restrict endptr, int base);
unsigned long long int wcstoull(wchar_t const *restrict nptr, wchar_t **restrict endptr, int base);

// Character conversions
wint_t btowc(int c);
int    wctob(wint_t c);

// Ftime conversion
size_t wcsftime(wchar_t *restrict s, size_t maxsize, const wchar_t *restrict format, const struct tm *restrict timeptr);

// Multibyte conversions
int    mbsinit  (mbstate_t const *ps);
size_t mbrlen   (char const *restrict s, size_t n, mbstate_t *restrict ps);
size_t mbrtowc  (wchar_t *restrict pwc, char const *restrict s, size_t n, mbstate_t *restrict ps);
size_t mbsrtowcs(wchar_t *restrict dst, char const **restrict src, size_t len, mbstate_t *restrict ps);
size_t wcrtomb  (char *restrict s, wchar_t wc, mbstate_t *restrict ps);
size_t wcsrtombs(char *restrict dst, wchar_t const **restrict src, size_t len, mbstate_t *restrict ps);

#ifdef __STDC_WANT_LIB_EXT1__

int vfwprintf_s (FILE *restrict stream, const wchar_t *restrict format, va_list arg);
int fwprintf_s  (FILE *restrict stream, const wchar_t *restrict format, ...);
int vsnwprintf_s(wchar_t *restrict s, rsize_t n, const wchar_t *restrict format, va_list arg);
int vswprintf_s (wchar_t *restrict s, rsize_t n, const wchar_t *restrict format, va_list arg);
int snwprintf_s (wchar_t *restrict s, rsize_t n, const wchar_t *restrict format, ...);
int swprintf_s  (wchar_t *restrict s, rsize_t n, const wchar_t *restrict format, ...);
int vwprintf_s  (wchar_t const *restrict format, va_list arg);
int wprintf_s   (wchar_t const *restrict format, ...);

int vfwscanf_s(FILE *restrict stream, const wchar_t *restrict format, va_list arg);
int fwscanf_s (FILE *restrict stream, const wchar_t *restrict format, ...);
int vswscanf_s(const wchar_t *restrict s, const wchar_t *restrict format, va_list arg);
int swscanf_s (const wchar_t *restrict s, const wchar_t *restrict format, ...);
int vwscanf_s (const wchar_t *restrict format, va_list arg);
int wscanf_s  (const wchar_t *restrict format, ...);

errno_t wcscpy_s  (wchar_t *restrict s1, rsize_t s1max, const wchar_t *restrict s2);
errno_t wcsncpy_s (wchar_t *restrict s1, rsize_t s1max, const wchar_t *restrict s2, rsize_t n);
errno_t wmemcpy_s (wchar_t *restrict s1, rsize_t s1max, const wchar_t *restrict s2, rsize_t n);
errno_t wmemmove_s(wchar_t *s1, rsize_t s1max, const wchar_t *s2, rsize_t n);
errno_t wcscat_s  (wchar_t *restrict s1, rsize_t s1max, const wchar_t *restrict s2);
errno_t wcsncat_s (wchar_t *restrict s1, rsize_t s1max, const wchar_t *restrict s2, rsize_t n);
wchar_t *wcstok_s (wchar_t *restrict s1, rsize_t *restrict s1max, const wchar_t *restrict s2, wchar_t **restrict ptr);
size_t wcsnlen_s  (const wchar_t *s, size_t maxsize);
errno_t wcrtomb_s (size_t *restrict retval, char *restrict s, rsize_t smax, wchar_t wc, mbstate_t *restrict ps);
errno_t mbsrtowcs_s(size_t *restrict retval, wchar_t *restrict dst, rsize_t dstmax, const char **restrict src, rsize_t len, mbstate_t *restrict ps);
errno_t wcsrtombs_s(size_t *restrict retval, char *restrict dst, rsize_t dstmax, const wchar_t **restrict src, rsize_t len, mbstate_t *restrict ps);

#endif
