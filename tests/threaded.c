
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>
#include <cia/sync.h>

static Cia_Mutex g_mutex;
static volatile i64 counter = 0;

int thrd_func(void *arg) {
    printf("child thread: ok!\n");
    for(int i = 0; i < 100000; ++i) {
        cia_mutex_lock(&g_mutex);
        counter += 1;
        cia_mutex_unlock(&g_mutex);
    }
    printf("child thread: counter = %I64d\n", counter);
    return 0;
}

int main() {
    printf("main thread: before\n");
    cia_mutex_init(&g_mutex);
    thrd_t thrd;
    int status = thrd_create(&thrd, thrd_func, NULL);
    if(status == thrd_error) {
        printf("main thread: error creating child thread\n");
        return 1;
    }
    printf("main thread: after!\n");
    for(int i = 0; i < 100000; ++i) {
        cia_mutex_lock(&g_mutex);
        counter += 1;
        cia_mutex_unlock(&g_mutex);
    }
    int exit_code;
    thrd_join(thrd, &exit_code);
    printf("main thread: counter = %I64d\n", counter);
    return 0;
}
