#pragma once

#if defined(_WIN32)
    #define _Thread_local __declspec(thread)
#endif

#define thread_local _Thread_local

#define ONCE_FLAG_INIT      1
#define TSS_DTOR_ITERATIONS 32

#if !defined(_timespec_defined)
    #define _timespec_defined
    typedef unsigned long long time_t;
    struct timespec {
        time_t tv_sec;
        long   tv_nsec;
    };
#endif

#if defined(_WIN32)
    typedef struct cnd_t {
    	int idk_yet;
    } cnd_t;

    typedef struct thrd_t {
        void* handle;
    } thrd_t;

    typedef struct tss_t {
    	int idk_yet;
    } tss_t;

    typedef struct mtx_t {
        int type;
        // Done to handle recursive mutexes
        unsigned long recursion;
        unsigned long owner;
        _Atomic(int) counter;
        void* semaphore;
    } mtx_t;
#else
    #error "Not implemented"
#endif

typedef void(*tss_dtor_t)  (void*);
typedef int (*thrd_start_t)(void*);
typedef int once_flag; // TODO: change this maybe?

enum {
    mtx_plain     = 0,
    mtx_recursive = 1,
    mtx_timed     = 2,
};

enum {
	thrd_success,
	thrd_timedout,
	thrd_busy,
	thrd_error,
	thrd_nomem
};

void call_once(once_flag *flag, void (*func)(void));

int  cnd_init     (cnd_t *cond);
int  cnd_broadcast(cnd_t *cond);
void cnd_destroy  (cnd_t *cond);
int  cnd_signal   (cnd_t *cond);
int  cnd_wait     (cnd_t *cond, mtx_t *mtx);
int  cnd_timedwait(
    cnd_t *restrict cond,
    mtx_t *restrict mtx,
    const struct timespec *restrict ts
);

void   mtx_destroy  (mtx_t *mtx);
int    mtx_init     (mtx_t *mtx, int type);
int    mtx_unlock   (mtx_t *mtx);
int    mtx_trylock  (mtx_t *mtx);
int    mtx_lock     (mtx_t *mtx);
int    mtx_timedlock(
    mtx_t *restrict mtx,
    const struct timespec *restrict ts
);

thrd_t thrd_current(void);
int    thrd_create (thrd_t *thr, thrd_start_t func, void *arg);
int    thrd_detach (thrd_t thr);
int    thrd_equal  (thrd_t thr0, thrd_t thr1);
int    thrd_join   (thrd_t thr, int *res);
void   thrd_yield  (void);
int thrd_sleep(
    const struct timespec *duration,
    struct timespec *remaining
);
_Noreturn void thrd_exit(int res);

int   tss_create(tss_t *key, tss_dtor_t dtor);
void  tss_delete(tss_t  key);
void *tss_get   (tss_t  key);
int   tss_set   (tss_t  key, void *val);
