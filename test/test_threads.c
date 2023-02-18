#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

#define N_THREADS 5

_Thread_local int counter;
once_flag flag = ONCE_FLAG_INIT;

void init() {
    puts("Hey I got a call");
}

int f(void* thr_data) {
    call_once(&flag, init);
    for(int n = 0; n < 5; ++n)
        counter++;
    puts("Finished");
    return 0;
}

int main(void)
{
    thrd_t thread[N_THREADS];
    for(int i = 0; i != N_THREADS; ++i) {
        int status = thrd_create(&thread[i], f, NULL);
        if(status == thrd_error) {
            puts("Failed creating threads");
        }
    }
    for(int i = 0; i != N_THREADS; ++i) {
        int res;
        if(thrd_join(thread[i], &res) == thrd_error) {
            puts("Failed waiting on thread");
        }
    }
}
