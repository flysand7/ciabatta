
#define _CIA_MUTEX_TAG_UNLOCKED 0x00000000
#define _CIA_MUTEX_TAG_LOCKED   0x00000001

void cia_mutex_init(Cia_Mutex *mutex) {
    mutex->tag = _CIA_MUTEX_TAG_UNLOCKED;
}

void cia_mutex_lock(Cia_Mutex *mutex) {
    do {
        _rt_sync_wait(&mutex->tag, _CIA_MUTEX_TAG_LOCKED, _RT_SYNC_WAIT_INFINITE);
    } while(mutex->tag == _CIA_MUTEX_TAG_LOCKED);
    mutex->tag = _CIA_MUTEX_TAG_LOCKED;
}

void cia_mutex_unlock(Cia_Mutex *mutex) {
    mutex->tag = _CIA_MUTEX_TAG_UNLOCKED;
    u32 woken = 0;
    _rt_sync_wake_one(&mutex->tag, &woken);
}

