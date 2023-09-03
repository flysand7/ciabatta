
#pragma once

#include <cia/def.h>
#include <stdint.h>

struct Cia_CRT_Params typedef Cia_CRT_Params;
struct Cia_CRT_Params {
    u64 stack_size;
    u64 tls_image_size;
    void *tls_image_base;
};

struct Cia_TCB typedef Cia_TCB;
struct Cia_TCB {
    u64 thread_id;
    u64 parent_id;
    u64 pad0[3];
    u64 stack_canary;
    u64 pad1[2];
};


