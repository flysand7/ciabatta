
// NOTE: debug mutexes will follow the recursive logic but error if they
// actually recurse, this is slower than doing plain logic but it helps
// debug weird mutex errors.
//
// Based on these posts:
//   https://preshing.com/20120305/implementing-a-recursive-mutex/
//   https://preshing.com/20120226/roll-your-own-lightweight-mutex/

DWORD _tls_index = 0;

typedef struct thrd__wrapper_info {
    thrd_start_t func;
    void* arg;
} thrd__wrapper_info;

static DWORD thrd__wrapper(void* arg) {
    thrd__wrapper_info info = *((thrd__wrapper_info*) arg);
    int result = info.func(info.arg);

    // TODO(NeGate): setup TSS dtors here
    return (DWORD) result;
}

thrd_t thrd_current(void) {
    return (thrd_t){ GetCurrentThread() };
}

int thrd_create(thrd_t *thr, thrd_start_t func, void *arg) {
    thrd__wrapper_info* info = malloc(sizeof(thrd__wrapper_info));
    if (info == NULL) {
        return thrd_nomem;
    }

    info->func = func;
    info->arg = arg;

    // technically thrd_start_t and LPTHREAD_START_ROUTINE aren't the same but are close
    // enough to be ABI compatible, namely a difference in signedness of the return val.
    thr->handle = CreateThread(NULL, 0, thrd__wrapper, info, 0, NULL);
    return thr->handle != NULL ? thrd_success : thrd_error;
}

int thrd_detach(thrd_t thr) {
    return CloseHandle(thr.handle) != 0 ? thrd_success : thrd_error;
}

int thrd_equal(thrd_t thr0, thrd_t thr1) {
    return GetThreadId(thr0.handle) == GetThreadId(thr1.handle);
}

int thrd_join(thrd_t thr, int *res) {
    DWORD wait = WaitForSingleObject(thr.handle, INFINITE);
    if (wait == WAIT_FAILED) {
        return thrd_error;
    } else if (wait == WAIT_TIMEOUT) {
        return thrd_timedout;
    }

    if (res != NULL) {
        // snatch that exit code
        DWORD ures;
        if (GetExitCodeThread(thr.handle, &ures) == 0) {
            CloseHandle(thr.handle);
            return thrd_error;
        }

        *res = (int) ures;
    }

    CloseHandle(thr.handle);
    return thrd_success;
}

void thrd_yield(void) {
    Sleep(0);
}

_Noreturn void thrd_exit(int res) {
    // TODO(NeGate): setup TSS dtors here
    ExitThread((DWORD) res);
}

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
