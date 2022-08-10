
size_t wcslen(const wchar_t *s) {
    size_t len = 0;
    while(s[len++]);
    return len;
}

wchar_t *wmemset(wchar_t *str, wchar_t c, size_t n) {
    while(n--) {
        str[n] = c;
    }
    return str;
}

wchar_t *wmemmove(wchar_t *dst, wchar_t const *src, size_t n) {
    if(dst != src) {
        if(dst < src) {
            for (size_t i = n; i--;) dst[i] = src[i];
        } else {
            for (size_t i = 0; i < n; i++) dst[i] = src[i];
        }
    }
    return dst;
}

wchar_t *wmemcpy(wchar_t *restrict dst, wchar_t const *restrict src, size_t n) {
    while(n--) dst[n] = src[n];
    return (wchar_t *)dst;
}

wchar_t *wcscpy(wchar_t *restrict dst, wchar_t const *restrict src) {
    size_t i;
    for(i = 0; src[i]; ++i) {
        dst[i] = src[i];
    }
    dst[i] = 0;
    return (wchar_t *)dst;
}

wchar_t *wcsncpy(wchar_t *restrict dst, wchar_t const *restrict src, size_t n) {
    // TODO: more than one null terminator, not write if n was reached
    size_t i;
    for(i = 0; i != n && src[i]; ++i) {
        dst[i] = src[i];
    }
    dst[i] = 0;
    return (wchar_t *)dst;
}

wchar_t *wcscat(wchar_t *restrict dst, wchar_t const *restrict src) {
    size_t dst_len = wcslen(dst);
    size_t i;
    for(i = 0; src[i]; ++i) {
        dst[dst_len + i] = src[i];
    }
    dst[dst_len + i] = 0;
    return (wchar_t *)dst;
}

wchar_t *wcsncat(wchar_t *restrict dst, wchar_t const *restrict src, size_t n) {
    size_t dst_len = wcslen(dst);
    size_t i;
    for(i = 0; i != n && src[i]; ++i) {
        dst[dst_len + i] = src[i];
    }
    dst[dst_len + i] = 0;
    return (wchar_t *)dst;
}

int wcscmp(wchar_t const *s1, wchar_t const *s2) {
    size_t i;
    for(i = 0; s1[i] && s2[i]; ++i) {
        if(s1[i] != s2[i]) break;
    }
    if(s1[i] < s2[i]) return -1;
    if(s1[i] > s2[i]) return 1;
    return 0;
}

int wcsncmp(wchar_t const *s1, wchar_t const *s2, size_t n) {
    size_t i;
    for(i = 0; i != n && s1[i] && s2[i]; ++i) {
        if(s1[i] != s2[i]) break;
    }
    if(s1[i] < s2[i]) return -1;
    if(s1[i] > s2[i]) return 1;
    return 0;
}

int wmemcmp(wchar_t const *s1, wchar_t const *s2, size_t n) {
    // TODO: OOB when ranges are equal
    size_t i;
    for(i = 0; i != n; ++i) {
        if(s1[i] != s2[i]) break;
    }
    if(s1[i] < s2[i]) return -1;
    if(s1[i] > s2[i]) return 1;
    return 0;
}
