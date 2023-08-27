
int cia_wait(u32 *addr, u32 compare_with, u64 time) {
    _rt_sync_wait(addr, compare_with, time);
}

int cia_wake_one(u32 *addr, u32 *n_woken) {
    _rt_sync_wake_one(addr, n_woken);
}

int cia_wake_all(u32 *addr, u32 *n_woken) {
    _rt_sync_wake_all(addr, n_woken);
}

