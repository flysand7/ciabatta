
#define AUX_CNT 32
#define DYN_CNT 37

#define _mfence() asm volatile("" ::: "memory")

struct Loader_Info typedef Loader_Info;
struct Loader_Info {
    u64 *sp;
    u8 *ldso_base;
    u64 *dyn;
    u64 *aux;
};

static void print_string_n(char *str, u64 len) {
    sys_write(STDOUT_FILENO, str, len);
}
static void print_char(char c) {
    print_string_n(&c, 1);
}
static void print_string(char *str) {
    int str_len = 0;
    while(str[str_len] != 0) {
        str_len += 1;
    }
    print_string_n(str, str_len);
}
static void print_int(i64 number) {
    if(number < 0) {
        print_char('-');
        number = -number;
    }
    char buf[20];
    buf[19] = 0;
    char *p = buf + sizeof buf - 1;
    do {
        *--p = (number%10) + '0';
        number /= 10;
    } while(number > 0);
    print_string(p);
}
static void print_uint(u64 number) {
    char buf[20];
    buf[19] = 0;
    char *p = buf + sizeof buf - 1;
    do {
        *--p = (number%10) + '0';
        number /= 10;
    } while(number > 0);
    print_string(p);
}
static void print_hex(u64 number) {
    print_string("0x");
    char digits[] = "0123456789abcdef";
    char buf[20];
    buf[19] = 0;
    char *p = buf + sizeof buf - 1;
    for(int i = 0; i < 64; i += 4) {
        // if(i != 0 && i % 16 == 0) {
        //     *--p = '_';
        // }
        u8 bits = (number >> i) & 0x0f;
        char digit = digits[bits];
        *--p = digit;
    }
    print_string(p);
}
static void printf(char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char str_buf[256];
    i64 buf_i = 0;
    while(*fmt != 0) {
        while(*fmt != '%' && *fmt != 0 && buf_i != sizeof str_buf-1) {
            str_buf[buf_i] = *fmt;
            buf_i += 1;
            fmt++;
        }
        str_buf[buf_i] = 0;
        print_string_n(str_buf, buf_i);
        buf_i = 0;
        if(*fmt == '%') {
            ++fmt;
            if(*fmt == 'd') {
                i64 number = va_arg(args, i64);
                print_int(number);
            }
            else if(*fmt == 'u') {
                u64 number = va_arg(args, u64);
                print_uint(number);
            }
            else if(*fmt == 'x') {
                u64 number = va_arg(args, u64);
                print_hex(number);
            }
            else if(*fmt == 's') {
                char *str = va_arg(args, char *);
                print_string(str);
            }
            else if(*fmt == 'c') {
                int c = va_arg(args, int);
                print_char(c);
            }
            ++fmt;
        }
    }
    va_end(args);
}

#if defined(_DEBUG) && defined(_CIA_LD_DEBUG)
    #define _dbg_print_char(c)       print_char(c)
    #define _dbg_print_string(s)     print_string(s)
    #define _dbg_print_string_n(s,n) print_string_n(s,n)
    #define _dbg_print_int(d)        print_int(d)
    #define _dbg_print_uint(u)       print_uint(u)
    #define _dbg_print_hex(x)        print_hex(x)
    #define _dbg_printf(fmt, ...)    printf(fmt, ##__VA_ARGS__)
#else
    #define _dbg_print_char(c)       do {} while(0)
    #define _dbg_print_string(s)     do {} while(0)
    #define _dbg_print_string_n(s,n) do {} while(0)
    #define _dbg_print_int(d)        do {} while(0)
    #define _dbg_print_uint(u)       do {} while(0)
    #define _dbg_print_hex(x)        do {} while(0)
    #define _dbg_printf(fmt, ...)    do {} while(0)
#endif

