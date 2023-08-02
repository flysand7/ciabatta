
#include <cia-def.h>
#include "syscall.c"

// void *memset(void *dst, int value, u64 num) {
//     u8 *bytes = dst;
//     for(int i = 0; i != num; ++i) {
//         bytes[i] = value;
//     }
//     return dst;
// }

static void write_char(char c) {
    _syscall_write(STDOUT_FILENO, &c, 1);
}

static void write_string(char *str) {
    int str_len = 0;
    while(str[str_len] != 0) {
        str_len += 1;
    }
    _syscall_write(STDOUT_FILENO, str, str_len);
}

static void write_int(i64 number) {
    if(number < 0) {
        write_char('-');
        number = -number;
    }
    char buf[20];
    buf[19] = 0;
    char *p = buf + sizeof buf - 1;
    do {
        *--p = (number%10) + '0';
        number /= 10;
    } while(number > 0);
    write_string(p);
}

static void write_uint(u64 number) {
    char buf[20];
    buf[19] = 0;
    char *p = buf + sizeof buf - 1;
    do {
        *--p = (number%10) + '0';
        number /= 10;
    } while(number > 0);
    write_string(p);
}

static void write_hex(u64 number) {
    write_string("0x");
    char digits[] = "0123456789abcdef";
    char buf[20];
    buf[19] = 0;
    char *p = buf + sizeof buf - 1;
    for(int i = 0; i < 64; i += 4) {
        if(i != 0 && i % 16 == 0) {
            *--p = '\'';
        }
        u8 bits = (number >> i) & 0x0f;
        char digit = digits[bits];
        *--p = digit;
    }
    write_string(p);
}

void _dlstart_c(u64 *sp, void *dynv) {
    write_string("Entered dynamic loader\n");
    int argc = *sp;
    char **argv = (void *)(sp+1);
    write_string("We're gonna load an executable: ");
    write_string(argv[0]);
    write_string("\n");
    write_string("With parameters: ");
    for(int i = 1; i < argc; ++i) {
        if(i != 1) {
            write_string(", ");
        }
        write_string(argv[i]);
    }
    write_string("\n");
    _syscall_exit(0);
}
