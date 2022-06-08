
#pragma once

// Technically all we should have here is typedef for size_t.
// but I can't get that without macro shittery so
// for now I'm just doing this, which is not quite correct
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

// int _wcsicmp(const wchar_t *string1, const wchar_t *string2);

void *memcpy(void *restrict s1, const void *restrict s2, size_t n);
void *memmove(void *s1, const void *s2, size_t n);
char *strcpy(char *restrict s1, const char *restrict s2);
char *strncpy(char *restrict s1, const char *restrict s2, size_t n);
char *strcat(char *restrict s1, const char *restrict s2);
char *strncat(char *restrict s1, const char *restrict s2, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
int strcmp(const char *s1, const char *s2);
int strcoll(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
size_t strxfrm(char *restrict s1, const char *restrict s2, size_t n);
void *memchr(const void *s, int c, size_t n);
char *strchr(const char *s, int c);
size_t strcspn(const char *s1, const char *s2);
char *strpbrk(const char *s1, const char *s2);
char *strrchr(const char *s, int c);
size_t strspn(const char *s1, const char *s2);
char *strstr(const char *s1, const char *s2);
char *strtok(char *restrict s1, const char *restrict s2);
void *memset(void *s, int c, size_t n);
char *strerror(int errnum);
size_t strlen(const char *s);

#if __STDC_WANT_LIB_EXT1__ == 1
    errno_t memcpy_s(void *restrict s1, rsize_t s1max, const void *restrict s2, rsize_t n);
    errno_t memmove_s(void *s1, rsize_t s1max, const void *s2, rsize_t n);
    errno_t strcpy_s(char *restrict s1, rsize_t s1max, const char *restrict s2);
    errno_t strncpy_s(char *restrict s1, rsize_t s1max,const char *restrict s2, rsize_t n);
    errno_t strcat_s(char *restrict s1, rsize_t s1max, const char *restrict s2);
    errno_t strncat_s(char *restrict s1, rsize_t s1max, const char *restrict s2, rsize_t n);
    char *strtok_s(char *restrict s1, rsize_t *restrict s1max, const char *restrict s2, char **restrict ptr);
    errno_t memset_s(void *s, rsize_t smax, int c, rsize_t n);
    errno_t strerror_s(char *s, rsize_t maxsize, errno_t errnum);
    size_t strerrorlen_s(errno_t errnum);
    size_t strnlen_s(const char *str, size_t strsz);
#endif
