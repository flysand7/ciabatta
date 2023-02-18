
// Note(bumbread): this file is included into threads.h when compiled under
// windows.

#pragma once

#define ONCE_FLAG_INIT ((once_flag){0})

typedef struct thrd_t {
    void *handle;
} thrd_t;

typedef struct tss_t {
    unsigned tls_index;
} tss_t;

// We pretend that once_flag defined the same way as INIT_ONCE
// from winapi headers (aka _RTL_RUN_ONCE), which itself is defined
// as a union
typedef union once_flag {
    void *ptr;
} once_flag;

typedef struct cnd_t {
    int idk_yet;
} cnd_t;

typedef struct mtx_t {
    int type;
    // Done to handle recursive mutexes
    unsigned long recursion;
    unsigned long owner;
    _Atomic(int) counter;
    void* semaphore;
} mtx_t;
