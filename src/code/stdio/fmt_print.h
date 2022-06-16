
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>
#include <math.h>

// This stuff is kinda related to what's going on in this file, so I left it
// in a rather ugly manner here.
#if !defined(once)
    #define once
    
    #define countof(arr) (sizeof(arr)/sizeof(arr[0]))

    #define CALL_PRINTF(fmt_func, ctx, out, fmt)       \
        va_list args;                                  \
        va_start(args, fmt);                           \
        int result = fmt_func(ctx, out, fmt, args);    \
        va_end(args)

    enum ArgLength {
        LEN_HH,
        LEN_H,
        LEN_I,
        LEN_L,
        LEN_LL,
        LEN_J,
        LEN_Z,
        LEN_T,
    } typedef ArgLength;

    enum ArgConv {
        CONV_INT,
        CONV_FLT,
        CONV_EXP,
        CONV_SCI,
        CONV_CHAR,
        CONV_STR,
        CONV_PTR,
        CONV_N,
        CONV_PERCENT,
    } typedef ArgConv;

    struct Format typedef Format;
    struct Format {
        bool flag_left;
        bool flag_sign;
        bool flag_space;
        bool flag_form;
        bool flag_zero;
        bool flag_upper;
        bool flag_unsigned;
        int base;
        int width;
        int prec;
        ArgLength arg_len;
        ArgConv arg_conv;
    };

    inline static uint64_t get_int_arg(Format *fmt, va_list args, bool *signp) {
        bool sign = 0;
        uint64_t num = 0;

        // Read the integer from the args and split it into tuple of (sign, digits)
        // If we've got HH or H we need to parse as int, and do the necessary conv
        // to unsigned if needed.
        if(fmt->arg_len < LEN_I) {
            int ch = va_arg(args, int);
            if(ch < 0) {
                if(fmt->flag_unsigned) {
                    num = ch + (fmt->arg_len == LEN_HH? UCHAR_MAX : USHRT_MAX);
                }
                else {
                    num = -ch;
                    sign = 1;
                }
            }
        }
        // Otherwise we see whether the number is signed or unsigned. For unsigned
        // numbers we just parse according to the length, for signed we do that
        // plus handle the sign
        else if(fmt->flag_unsigned) {
            switch(fmt->arg_len) {
                case LEN_I:  num = va_arg(args, unsigned int); break;
                case LEN_L:  num = va_arg(args, unsigned long); break;
                case LEN_LL: num = va_arg(args, unsigned long long); break;
                case LEN_J:  num = va_arg(args, uintmax_t); break;
                case LEN_Z:  num = va_arg(args, size_t); break;
                case LEN_T:  num = va_arg(args, size_t); break;
                default:;
            }
        }
        else {
            // TODO: signed size_t
            int64_t i = 0;
            switch(fmt->arg_len) {
                case LEN_I:  i = va_arg(args, int); break;
                case LEN_L:  i = va_arg(args, long); break;
                case LEN_LL: i = va_arg(args, long long); break;
                case LEN_J:  i = va_arg(args, uintmax_t); break;
                case LEN_Z:  i = va_arg(args, int64_t); break;
                case LEN_T:  i = va_arg(args, ptrdiff_t); break;
                default:;
            }
            if(i < 0) {
                num = -i;
                sign = 1;
            }
            else {
                num = i;
            }
        }

        *signp = sign;
        return num;
    }
#endif

// Generic printf function that is used to print to stdout, strings and files.
// The basic idea is to make generic printf that uses output function as
// parameter and that function is distinct for printing to files, strings etc.
// Furthermore, this generic printf is abbstracted over character type, and
// depending on the macro fchar it could print in char's or wchar_t's.

typedef void (suffix(out_func_t))(void* ctx, fchar ch);

inline static int suffix(fmt_atoi)(fchar const *str, int *value) {
    int i = 0;
    int val = 0;
    while('0' <= str[i] && str[i] <= '9') {
        int digit = str[i] - '0';
        if(val > (INT_MAX - digit) / 10) {
            return -1;
        }
        val = 10*val + digit;
        ++i;
    }
    *value = val;
    return i;
}

