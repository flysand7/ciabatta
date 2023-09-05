
#pragma once

#include <stdatomic.h>

int cia_wait(u32 *addr, u32 compare_with, u64 time);
int cia_wake_one(u32 *addr, u32 *n_woken);
int cia_wake_all(u32 *addr, u32 *n_woken);

struct Cia_Mutex typedef Cia_Mutex;
struct Cia_Mutex {
    u32 tag;
};

void cia_mutex_init(Cia_Mutex *mutex);
void cia_mutex_lock(Cia_Mutex *mutex);
void cia_mutex_unlock(Cia_Mutex *mutex);

