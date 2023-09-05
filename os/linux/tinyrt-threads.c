
extern i64 _cia_start_thread(
    u64 flags,
    void *stack_base,
    int *parent_tid,
    int *child_tid,
    void *tls,
    int (*thread_fn)(void *ctx),
    void *ctx
);

static _RT_Status _rt_thread_current(_RT_Thread *thread) {
    thread->handle = (void *)((u64)__builtin_frame_address(0) & ~(cia_stack_size - 1));
    return _RT_STATUS_OK;
}

static _RT_Status _rt_thread_create(_RT_Thread *thread, int (*thread_fn)(void *ctx), void *ctx) {
    // Create the memory for stack
    u64 mmap_prot = PROT_READ|PROT_WRITE;
    u64 mmap_flags = MAP_PRIVATE|MAP_ANONYMOUS|MAP_NORESERVE;
    void *stack_base = sys_mmap(0, 2*cia_stack_size, mmap_prot, mmap_flags, -1, 0);
    if((i64)stack_base < 0) {
        return _RT_ERROR_GENERIC;
    }
    void *stack = (u8*)stack_base + 2*cia_stack_size;
    // Find the TLS base and initialize the tls
    Cia_TCB *tcb = (void *)((u64)((u8 *)stack - 1) & ~(cia_stack_size - 1));
    tcb->stack_canary = 0x12345678deadbeef;
    u8 *tls_base = (u8 *)tcb - cia_tls_image_size;
    for(int i = 0; i < cia_tls_image_size; ++i) {
        tls_base[i] = ((u8 *)cia_tls_image_base)[i];
    }
    // Initialize the _RT_Thread handle, which would point to
    // the TCB
    thread->handle = tcb;
    // Create the new thread
    u64 flags = 0;
    // flags |= CLONE_CHILD_CLEARTID;
    // flags |= CLONE_PARENT_SETTID;
    flags |= CLONE_FS;
    flags |= CLONE_FILES;
    flags |= CLONE_SIGHAND;
    flags |= CLONE_THREAD;
    flags |= CLONE_VM;
    flags |= CLONE_SYSVSEM;
    int *child_tid = (int *)&tcb->thread_id;
    int *parent_tid = (int *)&tcb->parent_id;
    *child_tid = 1;
    *parent_tid = 0;
    i64 ret = _cia_start_thread(flags, stack, parent_tid, child_tid, 0, thread_fn, ctx);
    if(ret < 0) {
        return _RT_ERROR_GENERIC;
    }
    return _RT_STATUS_OK;
}

static _RT_Status _rt_thread_join(_RT_Thread *thread) {
    return _RT_ERROR_NOT_IMPLEMENTED;
}

static _RT_Status _rt_thread_detach(_RT_Thread *thread) {
    return _RT_ERROR_NOT_IMPLEMENTED;
}

static _RT_Status _rt_thread_terminate(_RT_Thread *thread) {
    return _RT_ERROR_NOT_IMPLEMENTED;
}

static _RT_Status _rt_thread_sleep(u64 time) {
    return _RT_ERROR_NOT_IMPLEMENTED;
}

static _RT_Status _rt_thread_get_timer_freq(u64 *freq) {
    return _RT_ERROR_NOT_IMPLEMENTED;
}
