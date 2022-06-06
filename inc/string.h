
#pragma once

#if !defined(NULL)
    #define NULL ((void *)0)
#endif

// int _wcsicmp(const wchar_t *string1, const wchar_t *string2);

void *memcpy(void * restrict s1, const void * restrict s2, size_t n);
void *memmove(void *s1, const void *s2, size_t n);
char *strcpy(char * restrict s1, const char * restrict s2);
char *strncpy(char * restrict s1, const char * restrict s2, size_t n);
char *strcat(char * restrict s1, const char * restrict s2);
char *strncat(char * restrict s1, const char * restrict s2, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
int strcmp(const char *s1, const char *s2);
int strcoll(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
size_t strxfrm(char * restrict s1, const char * restrict s2, size_t n);
void *memchr(const void *s, int c, size_t n);
char *strchr(const char *s, int c);
size_t strcspn(const char *s1, const char *s2);
char *strpbrk(const char *s1, const char *s2);
char *strrchr(const char *s, int c);
size_t strspn(const char *s1, const char *s2);
char *strstr(const char *s1, const char *s2);
char *strtok(char * restrict s1, const char * restrict s2);
void *memset(void *s, int c, size_t n);
char *strerror(int errnum);
size_t strlen(const char *s);

#if __STDC_WANT_LIB_EXT1__ == 1
int memcpy_s(void * restrict s1, size_t s1max, const void * restrict s2, size_t n);
int memmove_s(void *s1, size_t s1max, const void *s2, size_t n);
int strcpy_s(char * restrict s1, size_t s1max, const char * restrict s2);
int strncpy_s(char * restrict s1, size_t s1max,const char * restrict s2, size_t n);
int strcat_s(char * restrict s1, size_t s1max, const char * restrict s2);
int strncat_s(char * restrict s1, size_t s1max, const char * restrict s2, size_t n);
char *strtok_s(char * restrict s1, size_t * restrict s1max, const char * restrict s2, char ** restrict ptr);
int memset_s(void *s, size_t smax, int c, size_t n);
int strerror_s(char *s, size_t maxsize, int errnum);
size_t strerrorlen_s(int errnum);
size_t strnlen_s(const char *s, size_t maxsize);
#endif
