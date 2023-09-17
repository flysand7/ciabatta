
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <stdatomic.h>
#include <cia/sync.h>
#include <cia/mem.h>

#include <cia-ld/tcb.h>

thrd_t thrd;
static Cia_Mutex g_mutex;
static volatile i64 counter = 0;

int thrd_func(void *arg) {
    _LD_Thread_Block *tcb = thrd.thread.handle;
    printf("child thread TID: %I64d\n", tcb->thread_id);
    printf("child thread: ok!\n");
    for(int i = 0; i < 100000; ++i) {
        cia_mutex_lock(&g_mutex);
        counter += 1;
        cia_mutex_unlock(&g_mutex);
    }
    for(;;);
    printf("child thread: counter = %I64d\n", counter);
    return 0;
}

int main() {
    _LD_Thread_Block *tcb = (void *)((u64)__builtin_frame_address(0) & ~(2*MB - 1));
    printf("main thread ID: %I64x\n", tcb->thread_id);
    printf("main thread: before\n");
    cia_mutex_init(&g_mutex);
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
    thrd_detach(thrd);
    printf("main thread: counter = %I64d\n", counter);
    thrd_terminate(thrd);
    printf("terminated child thread\n", counter);
    return 0;
}
