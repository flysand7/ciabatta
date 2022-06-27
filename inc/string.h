
#pragma once

#include <stddef.h>

#if !defined(NULL)
    #define NULL ((void *)0)
#endif

#if !defined(__STDC_LIB_EXT1__)
    #define __STDC_LIB_EXT1__
    typedef int errno_t;
    typedef size_t rsize_t;
#endif

#if __STDC_WANT_SECURE_LIB__ == 1
    #if !defined(__STDC_WANT_LIB_EXT1__)
        #define __STDC_WANT_LIB_EXT1__ 1
    #endif
#endif

#if !defined(__STDC_ALLOC_LIB__)
    #define __STDC_ALLOC_LIB__
#endif

size_t  strlen(const char *s);
char   *strerror(int errnum);

// Copying
void  *memset  (void *s,            int c,                   size_t n);
void  *memmove (void *s1,           const void *s2,          size_t n);
void  *memcpy  (void *restrict s1,  const void *restrict s2, size_t n);
char  *strncpy (char *restrict s1,  const char *restrict s2, size_t n);
char  *strcpy  (char *restrict s1,  const char *restrict s2);

// Concat
char  *strncat (char *restrict s1,  const char *restrict s2, size_t n);
char  *strcat  (char *restrict s1,  const char *restrict s2);

// Comparison
int    memcmp  (const void *s1,     const void *s2, size_t n);
int    strncmp (const char *s1,     const char *s2, size_t n);
int    strcmp  (const char *s1,     const char *s2);

// Collation/Localisation thingy
int    strcoll (const char *s1,     const char *s2);
size_t strxfrm (char *restrict s1,  const char *restrict s2, size_t n);

// Tokenization and related stuff
void   *memchr (const void *str,    int c, size_t n);
char   *strchr (const char *str,    int c);
size_t  strcspn(const char *str,    const char *s);
char   *strpbrk(const char *str,    const char *s);
char   *strrchr(const char *str,    int c);
size_t  strspn (const char *str,    const char *s);
char   *strstr (const char *str,    const char *s);
char   *strtok (char *restrict str, const char *restrict s);

// Linux extension: reentrant strtok
char   *strtok_r(char *restrict s1, const char *restrict s2, char **restrict ctx);

// Extension 1
#if __STDC_WANT_LIB_EXT1__ == 1

size_t strnlen_s(const char *str, size_t strsz);
    // Safe memory copies
    errno_t memset_s (void *s,           rsize_t smax,  int c,                   rsize_t n);
    errno_t memmove_s(void *s1,          rsize_t s1max, const void *s2,          rsize_t n);
    errno_t memcpy_s (void *restrict s1, rsize_t s1max, const void *restrict s2, rsize_t n);
    errno_t strncpy_s(char *restrict s1, rsize_t s1max, const char *restrict s2, rsize_t n);
    errno_t strcpy_s (char *restrict s1, rsize_t s1max, const char *restrict s2);

    // Safe concat et strtok
    errno_t  strcat_s (char *restrict s1, rsize_t s1max, const char *restrict s2);
    errno_t  strncat_s(char *restrict s1, rsize_t s1max, const char *restrict s2, rsize_t n);
    char    *strtok_s(char *restrict s1, rsize_t *restrict s1max, const char *restrict s2, char **restrict ptr);

    // Safe error strings
    errno_t  strerror_s(char *s, rsize_t maxsize, errno_t errnum);
    size_t   strerrorlen_s(errno_t errnum);
#endif

// Extension 2
#if __STDC_WANT_LIB_EXT2__ == 1
    char *strdup(const char *str1);
#endif
