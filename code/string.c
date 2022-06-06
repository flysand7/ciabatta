#include <string.h>

void *memcpy(void *restrict s1, const void *restrict s2, size_t n) {
    const unsigned char *restrict c2 = s2;
    unsigned char *restrict c1 = s1;

    while (n--) {
        *c1++ = *c2++;
    }
    return s1;
}

int memcmp(const void *s1, const void *s2, size_t n) {
    const unsigned char *u1 = s1;
    const unsigned char *u2 = s2;

    for (; n--; u1++, u2++) {
        if (*u1 != *u2) return *u1 - *u2;
    }

    return 0;
}

void *memset(void *s, int c, size_t n) {
    unsigned char *restrict buf = s;
    while (n--) {
        *buf++ = c;
    }
    return s;
}

int strcmp(const char *s1, const char *s2) {
    while (*s1 != 0 && *s2 != 0) {
        if (*s1 != *s2) break;
    }

    if(*s1 > *s2) return 1;
    if(*s1 < *s2) return -1;
    return 0;
}

size_t strlen(const char *s) {
    size_t i = 0;
    while (s[i]) {
        i++;
    }
    return i;
}

// __STDC_WANT_LIB_EXT1__
size_t strnlen_s(const char *s, size_t maxsize) {
    if (s == NULL) return 0;

    size_t i = 0;
    while (s[i] && i < maxsize) {
        i++;
    }
    return i;
}
