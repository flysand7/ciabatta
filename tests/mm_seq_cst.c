
#include <stdatomic.h>
#include <threads.h>
#include <stdio.h>

#if 1
    enum memory_order store_memory_order = memory_order_acquire;
    enum memory_order load_memory_order = memory_order_release;
#else
    enum memory_order store_memory_order = memory_order_seq_cst;
    enum memory_order load_memory_order = memory_order_seq_cst;
#endif

// Scheduler on linux has too coarse granularity
// to allow a thread to kind of a lot of work
// before switching to another thread. If we
// introduce an arbitrary delay, the threads will
// be able to be run in a random order better.
// Without it, the threads seem to be run as they are
// created, i.e. the scheduling is too convenient and
// hides a potential bug that we're testing for here
#define delay_that_works 1000000
#define arbitrary_delay(n) for(volatile int i = 0; i < n; ++i)

// This is all the explicit shared state between threads.
struct Shared_State typedef Shared_State;
struct Shared_State {
    _Atomic(int) x;
    _Atomic(int) y;
    _Atomic(int) cnt;
};
static Shared_State g_shared_state = {0};

// Sets the atomic flag x
int tx(void *ctx) {
    arbitrary_delay(delay_that_works);
    Shared_State *ss = ctx;
    char msg[] = "Thread tx running\n";
    fwrite(msg, 1, sizeof msg, stdout);
    atomic_store_explicit(&ss->x, 1, store_memory_order);
    return 0;
}

// Set the atomic flag y
int ty(void *ctx) {
    arbitrary_delay(delay_that_works);
    Shared_State *ss = ctx;
    char msg[] = "Thread ty running\n";
    fwrite(msg, 1, sizeof msg, stdout);
    atomic_store_explicit(&ss->y, 1, store_memory_order);
    return 0;
}

// Spinlock until flag x is set, if y is also set, add 1 to cnt
int t1(void *ctx) {
    arbitrary_delay(delay_that_works);
    Shared_State *ss = ctx;
    char msg[] = "Thread t1 running\n";
    fwrite(msg, 1, sizeof msg, stdout);
    while(atomic_load_explicit(&ss->x, load_memory_order) == 0)
        ;
    if(atomic_load_explicit(&ss->y, load_memory_order) == 1) {
        atomic_fetch_add_explicit(&ss->cnt, 1, memory_order_relaxed);
    }
    return 0;
}

// Spinlock until flag y is set, if x is also set, add 1 to cnt
int t2(void *ctx) {
    arbitrary_delay(delay_that_works);
    Shared_State *ss = ctx;
    char msg[] = "Thread t2 running\n";
    fwrite(msg, 1, sizeof msg, stdout);
    while(atomic_load_explicit(&ss->y, load_memory_order) == 0)
        ;
    if(atomic_load_explicit(&ss->x, load_memory_order) == 1) {
        atomic_fetch_add_explicit(&ss->cnt, 1, memory_order_relaxed);
    }
    return 0;
} 

int main() {
    // TODO: rewrite this to run in a loop after I
    // fix stack leaking on thread destruction
    thrd_start_t thread_funcs[4] = {tx,ty,t1,t2};
    thrd_t threads[4];
    // Spawn the threads
    for(int i = 0; i < 4; ++i) {
        int status = thrd_create(&threads[i], thread_funcs[i], &g_shared_state);
        if(status != thrd_success) {
            char msg[] = "Thread unable to start\n";
            fwrite(msg, 1, sizeof msg, stdout);
        }
    }
    // Wait for threads to complete
    for(int i = 0; i < 4; ++i) {
        int _exit_code;
        thrd_join(threads[i], &_exit_code);
    }
    // Check to see the cnt variable
    int result = atomic_load_explicit(&g_shared_state.cnt, memory_order_relaxed);
    if(result == 0) {
        char msg[] = "This shouldn't happen in sequentially-consistent model!\n";
        fwrite(msg, 1, sizeof msg, stdout);
    }
    return 0;
}
