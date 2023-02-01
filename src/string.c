
void *memcpy(void *restrict to, void const *restrict from, size_t n) {
    u8       *restrict dst = to;
    u8 const *restrict src = from;
    while (n--) {
        *dst++ = *src++;
    }
    return dst;
}

void *memmove(void *to, const void *from, size_t n) {
    u8       *dst = to;
    u8 const *src = from;
    if (src != dst) {
        if (src < dst) {
            // reverse copy
            for (size_t i = n; i--;) dst[i] = src[i];
        } else {
            // normal copy
            for (size_t i = 0; i < n; i++) dst[i] = src[i];
        }
    }
    return dst;
}

char *strcpy(char *restrict dst, char const *restrict src) {
    size_t i = 0;
    for(i = 0; src[i]; ++i) {
        dst[i] = src[i];
    }
    dst[i] = 0;
    return dst;
}

char *strncpy(char *restrict dst, char const *restrict src, size_t n) {
    size_t i;
    for(i = 0; i != n && src[i]; ++i) {
        dst[i] = src[i];
    }
    for(; i != n; ++i) {
        dst[i] = 0;
    }
    return dst;
}


char *strcat(char *restrict dst, const char *restrict src) {
    size_t start = strlen(dst);
    return strcpy(dst+start, src);
}

char *strncat(char *restrict dst, char const *restrict src, size_t n) {
    size_t start = strlen(dst);
    strncpy(dst+start, src, n);
    return dst;
}


int memcmp(void const *p1, void const *p2, size_t n) {
    u8 const *s1 = p1;
    u8 const *s2 = p2;
    size_t i;
    for(i = 0; i != n; ++i) {
        if(s1[i] != s2[i]) break;
    }
    if(i != n) {
        if(s1[i] < s2[i]) return -1;
        if(s1[i] > s2[i]) return 1;
    }
    return 0;
}

void *memset(void *s, int c, size_t n) {
    u8 *restrict buf = s;
    while (n--) {
        *buf++ = c;
    }
    return s;
}

int strcmp(char const *s1, char const *s2) {
    size_t i;
    for(i = 0; s1[i] && s2[i]; ++i) {
        if(s1[i] != s2[i]) break;
    }
    if(s1[i] < s2[i]) return -1;
    if(s1[i] > s2[i]) return +1;
    return 0;
}

int strncmp(const char *s1, const char *s2, size_t n) {
    size_t i;
    for(i = 0; i != n && s1[i] && s2[i]; ++i) {
        if(s1[i] != s2[i]) break;
    }
    if(s1[i] < s2[i]) return -1;
    if(s1[i] > s2[i]) return +1;
    return 0;
}

int strcoll(const char *s1, const char *s2) {
    return strcmp(s1, s2);
}

// TODO: I don't quite understand the intent nor use behind this function
// so I'm just going to ignore locales for now.
size_t strxfrm(char *restrict s1, const char *restrict s2, size_t n) {
    size_t len = strlen(s2);
    if(s1 != NULL && n != 0) {
        for(size_t i = 0; i != n; ++i) {
            *s1 = *s2;
        }
    }
    return len;
}

void *memchr(const void *ptr, int c, size_t n) {
    const char *s = ptr;
    for(size_t i = 0; i != n; ++i) {
        if(s[i] == c) {
            // Casting away const because clang warnings
            return (void *)(s+i);
        }
    }
    return NULL;
}

char *strchr(const char *s, int c) {
    do {
        if(*s == c) return (char *)s;
    } while(*s++);
    return NULL;
}

size_t strspn(const char *s1, const char *s2) {
    size_t i = 0;
    for(; *s1; ++s1) {
        if(strchr(s2, *s1) == NULL) {
            break;
        }
        ++i;
    }
    return i;
}

size_t strcspn(const char *s1, const char *s2) {
    size_t i = 0;
    for(; *s1; ++s1) {
        if(strchr(s2, *s1) != NULL) {
            break;
        }
        ++i;
    }
    return i;
}

char *strpbrk(const char *s1, const char *s2) {
    while(*s1) {
        if(strchr(s2, *s1) == NULL) {
            break;
        }
        ++s1;
    }
    return (char *)s1;
}

char *strrchr(const char *s, int c) {
    char const *last = NULL;
    for(; *s != 0; ++s) {
        if(*s == c) last = s;
    }
    return (char *)last;
}

char *strstr(const char *s1, const char *s2) {
    if(*s2 == 0) return (char *)s1;
    size_t len = strlen(s2);
    for(; *s1 != 0; ++s1) {
        if(strncmp(s1, s2, len) == 0) return (char *)s1;
    }
    return NULL;
}

// TODO: there may be restrict-related UB
char *strtok(char *restrict s1, const char *restrict s2) {
    static char *str;
    return strtok_r(s1, s2, &str);
}

char *strtok_r(char *restrict s1, const char *restrict s2, char **restrict ctx) {
    if(s1 != NULL) *ctx = s1;
    if(*ctx == NULL) return NULL;

    size_t junk_len = strspn(*ctx, s2);
    char *tok_start = *ctx + junk_len;
    if(*tok_start == 0) {
        *ctx = NULL;
        return NULL;
    }

    size_t tok_len = strcspn(*ctx, s2);

    char *tok_end = tok_start + tok_len;
    *tok_end = 0;
    *ctx = tok_end+1;

    return tok_start;
}

char *strerror(int errnum) {
    switch(errnum) {
        case 0:      return "No errors";
        case EDOM:   return "Value is outside of domain of the function";
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

size_t strnlen_s(const char *s, size_t maxsize) {
    if (s == NULL) return 0;
    size_t i = 0;
    while (s[i] && i < maxsize) {
        i++;
    }
    return i;
}

char *strdup(const char *str1) {
    if(str1 == NULL) return NULL;
    size_t len = strlen(str1);
    char *copy = calloc(len+1, 1);
    strcpy(copy, str1);
    return copy;
}
