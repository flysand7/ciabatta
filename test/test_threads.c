#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

atomic_int acnt;
int cnt;

int f(void* thr_data)
{
    for(int n = 0; n < 5; ++n)
        puts("b");
    return 0;
}

int main(void)
{
    thrd_t thread;
    int status = thrd_create(&thread, f, NULL);
    if(status == thrd_error) {
        puts("Failed creating threads");
    }
    for(int n = 0; n < 5; ++n) {
        puts("a");
    }
    int res;
    if(thrd_join(thread, &res) == thrd_error) {
        puts("Failed waiting on thread");
    }
    puts("Finished");
}
