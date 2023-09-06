
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>
#include <cia/sync.h>

static void print_string_n(char *str, u64 len) {
    fwrite(str, 1, len, stdout);
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

static Cia_Mutex g_print_mutex;
static Cia_Mutex g_mutex;
static volatile i64 counter = 0;

int thrd_func(void *arg) {
    print_string("child thread: ok!\n");
    for(int i = 0; i < 100000; ++i) {
        cia_mutex_lock(&g_mutex);
        counter += 1;
        cia_mutex_unlock(&g_mutex);
    }
    cia_mutex_lock(&g_print_mutex);
    print_string("child thread: counter = ");
    print_int(counter);
    print_char('\n');
    cia_mutex_unlock(&g_print_mutex);
    return 0;
}

int main() {
    print_string("main thread: before\n");
    cia_mutex_init(&g_mutex);
    cia_mutex_init(&g_print_mutex);
    thrd_t thrd;
    int status = thrd_create(&thrd, thrd_func, NULL);
    if(status == thrd_error) {
        print_string("main thread: error creating child thread\n");
        return 1;
    }
    print_string("main thread: after!\n");
    for(int i = 0; i < 100000; ++i) {
        cia_mutex_lock(&g_mutex);
        counter += 1;
        cia_mutex_unlock(&g_mutex);
    }
    int exit_code;
    thrd_join(thrd, &exit_code);
    cia_mutex_lock(&g_print_mutex);
    print_string("main thread: counter = ");
    print_int(counter);
    print_char('\n');
    cia_mutex_unlock(&g_print_mutex);
    return 0;
}
