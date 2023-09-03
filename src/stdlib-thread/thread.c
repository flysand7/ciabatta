
int thrd_create(thrd_t *thr, thrd_start_t func, void *arg) {
    _RT_Status status = _rt_thread_create(&thr->thread, (int (*)(void *))func, arg);
    if(status == _RT_STATUS_OK) {
        return thrd_success;
    }
    return thrd_error;
}