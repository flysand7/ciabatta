
// Note(bumbread):
// https://gist.github.com/wbenny/6d7fc92e9b5c3194ce56bf8c60d6191d

#pragma comment(linker, "/merge:.CRT=.rdata")

#pragma section(".CRT$XLA", read)
__declspec(allocate(".CRT$XLA")) const PIMAGE_TLS_CALLBACK __xl_a = NULL;

#pragma section(".CRT$XLZ", read)
__declspec(allocate(".CRT$XLZ")) const PIMAGE_TLS_CALLBACK __xl_z = NULL;

#pragma section(".CRT$XLM", read)
__declspec(allocate(".CRT$XLM")) extern const PIMAGE_TLS_CALLBACK TlsCallbackArray;

char _tls_start = 0;
char _tls_end = 0;
unsigned int _tls_index = 0;

const IMAGE_TLS_DIRECTORY _tls_used = {
  (ULONG_PTR)&_tls_start,
  (ULONG_PTR)&_tls_end,
  (ULONG_PTR)&_tls_index,
  (ULONG_PTR)(&__xl_a + 1),
};

static void _thread_cleanup();

VOID NTAPI _tls_callback(
  PVOID DllHandle,
  DWORD Reason,
  PVOID Reserved
  )
{
  switch(Reason) {
      case DLL_THREAD_ATTACH: break;
      case DLL_THREAD_DETACH: {
          _thread_cleanup();
      } break;
      case DLL_PROCESS_ATTACH: break;
      case DLL_PROCESS_DETACH: break;
  }
  // __debugbreak();
}

const PIMAGE_TLS_CALLBACK TlsCallbackArray = { &_tls_callback };

// NOTE: debug mutexes will follow the recursive logic but error if they
// actually recurse, this is slower than doing plain logic but it helps
// debug weird mutex errors.
//
// Based on these posts:
//   https://preshing.com/20120305/implementing-a-recursive-mutex/
//   https://preshing.com/20120226/roll-your-own-lightweight-mutex/

typedef struct UserClosure {
    thrd_start_t func;
    void* arg;
} UserClosure;

static DWORD _thread_call_user(void* arg) {
    UserClosure info = *((UserClosure*) arg);
    int result = info.func(info.arg);
    return (DWORD) result;
}

thrd_t thrd_current(void) {
    return (thrd_t){ GetCurrentThread() };
}

int thrd_create(thrd_t *thr, thrd_start_t func, void *arg) {
    UserClosure* info = malloc(sizeof(UserClosure));
    if (info == NULL) {
        return thrd_nomem;
    }

    info->func = func;
    info->arg = arg;

    // technically thrd_start_t and LPTHREAD_START_ROUTINE aren't the same
    // but are close enough to be ABI compatible, namely a difference in
    // signedness of the return val.
    thr->handle = CreateThread(NULL, 0, _thread_call_user, info, 0, NULL);
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
    _thread_cleanup();
    ExitThread((DWORD)res);
    __builtin_unreachable();
}

// TSS functions

#define TSS_KEYS_MAX 1088

static tss_dtor_t _tss_dtors[TSS_KEYS_MAX];
static bool _tss_init[TSS_KEYS_MAX];

static void _thread_cleanup() {
    for(int i = 0; i != TSS_DTOR_ITERATIONS; ++i) {
        for(unsigned k = 1; k != TSS_KEYS_MAX; ++k) {
            if(!_tss_init[k]) {
                continue;
            }
            void *data = TlsGetValue(k);
            if(data == NULL) {
                continue;
            }
            TlsSetValue(k, NULL);
            if(_tss_dtors[k]) {
                _tss_dtors[k](data);
            }
        }
    }
}

int tss_create(tss_t *key, tss_dtor_t dtor) {
    DWORD tls_index = TlsAlloc();
    if(tls_index == TLS_OUT_OF_INDEXES) {
        return thrd_error;
    }
    key->tls_index = tls_index;
    if(tls_index >= TSS_KEYS_MAX) {
        __debugbreak();
        TlsFree(tls_index);
        return thrd_error;
    }
    _tss_init[tls_index] = true;
    _tss_dtors[tls_index] = dtor;
    return thrd_success;
}

void tss_delete(tss_t key) {
    _tss_init[key.tls_index] = false;
    _tss_dtors[key.tls_index] = NULL;
    TlsFree(key.tls_index);
}

void *tss_get(tss_t key) {
    void *data = TlsGetValue(key.tls_index);
    if(data == NULL && GetLastError() != ERROR_SUCCESS) {
        return NULL;
    }
    return data;
}

int tss_set(tss_t key, void *val) {
   if(!TlsSetValue(key.tls_index, val)) {
       return thrd_error;
   }
   return thrd_success;
}

// Call once

static BOOL _call_once_trampoline(PINIT_ONCE init_once, PVOID param, PVOID *ctx) {
    void (*user_func)(void) = *ctx;
    user_func();
    return TRUE;
}

void call_once(once_flag *flag, void (*func)(void)) {
    InitOnceExecuteOnce((void *)flag, _call_once_trampoline, NULL, (void **)&func);
}

// Mutex functions

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