inline static int suffix(fmt_parse)(
    Format *format,
    fchar const *str,
    va_list args
) {
    *format = (Format){0};
    // Parse flags
    int i = 0;
    for(;;) {
        if     (str[i] == '-') format->flag_left = true;
        else if(str[i] == '+') format->flag_sign = true;
        else if(str[i] == ' ') format->flag_space = true;
        else if(str[i] == '#') format->flag_form = true;
        else if(str[i] == '0') format->flag_zero = true;
        else break;
        ++i;
    }
    // Optional width specifier
    int width = 0;
    if(str[i] == '*') {
        ++i;
        width = va_arg(args, int);
        if(width < 0) {
            width = -width;
            format->flag_zero = true;
        }
    }
    else {
        int width_len = suffix(fmt_atoi)(str+i, &width);
        if(width_len < 0) return -1;
        i += width_len;
    }
    // Optional precision specifier
    int prec = 0;
    if(str[i] == '.') {
        ++i;
        if(str[i] == '*') {
            ++i;
            prec = va_arg(args, int);
            if(prec < 0) {
                prec = 0;
            }
        }
        else {
            int prec_len = suffix(fmt_atoi)(str+i, &prec);
            if(prec_len < 0) return -1;
            i += prec_len;
        }
    }
    format->width = width;
    format->prec = prec;
    // Optional length modifier
    format->arg_len = LEN_I;
    if(str[i] == 'h') {
        ++i;
        format->arg_len = LEN_H;
        if(str[i] == 'h') {
            ++i;
            format->arg_len = LEN_HH;
        }
    }
    else if(str[i] == 'l') {
        ++i;
        format->arg_len = LEN_L;
        if(str[i] == 'l') {
            ++i;
            format->arg_len = LEN_LL;
        }
    }
    else if(str[i] == 'j') {
        ++i;
        format->arg_len = LEN_J;
    }
    else if(str[i] == 'z') {
        ++i;
        format->arg_len = LEN_Z;
    }
    else if(str[i] == 't') {
        ++i;
        format->arg_len = LEN_T;
    }
    else if(str[i] == 'L') {
        ++i;
        format->arg_len = LEN_L;
    }
    // Conversion specifier
    switch(str[i]) {
        case 'd':
        case 'i':
            format->arg_conv = CONV_INT;
            format->base = 10;
            break;
        case 'u':
            format->arg_conv = CONV_INT;
            format->flag_unsigned = true;
            format->base = 10;
            break;
        case 'o':
            format->arg_conv = CONV_INT;
            format->flag_unsigned = true;
            format->base = 8;
            break;
        case 'x':
            format->arg_conv = CONV_INT;
            format->flag_unsigned = true;
            format->base = 16;
            break;
        case 'X':
            format->arg_conv = CONV_INT;
            format->flag_unsigned = true;
            format->flag_upper = true;
            format->base = 16;
            break;
        case 'f':
            format->arg_conv = CONV_FLT;
            format->base = 10;
            break;
        case 'F':
            format->arg_conv = CONV_FLT;
            format->flag_upper = true;
            format->base = 10;
            break;
        case 'e':
            format->arg_conv = CONV_EXP;
            format->base = 10;
            break;
        case 'E':
            format->arg_conv = CONV_EXP;
            format->flag_upper = true;
            format->base = 10;
            break;
        case 'g':
            format->arg_conv = CONV_SCI;
            format->base = 10;
            break;
        case 'G':
            format->arg_conv = CONV_SCI;
            format->flag_upper = true;
            format->base = 10;
            break;
        case 'a':
            format->arg_conv = CONV_FLT;
            format->base = 16;
            break;
        case 'A':
            format->arg_conv = CONV_FLT;
            format->flag_upper = true;
            format->base = 16;
            break;
        case 'c':
            format->arg_conv = CONV_CHAR;
            break;
        case 's':
            format->arg_conv = CONV_STR;
            break;
        case 'p':
            format->arg_conv = CONV_PTR;
            break;
        case 'n':
            format->arg_conv = CONV_N;
            break;
        case '%':
            format->arg_conv = CONV_PERCENT;
            break;
        default: return -1;
    }
    ++i;
    // Ignoring flags etc
    if(format->flag_sign && format->flag_zero) {
        format->flag_zero = 0;
    }
    if(format->flag_sign && format->flag_space) {
        format->flag_space = 0;
    }

    if(format->prec != 0) {
        format->flag_zero = 0;
    }
    else if(format->flag_zero) {
        // HACK: NOTE: I hate printf formats
        format->prec = format->width;
        format->width = 0;
    }
    if(format->arg_conv == CONV_FLT && format->prec == 0) {
        format->prec = 6;
    }
    return i;
}

