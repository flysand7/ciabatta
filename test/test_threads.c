#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

_Thread_local int counter;
tss_t key;

int f(void* thr_data) {
    tss_set(key, "Thread 2 finished");
    for(int n = 0; n < 5; ++n)
        counter++;
    puts(tss_get(key));
    return 0;
}

int main(void)
{
    tss_create(&key, NULL);
    thrd_t thread;
    int status = thrd_create(&thread, f, NULL);
    if(status == thrd_error) {
        puts("Failed creating threads");
    }
    for(int n = 0; n < 10; ++n) {
        counter++;
    }
    tss_set(key, "Thread 1 finished");
    int res;
    if(thrd_join(thread, &res) == thrd_error) {
        puts("Failed waiting on thread");
    }
    puts(tss_get(key));
    tss_delete(key);
}
