
void *memcpy(void *restrict s1, const void *restrict s2, size_t n) {
    const u8 *restrict c2 = s2;
    u8 *restrict c1 = s1;

    while (n--) {
        *c1++ = *c2++;
    }
    return s1;
}

void *memmove(void *s1, const void *s2, size_t n) {
    u8* c1 = s1;
    const u8* c2 = s2;
    if (c1 != c2) {
        if (c1 < c2) {
            // reverse copy
            for (size_t i = n; i--;) c1[i] = c2[i];
        } else {
            // normal copy
            for (size_t i = 0; i < n; i++) c1[i] = c2[i];
        }
    }

    return s1;
}

char *strcpy(char *restrict s1, const char *restrict s2) {
    while(*s2 != 0) {
        *s1++ = *s2++;
    }
    *s1 = 0;
    return s1;
}

char *strncpy(char *restrict s1, const char *restrict s2, size_t n) {
    size_t i = 0;
    for(; i < n && *s2 != 0; ++ i) {
        *s1++ = *s2++;
    }
    for(; i < n; ++i) {
        *s1++ = 0;
    }
    return s1;
}


char *strcat(char *restrict s1, const char *restrict s2) {
    size_t start = strlen(s1);
    return strcpy(s1+start, s2);
}

char *strncat(char *restrict s1, const char *restrict s2, size_t n) {
    size_t start = strlen(s1);
    strncpy(s1+start, s2, n);
    return s1;
}


int memcmp(const void *s1, const void *s2, size_t n) {
    const u8 *u1 = s1;
    const u8 *u2 = s2;
    for (; n--; u1++, u2++) {
        if (*u1 != *u2) return *u1 - *u2;
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

int strcmp(const char *s1, const char *s2) {
    int diff;
    do {
        diff = *s1 - *s2;
    } while(diff == 0 && *s1++ != 0 && *s2++ != 0);
    return diff;
}

int strncmp(const char *s1, const char *s2, size_t n) {
    int diff = 0;
    size_t i = 0;
    if(n != 0) do {
        diff = *s1 - *s2;
    } while(++i < n && diff != 0 && *s1 != 0 && *s2 != 0);
    return diff;
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
