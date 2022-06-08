// NOTE: this file doesn't exist in a vacuum, it's a template for generating
// the formatted print, you should define FMT_CHAR_TYPE before including it
inline static int FMT_FUNC_NAME (void *ctx, OutputFunc out, const FMT_CHAR_TYPE *fmt, va_list args) {
    size_t full_length = 0;

    while (*fmt) {
        // skip any normal non percent text
        const FMT_CHAR_TYPE *start = fmt;
        while (*fmt && *fmt != '%') fmt++;

        // print all those non percent text
        out(ctx, fmt - start, start);
        full_length += (fmt - start);

        // null terminated
        if (*fmt == '\0') break;
        fmt += 1;

        if (*fmt == '%') {
            out(ctx, 1, "%");
            fmt++;
            continue;
        }

        unsigned int precision = 0;
        if (*fmt == '.') {
            // custom precision
            fmt++;

            if (isdigit(*fmt)) {
                // just a small atoi
                // TODO: handle overflow, just in case(?)
                while (isdigit(*fmt)) {
                    precision *= 10u;
                    precision += (*fmt - '0');

                    fmt++;
                }
            } else if (*fmt == '*') {
                int p = va_arg(args, int);

                precision = p >= 0 ? ((unsigned int)p) : 0;
                fmt++;
            }
        }

        // integer length specifiers
        enum {
            CHAR,
            SHORT,
            INT,
            LONG,
            LLONG
        } int_length = INT;

        if (*fmt == 'l') {
            fmt++;

            if (*fmt == 'l') {
                int_length = LLONG;
                fmt++;
            } else {
                int_length = LONG;
            }
        } else if (*fmt == 'h') {
            fmt++;

            if (*fmt == 'h') {
                int_length = CHAR;
                fmt++;
            } else {
                int_length = SHORT;
            }
        } else if (*fmt == 'z') {
            fmt++;

            int_length = _Generic((size_t)0,
                                  unsigned short:     SHORT,
                                  unsigned int:       INT,
                                  unsigned long:      LONG,
                                  unsigned long long: LLONG);
        }

        FMT_CHAR_TYPE ch = *fmt++;
        switch (ch) {
            case 'c': {
                const char chr = va_arg(args, int);
                out(ctx, 1, &chr);
                full_length ++;
                break;
            }
            case 's': {
                const FMT_CHAR_TYPE *str = va_arg(args, FMT_CHAR_TYPE*);
                size_t len = FMT_STRLEN_S(str, precision ? precision : SIZE_MAX);

                out(ctx, len, str);
                full_length += len;
                break;
            }

            case 'b':
            case 'o':
            case 'i':
            case 'u':
            case 'd':
            case 'x':
            case 'X': {
                int base = 10;
                switch (ch) {
                    case 'X': case 'x': base = 16; break;
                    case 'o':           base = 8;  break;
                    case 'b':           base = 2;  break;
                    default:            base = 10; break;
                }

                const char* characters = "0123456789abcdef";
                if (ch == 'X') characters = "0123456789ABCDEF";

                uintmax_t i;
                if (ch == 'd' || ch == 'i') {
                    intmax_t num = 0;
                    switch (int_length) {
                        case CHAR:  num = (char) va_arg(args, int); break;
                        case SHORT: num = (short) va_arg(args, int); break;
                        case INT:   num = va_arg(args, int); break;
                        case LONG:  num = va_arg(args, long); break;
                        case LLONG: num = va_arg(args, long long); break;
                        default: break;
                    }

                    if (num < 0) {
                        out(ctx, 1, "-");
                        i = -num;
                        full_length += 1;
                    } else {
                        i = num;
                    }
                } else {
                    switch (int_length) {
                        case CHAR:  i = (char) va_arg(args, int); break;
                        case SHORT: i = (short) va_arg(args, int); break;
                        case INT:   i = va_arg(args, int); break;
                        case LONG:  i = va_arg(args, long); break;
                        case LLONG: i = va_arg(args, long long); break;
                        default: i = 0; break;
                    }
                }

                if (i == 0) {
                    out(ctx, 1, "0");
                    full_length += 1;
                } else {
                    FMT_CHAR_TYPE buffer[20];
                    size_t len = sizeof(buffer);

                    // we build the number in reverse
                    while (i != 0) {
                        buffer[--len] = characters[i % base];
                        i /= base;
                    }

                    out(ctx, sizeof(buffer) - (len * sizeof(FMT_CHAR_TYPE)), buffer + len);
                    full_length += (sizeof(buffer) - len);
                }
                break;
            }

            default: break;
        }
    }

    return full_length;
}

