
#pragma once

#include <cia/def.h>
#include <stdint.h>

struct Cia_CRT_Params typedef Cia_CRT_Params;
struct Cia_CRT_Params {
    u64 stack_size;
    u64 tls_image_size;
    void *tls_image_base;
};

// These control how the thread should behave once it's terminated
// if the main thread didn't call join nor detach, the thread
// would just wait until one of them is set.
// If the thread is joined,
// would signal main thread that it has finished and exit without
// cleaning up the resources (thrd_join does that for us)
// If the thread is detached, it would clear it's resources and exit
#define _CIA_THREAD_BEHAVIOUR_NOT_SET 0x0
#define _CIA_THREAD_BEHAVIOUR_JOIN    0x1
#define _CIA_THREAD_BEHAVIOUR_DETACH  0x2
#define _CIA_THREAD_BEHAVIOUR_FINISH  0x3

struct Cia_TCB typedef Cia_TCB;
struct Cia_TCB {
    /* +0x00 */ u64 thread_id;
    /* +0x08 */ u64 parent_id;
    /* +0x10 */ u32 thread_behaviour; /* One of the CIA_THREAD_BEHAVIOR_* constants */
    /* +0x14 */ u32 thread_finished;
    /* +0x18 */ u32 pad0;
    /* +0x1c */ i32 exit_code;
    /* +0x20 */ u64 pad1;
    /* +0x28 */ u64 stack_canary;
};


