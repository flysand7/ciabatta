
// Note(bumbread): this file is included into threads.h when compiled under
// windows.

#pragma once

typedef struct cnd_t {
    int idk_yet;
} cnd_t;

typedef struct thrd_t {
    void *handle;
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
