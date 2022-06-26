
#include <stdio.h>
#include <stdlib.h>

typedef int (pfx(cbfn))(void *ctx, ctype ch);

static int pfx(vprintfcb)(
    void *ctx,
    cbfn *cb,
    const ctype *fmt,
    va_list args
) {
    int w = 0;
    while(*fmt) {
        while(*fmt && *fmt != '%') {
            if(!cb(ctx, *fmt))
                goto end;
            ++w;
            ++fmt;
        }
        if(*fmt == '%') {
            ++fmt;
            char f = *fmt++;
            switch(f) {
                case 's': {
                    char *str = va_arg(args, char *);
                    while(*str) {
                        if(!cb(ctx, *str))
                            goto end;
                        ++str;
                        ++w;
                    }
                } break;
                case 'd': case 'i': {
                    int num = va_arg(args, int);
                    char buffer[20];
                    char *str = buffer;
                    itoa(num, buffer, 10);
                    while(*str) {
                        if(!cb(ctx, *str))
                            goto end;
                        ++str;
                        ++w;
                    }
                } break;
            }
        }
    }
    goto end;
print_str:
    
end:
    return w;
}