inline static int suffix(fmt_fprint_int)(
    void *ctx,
    suffix(out_func_t) *out,
    Format *fmt,
    va_list args
) {
    int w = 0;
    
    bool sign;
    uint64_t num = get_int_arg(fmt, args, &sign);

    // Find the actual length the number takes up
    int prefix_len = 0;
    int digits_num = 0;
    if(fmt->flag_space || fmt->flag_sign || sign) {
        prefix_len = 1;
    }
    if(num != 0 && fmt->flag_form) {
        if(fmt->base == 16) prefix_len += 2; // 0x
        if(fmt->base == 8)  digits_num += 1; // 0
    }

    {
        uint64_t digits = num;
        do {
            digits_num += 1;
            digits /= fmt->base;
        } while(digits != 0);
    }
    int s_len = prefix_len + digits_num; // length of significant chars
    int d_len = s_len;                   // length of all displayed chars
    int zpad = 0;                        // pre-pad of zeroes from precision
    if(digits_num < fmt->prec) {
        zpad = fmt->prec - digits_num;
        d_len = s_len + zpad;
    }

    // See if padding is necessary and extract info about padding
    bool left_pad = false;
    bool right_pad = true;
    char pad_ch = ' ';
    int pad_len = 0;
    if(!sign && fmt->flag_space) { // make space part of pad
        d_len += 1;
        pad_len = 1;
    }
    if(d_len < fmt->width) {
        pad_len = fmt->width - d_len;
    }
    if(fmt->flag_left) {
        left_pad = true;
        right_pad = false;
    }
    if(fmt->flag_zero) {
        pad_ch = '0';
    }

    // Fill up an array of significant digits
    char digits_arr[33] = {0};
    char *digits = digits_arr + countof(digits_arr);
    {
        uint64_t temp = num;
        do {
            int digit = temp % fmt->base;
            char ch = 0;
            if(digit < 10)           ch = digit + '0';
            else if(fmt->flag_upper) ch = digit - 10 + 'A';
            else                     ch = digit - 10 + 'a';
            *--digits = ch;
            temp /= fmt->base;
        } while(temp != 0);
    }

    // Start displaying number
    // Left-pad if necessary
    if(left_pad) while(pad_len--) {
        out(ctx, pad_ch);
        ++w;
    }

    // Print sign
    if(sign)                 { out(ctx, '-'); ++w; }
    else if(fmt->flag_sign)  { out(ctx, '+'); ++w; }
    else if(fmt->flag_space) { out(ctx, ' '); ++w; }

    // Print 0x, 0X
    if(num != 0 && fmt->base == 16 && fmt->flag_form) {
        out(ctx, '0');
        if(fmt->flag_upper) out(ctx, 'X');
        else                out(ctx, 'x');
        w += 2;
    }

    // Print zpad
    while(zpad--) {
        out(ctx, '0');
        ++ w;
    }

    // Print digit string
    while(*digits != 0) {
        out(ctx, *digits);
        ++digits;
        ++w;
    }

    // Right-pad if necessary
    if(right_pad) while(pad_len--) {
        out(ctx, pad_ch);
        ++w;
    }

    return w;
}

inline static int suffix(fmt_fprint_ptr)(
    void *ctx,
    suffix(out_func_t) *out,
    Format *fmt,
    va_list args
) {
    int w = 0;
    uintptr_t num = va_arg(args, uintptr_t);

    int s_len = 2 + 16; // length of significant chars
    int d_len = s_len;                   // length of all displayed chars
    int zpad = 0;                        // pre-pad of zeroes from precision
    if(16 < fmt->prec) {
        zpad = fmt->prec - 16;
        d_len = s_len + zpad;
    }

    // See if padding is necessary and extract info about padding
    bool left_pad = true;
    bool right_pad = false;
    char pad_ch = ' ';
    int pad_len = 0;
    if(d_len < fmt->width) {
        pad_len = fmt->width - d_len;
    }
    if(fmt->flag_left) {
        left_pad = false;
        right_pad = true;
    }
    if(fmt->flag_zero) {
        pad_ch = '0';
    }

    // Fill up an array of significant digits
    char digits_arr[33] = {0};
    char *digits = digits_arr + countof(digits_arr);
    {
        uint64_t temp = num;
        do {
            int digit = temp % 16;
            char ch = 0;
            if(digit < 10)           ch = digit + '0';
            else if(fmt->flag_upper) ch = digit - 10 + 'A';
            else                     ch = digit - 10 + 'a';
            *--digits = ch;
            temp /= 16;
        } while(s_len-- != 0);
    }

    // Start displaying number
    // Left-pad if necessary
    if(left_pad) while(pad_len--) {
        out(ctx, pad_ch);
        ++w;
    }

    out(ctx, '0');
    out(ctx, 'x');
    w += 2;

    // Print zpad
    while(zpad--) {
        out(ctx, '0');
        ++ w;
    }

    // Print digit string
    while(*digits != 0) {
        out(ctx, *digits);
        ++digits;
        ++w;
    }

    // Right-pad if necessary
    if(right_pad) while(pad_len--) {
        out(ctx, pad_ch);
        ++w;
    }

    return w;
}

