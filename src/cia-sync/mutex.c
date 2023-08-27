
#define _CIA_MUTEX_FREE 0
#define _CIA_MUTEX_LOCK 1
#define _CIA_MUTEX_CONT 2

void cia_mutex_init(Cia_Mutex *mutex) {
    mutex->tag = _CIA_MUTEX_FREE;
}

void cia_mutex_lock(Cia_Mutex *mutex) {
    u64 prev_tag;
    for(;;) {
        prev_tag = __sync_val_compare_and_swap(&mutex->tag, _CIA_MUTEX_FREE, _CIA_MUTEX_LOCK);
        // We got the mutex, lets bail
        if(prev_tag == _CIA_MUTEX_FREE) {
            break;
        }
        // We should wait if:
        //   (1) the mutex is contested
        //   (2) this thread locking the mutex makes it contested
        bool should_wait = 0;
        should_wait |= (prev_tag == _CIA_MUTEX_CONT);
        should_wait |= (__sync_val_compare_and_swap(&mutex->tag, _CIA_MUTEX_LOCK, _CIA_MUTEX_CONT) != _CIA_MUTEX_FREE);
        // We wait while its contested
        if(should_wait) {
            _rt_sync_wait(&mutex->tag, _CIA_MUTEX_CONT, _RT_SYNC_WAIT_INFINITE);
        }
    }
}

void cia_mutex_unlock(Cia_Mutex *mutex) {
    // TODO: add error when we unlock a free mutex
    // TODO: support recursive muteces
    mutex->tag = _CIA_MUTEX_FREE;
    _rt_sync_wake_one(&mutex->tag, NULL);
}
