// NOTE: debug mutexes will follow the recursive logic but error if they
// actually recurse, this is slower than doing plain logic but it helps
// debug weird mutex errors.
//
// Based on these posts:
//   https://preshing.com/20120305/implementing-a-recursive-mutex/
//   https://preshing.com/20120226/roll-your-own-lightweight-mutex/
#include <win.h>
#include <threads.h>

void mtx_destroy(mtx_t *mtx) {
    CloseHandle(mtx->semaphore);
}

int mtx_init(mtx_t *mtx, int type) {
    *mtx = (mtx_t){
        .type = type,
        .semaphore = CreateSemaphore(NULL, 0, 1, NULL)
    };

    if (type == mtx_timed) {
        // TODO(NeGate): implement timed mutexes
        return thrd_error;
    }

    return thrd_success;
}

int mtx_lock(mtx_t *mtx) {
    bool try_recursive = (mtx->type == mtx_recursive);
#ifdef _DEBUG
    try_recursive = true;
#endif

    if (try_recursive) {
        DWORD tid = GetCurrentThreadId();

        if (atomic_fetch_add_explicit(&mtx->counter, 1, memory_order_acquire) > 1) {
            if (tid != mtx->owner) {
                WaitForSingleObject(mtx->semaphore, INFINITE);
            } else {
                // we recursive and already locked
#ifdef _DEBUG
                if (mtx->type != mtx_recursive) {
                    return thrd_error;
                }
#endif
            }
        }

        mtx->owner = tid;
        mtx->recursion++;
    } else {
        if (atomic_fetch_add_explicit(&mtx->counter, 1, memory_order_acquire) > 1) {
            WaitForSingleObject(mtx->semaphore, INFINITE);
        }
    }

    return thrd_success;
}

int mtx_timedlock(mtx_t *restrict mtx, const struct timespec *restrict ts) {
    return thrd_error;
}

int mtx_trylock(mtx_t *mtx) {
    bool try_recursive = (mtx->type == mtx_recursive);
#ifdef _DEBUG
    try_recursive = true;
#endif

    if (try_recursive) {
        DWORD tid = GetCurrentThreadId();

        // Do we own this mutex on this thread already?
        if (mtx->owner == tid) {
#ifdef _DEBUG
            if (mtx->type != mtx_recursive) {
                return thrd_error;
            }
#endif

            atomic_fetch_add(&mtx->counter, 1);
        } else {
            int expected = 1;
            if (!atomic_compare_exchange_strong(&mtx->counter, &expected, 0)) {
                return thrd_busy;
            }

            mtx->owner = tid;
        }

        mtx->recursion++;
        return thrd_success;
    } else {
        int expected = 1;
        if (!atomic_compare_exchange_strong(&mtx->counter, &expected, 0)) {
            return thrd_busy;
        }

        return thrd_success;
    }
}

int mtx_unlock(mtx_t *mtx) {
    bool try_recursive = (mtx->type == mtx_recursive);
#if _DEBUG
    try_recursive = true;
#endif

    if (try_recursive) {
        DWORD tid = GetCurrentThreadId();
        if (tid != mtx->owner) return thrd_error;

        unsigned long recur = --mtx->recursion;
        if (recur == 0) {
            mtx->owner = 0;
        }

        if (atomic_fetch_sub_explicit(&mtx->counter, 1, memory_order_release) > 0) {
            if (recur == 0) ReleaseSemaphore(mtx->semaphore, 1, NULL);
            else {
#ifdef _DEBUG
                if (mtx->type != mtx_recursive) {
                    return thrd_error;
                }
#endif
            }
        }
    } else {
        // release?
        if (atomic_fetch_sub_explicit(&mtx->counter, 1, memory_order_release) > 0) {
            ReleaseSemaphore(mtx->semaphore, 1, NULL);
        }
    }

    return thrd_success;
}
