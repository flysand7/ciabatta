

#pragma once

#include <cia/def.h>

struct _LD_CRT_Params typedef _LD_CRT_Params;
struct _LD_CRT_Params {
    u64 stack_size;
    u64 tls_image_size;
    void *tls_image_base;
};

// These signal a decision made by the main thread
// about whether a thread should be detached or joined
// If its not set, upon finishing the thread will wait
// until one of these flags is set.
// If the flag was set to detached, the thread will perform
// cleanup on its own after it reached the end of execution.
// If the flag was set to joined, it cannot perform the
// cleanup on its own. For if it did, the check on the state_finish
// futex might fail due to TCB being destroyed by the thread.
// Which is why we first need to make sure the thread stopped working
// and we don't need any of its TCB fields before cleaning it up on
// the main thread.
#define _LD_THREAD_STATE_NOT_YET  0x0
#define _LD_THREAD_STATE_DETACHED 0x1
#define _LD_THREAD_STATE_JOINED   0x2

struct _LD_Thread_Block typedef _LD_Thread_Block;
struct _LD_Thread_Block {
    /* DO NOT REORDER OR CHANGE SIZES (these comments are supposed to make it hard) */
    /* +0x00 */ i32 thread_id;
    /* +0x04 */ i32 state_finish;
    /* +0x08 */ _Atomic(u32) state_detach;
    /* +0x0c */ u32 exit_code;
    /* +0x10 */ u64 pad0;
    /* +0x18 */ u64 pad1;
    /* +0x20 */ u64 pad2;
    /* +0x28 */ u64 stack_canary;
    /* Not ABI dependent as far as I care (for now) */
    _LD_Thread_Block *next_tcb;
    _LD_Thread_Block *prev_tcb;
    _Atomic(u32) is_cancelled;
};

