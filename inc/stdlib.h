#pragma once

#if defined(_MSC_VER) && !defined(__clang__)
    #define _Noreturn __declspec(noreturn)
#endif

#if !defined(__STDC_LIB_EXT1__)
    #define __STDC_LIB_EXT1__
#endif

#ifdef __STDC_WANT_SECURE_LIB__
    #if !defined(__STDC_WANT_LIB_EXT1__)
        #define __STDC_WANT_LIB_EXT1__ 1
    #endif
#endif

#if !defined(NULL)
    #define NULL ((void *)0)
#endif

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#if defined(_WIN32)
    typedef unsigned long long size_t;
    typedef unsigned short wchar_t;
#else
    typedef unsigned long size_t;
    typedef int wchar_t;
#endif

typedef struct div_t {
    int quot;
    int rem;
} div_t;

typedef struct ldiv_t {
    long quot;
    long rem;
} ldiv_t;

typedef struct lldiv_t {
    long long quot;
    long long rem;
} lldiv_t;

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

#define RAND_MAX 65536

#define MB_CUR_MAX 5

// Microsoft extension, COUNTOF(x) counts array elements
#ifndef COUNTOF
#define COUNTOF(...) (sizeof(__VA_ARGS__) / sizeof((__VA_ARGS__)[0]))
#endif

// Some number stuff
int       abs  (int i);
long      labs (long i);
long long llabs(long long i);
div_t     div  (int numer,       int denom);
ldiv_t    ldiv (long numer,      long denom);
lldiv_t   lldiv(long long numer, long long denom);

// String conversion routines
double             atof    (const char *nptr);
int                atoi    (const char *nptr);
long               atol    (const char *nptr);
long long          atoll   (const char *nptr);
double             strtod  (const char *restrict nptr, char **restrict endptr);
float              strtof  (const char *restrict nptr, char **restrict endptr);
long double        strtold (const char *restrict nptr, char **restrict endptr);
long               strtol  (const char *restrict nptr, char **restrict endptr, int base);
long long          strtoll (const char *restrict nptr, char **restrict endptr, int base);
unsigned long      strtoul (const char *restrict nptr, char **restrict endptr, int base);
unsigned long long strtoull(const char *restrict nptr, char **restrict endptr, int base);

char *itoa(int value, char *str, int base);

// PRNG
int  rand (void);
void srand(unsigned int seed);

// Memory management functions
void *malloc       (size_t size);
void *calloc       (size_t nmemb, size_t size);
void *aligned_alloc(size_t align, size_t size);
void *realloc      (void *ptr,    size_t size);
void  free         (void *ptr);

// Communication with environment
_Noreturn void  abort        (void);
_Noreturn void  quick_exit   (int status);
_Noreturn void  _Exit        (int status);
_Noreturn void  exit         (int status);
int             atexit       (void (*func)(void));
int             at_quick_exit(void (*func)(void));
char           *getenv       (const char *name);
int             system       (const char *string);

const void *bsearch(
    const void *key,
    const void *base,
    size_t nmemb,
    size_t size,
    int (*compar)(const void *, const void *)
);

void qsort(
    void *base,
    size_t nmemb,
    size_t size,
    int (*compar)(const void *, const void *)
);

// Multibyte strings
int    mblen   (const char *s, size_t n);
int    mbtowc  (wchar_t *restrict pwc,  const char *restrict s, size_t n);
size_t mbstowcs(wchar_t *restrict pwcs, const char *restrict s, size_t n);
int    wctomb  (char *s, wchar_t wchar);
size_t wcstombs(char *restrict s, const wchar_t *restrict pwcs, size_t n);

// #ifdef __STDC_WANT_LIB_EXT1__
// 	typedef void (*constraint_handler_t)(const char * restrict msg, void * restrict ptr, errno_t error);

// 	constraint_handler_t set_constraint_handler_s(constraint_handler_t handler);
// 	void abort_handler_s(const char * restrict msg, void * restrict ptr, errno_t error);
// 	void ignore_handler_s(const char * restrict msg, void * restrict ptr, errno_t error);
// 	errno_t getenv_s(size_t * restrict len, char * restrict value, rsize_t maxsize, const char * restrict name);
// #endif
