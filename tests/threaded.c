
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

static void print_str(char *str) {
    int str_len = 0;
    while(str[str_len] != 0) {
        ++str_len;
    }
    fwrite(str, 1, str_len, stdout);
}

int thrd_func(void *arg) {
    print_str("child thread: ok!\n");
    for(;;);
    return 0;
}

int main() {
    print_str("main thread: before\n");
    thrd_t thrd;
    thrd_create(&thrd, thrd_func, NULL);
    print_str("main thread: after!\n");
    return 0;
}
