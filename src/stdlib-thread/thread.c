
int thrd_create(thrd_t *thr, thrd_start_t func, void *arg) {
    _RT_Status status = _rt_thread_create(&thr->thread, (int (*)(void *))func, arg);
    if(status == _RT_STATUS_OK) {
        return thrd_success;
    }
    return thrd_error;
}

int thrd_detach(thrd_t thr) {
    _RT_Status status = _rt_thread_detach(&thr.thread);
    if(status == _RT_STATUS_OK) {
        return thrd_success;
    }
    return thrd_error;
}

int thrd_join(thrd_t thr, int *out_exit_code) {
    _RT_Status status = _rt_thread_join(&thr.thread, out_exit_code);
    if(status == _RT_STATUS_OK) {
        return thrd_success;
    }
    return thrd_error;
}

void thrd_yield(void) {
    _rt_thread_yield();
}

/* remove later */
void thrd_terminate(thrd_t thr) {
    _rt_thread_terminate(&thr.thread);
}