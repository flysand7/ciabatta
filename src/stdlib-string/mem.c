
void *memcpy(void *restrict dst, void const *restrict src, size_t count) {
    u8 *restrict dst_bytes = dst;
    u8 const *restrict src_bytes = src;
    for(size_t i = 0; i < count; ++i) {
        dst_bytes[i] = src_bytes[i];
    }
    return dst_bytes;
}

void *memccpy(void *restrict dst, void const *restrict src, int c, size_t count) {
    u8 *restrict dst_bytes = dst;
    u8 const *restrict src_bytes = src;
    for(size_t i = 0; i < count; ++i) {
        dst_bytes[i] = src_bytes[i];
        if(src_bytes[i] == c) {
            return (void *)&src[i+1];
        }
    }
    return NULL;
}

void *memmove(void *dst, const void *src, size_t count) {
    if(dst == src) {
        return dst;
    }
    u8 *dst_bytes = dst;
    u8 const *src_bytes = src;
    if(dst < src) {
        for(size_t i = 0; i < count; ++i) {
            dst_bytes[i] = src_bytes[i];
        }
    }
    else {
        for(size_t i = count-1; i-- != 0; ) {
            dst_bytes[i] = src_bytes[i];
        }
    }
    return dst_bytes;
}
