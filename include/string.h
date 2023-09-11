
#pragma once

#include <cia/def.h>

void *memcpy(void *restrict dst, void const *restrict src, size_t count);
void *memccpy(void *restrict dst, void const *restrict src, int c, size_t count);
void *memmove(void *dst, const void *src, size_t count)

char *strcpy(char *restrict dst, char const *restrict src);
char *strncpy(char *restrict dst, char const *restrict src, size_t count);

void memset(void *dst, int ch, size_t length);
int _wcsicmp(uint16_t *s1, uint16_t *s2);