inline static int suffix(fmt_fprint_char)(
    void *ctx,
    suffix(out_func_t) *out,
    Format *fmt,
    va_list args
) {
    int w = 0;
    bool sign;
    uint64_t num = get_int_arg(fmt, args, &sign);

    // Calculate padding
    bool left_pad = true;
    bool right_pad = false;
    uint64_t pad = 0;
    if(1 < fmt->width) {
        pad = fmt->width - 1;
    }
    if(fmt->flag_left) {
        left_pad = false;
        right_pad = true;
    }

    // Print left pad
    if(left_pad) while(pad--) {
        out(ctx, ' ');
        ++w;
    }

    out(ctx, num);
    ++w;

    // Print right pad
    if(right_pad) while(pad--) {
        out(ctx, ' ');
        ++w;
    }

    return w;
}

inline static int suffix(fmt_just_fucking_print_float_someone_improve_this_later)(
    void *ctx,
    suffix(out_func_t) *out,
    Format *fmt,
    va_list args
) {
    int w = 0;
    double f = va_arg(args, double);
    uint64_t i = (uint64_t)f;

    char digits_arr[33] = {0};
    char *digits = digits_arr + countof(digits_arr);
    {
        uint64_t t = i;
        do {
            char ch = (t % 10) + '0';
            *--digits = ch;
            t /= 10;
        } while(t-- != 0);
    }

    while(*digits != 0) {
        out(ctx, *digits);
        ++digits;
        ++w;
    }

    f -= i;
    uint64_t nprec = fmt->prec;
    if(nprec != 0) {
        out(ctx, '.');
        w++;
    }

    for(int i = 0; i != fmt->prec; ++i) {
        f *= 10;
        uint64_t digit = (uint64_t)f;
        out(ctx, digit + '0');
        ++w;
        f -= digit;
    }

    return w;
}


inline static int suffix(fmt_fprint_str)(
    void *ctx,
    suffix(out_func_t) *out,
    Format *fmt,
    va_list args
) {
    int w = 0;
    char *str = va_arg(args, char *);
    uint64_t len = strlen(str);

    // Calculate padding
    bool left_pad = true;
    bool right_pad = false;
    uint64_t pad = 0;
    if(len < fmt->width) {
        pad = fmt->width - len;
    }
    if(fmt->flag_left) {
        left_pad = false;
        right_pad = true;
    }

    // Print left pad
    if(left_pad) while(pad--) {
        out(ctx, ' ');
        ++w;
    }

    while(*str) {
        out(ctx, *str++);
        ++w;
    }

    // Print right pad
    if(right_pad) while(pad--) {
        out(ctx, ' ');
        ++w;
    }

    return w;
}

inline static int suffix(fmt_print)(
    void *ctx,
    suffix(out_func_t) *out,
    const fchar *fmt,
    va_list args
) {
    int w = 0; // N chars output
    int i = 0; // N chars read from fmt
    while(fmt[i] != 0) {
        if(fmt[i] == '%') {
            ++i;
            Format format;
            int fmt_len = suffix(fmt_parse)(&format, fmt+i, args);
            if(fmt_len < 0) return -1;
            i += fmt_len;
            int written;
            switch(format.arg_conv) {
                case CONV_INT: {
                    written = suffix(fmt_fprint_int)(ctx, out, &format, args);
                } break;
                case CONV_FLT: {
                    written = suffix(fmt_just_fucking_print_float_someone_improve_this_later)(ctx, out, &format, args);
                } break;
                case CONV_EXP: {
                    written = suffix(fmt_just_fucking_print_float_someone_improve_this_later)(ctx, out, &format, args);
                } break;
                case CONV_SCI: {
                    written = suffix(fmt_just_fucking_print_float_someone_improve_this_later)(ctx, out, &format, args);
                } break;
                case CONV_CHAR: {
                    written = suffix(fmt_fprint_char)(ctx, out, &format, args);
                } break;
                case CONV_STR: {
                    written = suffix(fmt_fprint_str)(ctx, out, &format, args);
                } break;
                case CONV_PTR: {
                    written = suffix(fmt_fprint_ptr)(ctx, out, &format, args);
                } break;
                case CONV_N: {
                    int *n = va_arg(args, int *);
                    *n = w;
                    written = 0;
                } break;
                case CONV_PERCENT: {
                    out(ctx, '%');
                    written = 1;
                } break;
                if(written < 0) {
                    return -1;
                }
                w += written;
            }
        }
        // Print non-formatted characters as god intended
        else while(fmt[i] != '%' && fmt[i] != 0) {
            out(ctx, fmt[i]);
            ++i;
            ++w;
        }
    }
    return i;
}

