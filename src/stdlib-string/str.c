
char *strcpy(char *restrict dst, char const *restrict src) {
    i64 str_len = 0;
    for(i64 i = 0; src[i] != 0; ++i) {
        dst[i] = src[i];
        str_len += 1;
    }
    dst[str_len] = 0;
    return dst;
}

char *strncpy(char *restrict dst, char const *restrict src, size_t count) {
    i64 str_len = 0;
    for(i64 i = 0; i < count; ++i) {
        if(src[i] == 0) {
            break;
        }
        dst[i] = src[i];
        str_len += 1;
    }
    for(i64 i = str_len; i < count; ++i) {
        dst[i] = 0;
    }
    return dst;
}

