
#define _CIA_MUTEX_FREE 0
#define _CIA_MUTEX_LOCK 1
#define _CIA_MUTEX_CONT 2

void cia_mutex_init(Cia_Mutex *mutex) {
    atomic_store_explicit(&mutex->tag, _CIA_MUTEX_FREE, memory_order_relaxed);
}

void cia_mutex_lock(Cia_Mutex *mutex) {
    u32 p_tag;
    for(;;) {
        //p_tag = __sync_val_compare_and_swap(&mutex->tag, _CIA_MUTEX_FREE, _CIA_MUTEX_LOCK);
        p_tag = _CIA_MUTEX_FREE;
        atomic_compare_exchange_strong_explicit(
            &mutex->tag
            , &p_tag
            , _CIA_MUTEX_LOCK
            , memory_order_relaxed
            , memory_order_relaxed
        );
        // We got the mutex, lets bail
        if(p_tag == _CIA_MUTEX_FREE) {
            break;
        }
#if 0
        // We should wait if:
        //   (1) the mutex is contested
        //   (2) 
        bool should_wait = 0;
        should_wait |= (p_tag == _CIA_MUTEX_CONT);
        should_wait |= (__sync_val_compare_and_swap(&mutex->tag, _CIA_MUTEX_LOCK, _CIA_MUTEX_CONT) != _CIA_MUTEX_FREE);
        // We wait while its contested
        if(should_wait) {
            _rt_sync_wait(&mutex->tag, _CIA_MUTEX_CONT, _RT_SYNC_WAIT_INFINITE);
        }
#else
        _rt_sync_wait(&mutex->tag, _CIA_MUTEX_LOCK, _RT_SYNC_WAIT_INFINITE);
#endif
    }
}

void cia_mutex_unlock(Cia_Mutex *mutex) {
    // TODO: add error when we unlock a free mutex
    // TODO: support recursive muteces
    atomic_store_explicit(&mutex->tag, _CIA_MUTEX_FREE, memory_order_relaxed);
    _rt_sync_wake_one(&mutex->tag, NULL);
}
