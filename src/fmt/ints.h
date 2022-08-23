
static inline u64 pfx(fmt_int_arg)(
    fmt_stream_t *stream,
    fmt_t *fmt,
    ctype const **strp,
    int *signp,
    int *basep,
    va_list va
) {
    ctype const *str = *strp;
    // Get integer properties
    int base;
    int is_signed;
    switch(*str) {
        case 'o': base = 8; break;
        case 'b': base = 2; break;
        case 'x':
        case 'X': base = 16; break;
        default:  base = 10;
    }
    switch(*str) {
        case 'd': is_signed = 1; break;
        case 'i': is_signed = 1; break;
        default:  is_signed = 0;
    }
    if(*str == 'X') {
        fmt->flags |= FLAG_UPPER;
    }
    // Discard some flags
    if(base == 10) {
        fmt->flags &= ~FLAG_HASH;
    }
    if(!is_signed) {
        fmt->flags &= ~(FLAG_PLUS|FLAG_SPACE);
    }
    if(fmt->flags & FLAG_PREC) {
        fmt->flags &= ~FLAG_ZERO;
    }
    // Parse sign and abs value of an integer
    int sign = 0;
    u64 abs_value = 0;
    if(is_signed) {
        i64 num;
        if(fmt->flags & FLAG_LLONG) {
            num = (i64)va_arg(va, intll);
        }
        else if(fmt->flags & FLAG_LONG) {
            num = (i64)va_arg(va, intl);
        }
        else {
            num = (i64)va_arg(va, int);
        }
        // Handle overflow on '-'
        if(num == LLONG_MIN) {
            sign = 1;
            abs_value = (u64)LLONG_MAX + 1;
        }
        else {
            sign = num >= 0?   0 :    1;
            num = num >= 0? num : -num;
            abs_value = (u64)num;
        }
    }
    else {
        if(fmt->flags & FLAG_LLONG) {
            abs_value = (u64)va_arg(va, intull);
        }
        else if(fmt->flags & FLAG_LONG) {
            abs_value = (u64)va_arg(va, intul);
        }
        else {
            abs_value = (u64)va_arg(va, intu);
        }
    }
    ++str;
    *strp = str;
    *basep = base;
    *signp = sign;
    return abs_value;
}

static inline int pfx(fmt_int)(
    fmt_stream_t *stream,
    fmt_t fmt,
    int neg,
    u64 value,
    int base) {
    // We'll Write digits into buf in reverse and then call _out_rbuf
    int ndigits = 0;
    ctype digits[64] = {0};
    // Remove hash flag for 0 values
    if(value == 0) fmt.flags &= ~FLAG_HASH;
    // Write digits to buffer in reverse (starting from least significant)
    do {
        int d = (int)(value%base);
        value /= base;
        if(d < 10)
             d += '0';
        else if(fmt.flags & FLAG_UPPER)
             d += 'A' - 10;
        else d += 'a' - 10;
        digits[ndigits++] = d;
    } while(value);
    // Figure out the length of the prefix (part of number before digit stirng)
    int pref_len = 0;
    if(fmt.flags & FLAG_HASH)       pref_len = base == 8? 1 : 2;
    else if(neg)                pref_len = 1;
    else if(fmt.flags & FLAG_PLUS)  pref_len = 1;
    else if(fmt.flags & FLAG_SPACE) pref_len = 1;
    if(ndigits > fmt.prec) fmt.prec = ndigits;
    int num_len = pref_len + fmt.prec;
    int pad_len = fmt.width - num_len;
    if(!(fmt.flags & FLAG_LEFT)) {
        ctype pad_ch = ' ';
        if(fmt.flags & FLAG_ZERO) pad_ch = '0';
        while(pad_len-- > 0) {
            stream_out(stream, pad_ch);
        }
    }
    // Print width left-pad if it's made out of space
    if(!(fmt.flags & FLAG_LEFT) && !(fmt.flags & FLAG_ZERO)) while(pad_len-- > 0) {
        stream_out(stream, ' ');
    }
    // Print prefix
    if(fmt.flags & FLAG_HASH) {
        stream_out(stream, '0');
        if(base == 16)     { stream_out(stream, 'x'); }
        else if(base == 2) { stream_out(stream, 'b'); }
    }
    else if(neg)                { stream_out(stream, '-'); }
    else if(fmt.flags & FLAG_PLUS)  { stream_out(stream, '+'); }
    else if(fmt.flags & FLAG_SPACE) { stream_out(stream, ' '); }
    // Print width left-pad if it's made out of zero
    if(!(fmt.flags & FLAG_LEFT) && (fmt.flags & FLAG_ZERO)) while(pad_len-- > 0) {
        stream_out(stream, '0');
    }
    // Print zero-pad due to precision
    for(int i = ndigits; i < fmt.prec; ++i) {
        stream_out(stream, '0');
    }
    // Output buffer in reverse
    if(ndigits) while(ndigits--) {
        stream_out(stream, digits[ndigits]);
    }
    // Print right-pad
    if(fmt.flags & FLAG_LEFT) while(pad_len-- > 0) {
        stream_out(stream, ' ');
    }
    return stream->w;
}
