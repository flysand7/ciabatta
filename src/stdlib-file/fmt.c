
static int print_string(char *str) {
    int str_len = 0;
    while(str[str_len] != 0) {
        str_len += 1;
    }
    u64 unused;
    _RT_Status status = _rt_file_write(&stdout->rt_file, str_len, str, &unused);
    if(status != _RT_STATUS_OK) {
        return -1;
    };
    return str_len;
}
static int print_int(i64 number) {
    bool negative = (number < 0);
    if(negative) {
        number = -number;
    }
    char buf[20];
    buf[sizeof buf - 1] = 0;
    char *p = buf + sizeof buf - 1;
    do {
        *--p = (number%10) + '0';
        number /= 10;
    } while(number > 0);
    if(negative) {
        *--p = '-';
    }
    u64 unused;
    int buf_len = (int)(&buf[0] + sizeof buf - p);
    _RT_Status status = _rt_file_write(&stdout->rt_file, buf_len, p, &unused);
    if(status != _RT_STATUS_OK) {
        return -1;
    }
    return buf_len;
}

static int print_uint(u64 number) {
    char buf[20];
    buf[sizeof buf - 1] = 0;
    char *p = buf + sizeof buf - 1;
    do {
        *--p = (number%10) + '0';
        number /= 10;
    } while(number > 0);
    u64 unused;
    int buf_len = (int)(&buf[0] + sizeof buf - p);
    _RT_Status status = _rt_file_write(&stdout->rt_file, buf_len, p, &unused);
    if(status != _RT_STATUS_OK) {
        return -1;
    }
    return buf_len;
}
static int print_hex(u64 number) {
    char digits[] = "0123456789abcdef";
    char buf[20];
    buf[sizeof buf - 1] = 0;
    char *p = buf + sizeof buf - 1;
    for(int i = 0; i < 64; i += 4) {
        // if(i != 0 && i % 16 == 0) {
        //     *--p = '_';
        // }
        u8 bits = (number >> i) & 0x0f;
        char digit = digits[bits];
        *--p = digit;
    }
    *--p = 'x';
    *--p = '0';
    u64 unused;
    int buf_len = (int)(&buf[0] + sizeof buf - p);
    _RT_Status status = _rt_file_write(&stdout->rt_file, buf_len, p, &unused);
    if(status != _RT_STATUS_OK) {
        return -1;
    }
    return buf_len;
}

int printf(const char *restrict fmt, ...) {
    i64 ret_printed = 0;
    va_list args;
    va_start(args, fmt);
    i64 buf_start_idx = 0;
    i64 buf_end_idx = 0;
    u64 unused;
    for(i64 i = 0; fmt[i] != 0;) {
        buf_start_idx = i;
        buf_end_idx = i;
        while(fmt[i] != '%' && fmt[i] != 0) {
            buf_end_idx += 1;
            i += 1;
        }
        i64 buf_size = buf_end_idx - buf_start_idx;
        if(buf_size != 0) {
            _RT_Status status = _rt_file_write(
                &stdout->rt_file
                , buf_size
                , (void *)&fmt[buf_start_idx]
                , &unused
            );
            if(status != _RT_STATUS_OK) {
                return -1;
            }
            ret_printed += buf_size;
        }
        if(fmt[i] == 0) {
            break;
        }
        // fmt[i] == '%'
        i += 1;
        int arg_chars;
        if(fmt[i] == 'd') {
            int arg = va_arg(args, int);
            arg_chars = print_int((i64)arg);
            
        }
        else if(fmt[i] == 'u') {
            unsigned int arg = va_arg(args, unsigned int);
            arg_chars = print_uint((u64)arg);
        }
        else if(fmt[i] == 'p') {
            void *arg = va_arg(args, void *);
            arg_chars = print_hex((u64)arg);
        }
        else if(fmt[i] == 's') {
            char *str = va_arg(args, char *);
            arg_chars = print_string(str);
        }
        else if(fmt[i] == 'c') {
            int ch = va_arg(args, int);
            _RT_Status status = _rt_file_write(&stdout->rt_file, 1, &ch, &unused);
            if(status != _RT_STATUS_OK) {
                return -1;
            }
            arg_chars = 1;
        }
        i += 1;
        if(arg_chars < 0) {
            return -1;
        }
        ret_printed += arg_chars;
    }
    va_end(args);
    return ret_printed;
}
