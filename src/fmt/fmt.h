
static inline int pfx(fmt_arg)(fmt_stream_t *stream, ctype const **strp, va_list va) {
    ctype const *str = *strp; 
    fmt_t fmt = {0};
    if(!pfx(fmt_read)(str, &fmt, va)) {
        return 0;
    }
    switch(*str) {
        case 'i':
        case 'u':
        case 'b':
        case 'o':
        case 'd':
        case 'x':
        case 'X':
        {
            int base;
            int sign;
            u64 abs = pfx(fmt_int_arg)(stream, &fmt, &str, &sign, &base, va);
            if(pfx(fmt_int)(stream, fmt, sign, abs, base) == -1) {
                return -1;
            }
        } break;
    }
    *strp = str;
    return 1;
}

static inline int pfx(fmt)(fmt_stream_t *stream, ctype const *str, va_list va) {
    stream->w = 0;
    while(*str) {
        if(*str == '%') {
            ++str;
            int ok = pfx(fmt_arg)(stream, &str, va);
            if(!ok) {
                return -1;
            }
        }
        else {
            stream_out(stream, *str++);
        }
    }
    return stream->w;
}
