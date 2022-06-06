#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>

// TODO: rewrite memmove to not allocate anything

typedef unsigned char byte;

void *memcpy(void *restrict s1, const void *restrict s2, size_t n) {
    const byte *restrict c2 = s2;
    byte *restrict c1 = s1;

    while (n--) {
        *c1++ = *c2++;
    }
    return s1;
}

void *memmove(void *s1, const void *s2, size_t n)
{
    byte *u1 = s1;
    byte const *u2 = s2;
    void *buffer = malloc(n);
    strcpy(buffer, s2);
    strcpy(s1, buffer);
    free(buffer);
    return s1;
}

void *memset(void *s, int c, size_t n) {
    byte *restrict buf = s;
    while (n--) {
        *buf++ = c;
    }
    return s;
}

char *strcpy(char *restrict s1, const char *restrict s2)
{
    while(*s2 != 0) {
        *s1++ = *s2++;
    }
    *s1 = 0;
    return s1;
}

char *strncpy(char *restrict s1, const char *restrict s2, size_t n)
{
    while(n-->0 && *s2 != 0) {
        *s1++ = *s2++;
    }
    while(n-->0) {
        *s1++ = 0;
    }
    return s1;
}


char *strncat(char *restrict s1, const char *restrict s2, size_t n)
{
    size_t start = strlen(s1);
    for(size_t i = 0; i != n && *s2 != 0; ++i) {
        s1[start+i] = s2[i];
    }
    s1[start+n] = 0;
    return s1;
}


int memcmp(const void *s1, const void *s2, size_t n) {
    const byte *u1 = s1;
    const byte *u2 = s2;
    for (; n--; u1++, u2++) {
        if (*u1 != *u2) return *u1 - *u2;
    }
    return 0;
}

int strcmp(const char *s1, const char *s2) {
    int diff;
    do {
        diff = *s1 - *s2;
    } while(diff != 0 && *s1 != 0 && *s2 != 0);
    return diff;
}

int strncmp(const char *s1, const char *s2, size_t n)
{
    int diff;
    size_t i = 0;
    if(n != 0) do {
        diff = *s1 - *s2;
    } while(++i < n && diff != 0 && *s1 != 0 && *s2 != 0);
    return diff;
}

// fuck locales
int strcoll(const char *s1, const char *s2)
{
    return strcmp(s1, s2);
}

// fuck bad implementation of strcoll
size_t strxfrm(char *restrict s1, const char *restrict s2, size_t n)
{
    size_t len = strlen(s2);
    if(s1 != NULL && n != 0) {
        for(size_t i = 0; i != n; ++i) {
            *s1 = *s2;
        }
    }
    return len;
}

void *memchr(const void *ptr, int c, size_t n)
{
    const byte *s = ptr;
    size_t i = 0;
    for(; i != n && *s != c; ++i, ++s) {
        ++s;
    }
    if(i == n) return NULL;
    return (void *)s; // fuck clang
}

char *strchr(const char *s, int c)
{
    while(*s && *s != c) ++s;
    if(*s != c) return NULL;
    return (void *)s; // fuck clang
}

size_t strcspn(const char *s1, const char *s2)
{
    size_t i = 0;
    while(*s1) {
        if(strchr(s2, *s1) != NULL) {
            break;
        }
        ++s1;
        ++i;
    }
    return i;
}

char *strpbrk(const char *s1, const char *s2)
{
    while(*s1) {
        if(strchr(s2, *s1) == NULL) {
            break;
        }
        ++s1;
    }
    return (char *)s1;
}

char *strstr(const char *s1, const char *s2)
{
    if(!*s2) return (char *)s1;
    size_t len = strlen(s2);
    while(*s1 != 0) {
        bool match = true;
        for(size_t i = 0; i != len; ++i) {
            if(!*s1 || s1[i] != s2[i]) {
                match = false;
                break;
            }
        }
        ++s1;
        if(match) return (char *)s1;
    }
    return NULL;
}

char *strerror(int errnum)
{
    switch(errnum) {
        case 0:      return "No errors";
        case EDOM:   return "Value is out of domain of the function";
        case EILSEQ: return "Illegal byte sequence";
        case ERANGE: return "Value is out of range";
    }
    return "Unkown error";
}

size_t strlen(const char *s) {
    size_t i = 0;
    while (s[i]) {
        i++;
    }
    return i;
}

#if __STDC_WANT_LIB_EXT1__ == 1

size_t strnlen_s(const char *s, size_t maxsize) {
    if (s == NULL) return 0;

    size_t i = 0;
    while (s[i] && i < maxsize) {
        i++;
    }
    return i;
}

#endif