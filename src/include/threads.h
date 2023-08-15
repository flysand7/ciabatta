
#pragma once

typedef int (*thrd_start_t)(void *);

struct thrd_t typedef thrd_t;
struct thrd_t {
    _RT_Thread thread;
};

enum {
    thrd_success = 0,
    thrd_nomem = -1,
    thrd_timedout = -2,
    thrd_busy = -3,
    thrd_error = -4
};

int thrd_create(thrd_t *thr, thrd_start_t func, void *arg);
