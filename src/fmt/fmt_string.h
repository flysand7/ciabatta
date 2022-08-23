
static inline int pfx(fmt_isdigit)(ctype ch) {
    return '0' <= ch && ch <= '9';
}

static inline int pfx(fmt_atoi)(ctype const **sp, int *err) {
    int i = 0;
    ctype const *s = *sp;
    while(pfx(fmt_isdigit(*s))) {
        int d = *s-'0';
        if(i > (INT_MAX - d)/10) {
            *err = 1;
            return 0;
        }
        i = 10*i + d;
        ++s;
    }
    *sp = s;
    return i;
}

static inline int pfx(fmt_read)(const ctype *str, fmt_t *fmt, va_list va) {
    int flags = 0;
    // Parse flag field
    {
        int isflag = 0;
        do {
            isflag = 1;
            switch(*str) {
                case '0': flags |= FLAG_ZERO;  break;
                case '-': flags |= FLAG_LEFT;  break;
                case '+': flags |= FLAG_PLUS;  break;
                case ' ': flags |= FLAG_SPACE; break;
                case '#': flags |= FLAG_HASH;  break;
                default:  isflag = 0;
            }
            if(isflag) ++str;
        } while(isflag);
        // If '-' and '0' are together we just discard '0'
        if(flags & FLAG_LEFT) flags &= ~FLAG_ZERO;
    }
    // Parse width field
    int width = 0;
    if(pfx(fmt_isdigit(*str))) {
        int err = 0;
        width = pfx(fmt_atoi(&str, &err));
        if(err) return 0;
    }
    else if(*str == '*') {
        ++str;
        width = va_arg(va, int);
        if(width < 0) {
            width = -width;
            flags |= FLAG_LEFT;
        }
    }
    // If present, parse the precision field
    int prec = 0;
    if(*str == '.') {
        flags |= FLAG_PREC;
        ++str;
        if(pfx(fmt_isdigit(*str))) {
            int err = 0;
            prec = pfx(fmt_atoi(&str, &err));
            if(err) return -1;
        }
        else if(*str == '*') {
            ++str;
            prec = va_arg(va, int);
            prec = (prec > 0? prec : 0);
        }
    }
    return 1;
}
