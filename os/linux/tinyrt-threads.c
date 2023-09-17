
static _LD_Thread_Block *thread_blocks_head = NULL;

extern i64 _rt_thread_start(
    u64 flags,
    void *stack_base,
    int *parent_tid,
    int *child_tid,
    void *tls,
    int (*thread_fn)(void *ctx),
    void *ctx
);

static void thread_sigcancel_handler(int sig, siginfo_t *info, void *ucontext) {
    _LD_Thread_Block *tcb = (void *)((u64)__builtin_frame_address(0) & ~(cia_stack_size - 1));
    u32 is_cancelled = atomic_load_explicit(&tcb->is_cancelled, memory_order_acquire);
    if(is_cancelled) {
        u32 tgid = sys_getpid();
        sys_exit(0);
    }
}

static void _rt_threads_setup() {
    u64 sigs[3] = {(1ul << SIGCANCEL)};
    struct sigaction handler = {
        .sa_sigaction = thread_sigcancel_handler,
        .sa_flags = SA_SIGINFO|SA_RESTART,
        .sa_mask = 0xfffffffc7ffffffful,
    };
    syscall(SYS_rt_sigaction, SIGCANCEL, &handler, NULL, 1 * sizeof(u64));
}

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
    // Find the TCB base and initialize the TCB
    _LD_Thread_Block *tcb = (void *)((u64)((u8 *)stack - 1) & ~(cia_stack_size - 1));
    u8 *tls_base = (u8 *)tcb - cia_tls_image_size;
    for(int i = 0; i < cia_tls_image_size; ++i) {
        tls_base[i] = ((u8 *)cia_tls_image_base)[i];
    }
    tcb->stack_canary = 0x12345678deadbeef;
    tcb->next_tcb = NULL;
    tcb->prev_tcb = thread_blocks_head;
    atomic_store_explicit(&tcb->is_cancelled, 0, memory_order_relaxed);
    if(thread_blocks_head != NULL) {
        thread_blocks_head->next_tcb = tcb;
    }
    thread_blocks_head = tcb;
    // This futex is reset on thread finish
    tcb->state_finish = 1;
    // Initialize the _RT_Thread handle, which would point to the TCB
    thread->handle = tcb;
    // Create the new thread
    u64 flags = 0;
    flags |= CLONE_CHILD_CLEARTID;
    flags |= CLONE_PARENT_SETTID;
    flags |= CLONE_FS;
    flags |= CLONE_FILES;
    flags |= CLONE_SIGHAND;
    flags |= CLONE_THREAD;
    flags |= CLONE_VM;
    flags |= CLONE_SYSVSEM;
    i64 ret = _rt_thread_start(flags, stack, &tcb->thread_id, &tcb->state_finish, 0, thread_fn, ctx);
    if(ret < 0) {
        return _RT_ERROR_GENERIC;
    }
    return _RT_STATUS_OK;
}

int _rt_thread_setup(int (*thread_fn)(void *ctx), void *ctx) {
    // struct sigaction handler = {
    //     .sa_sigaction = thread_sigcancel_handler,
    //     .sa_flags = SA_SIGINFO|SA_RESTART,
    //     .sa_mask = 0,
    // };
    // i32 result = sys_sigaction(SIGCANCEL, &handler, NULL);
    return thread_fn(ctx);
}

void _rt_thread_finish(int exit_code) {
    _LD_Thread_Block *tcb = (void *)((u64)__builtin_frame_address(0) & ~(cia_stack_size - 1));
    // Wait until the main thread decides what to do with the child thread
    u32 thread_state = atomic_load_explicit(&tcb->state_detach, memory_order_relaxed);
    while(thread_state == _LD_THREAD_STATE_NOT_YET) {
        syscall(SYS_futex, &tcb->state_detach, FUTEX_WAIT, _LD_THREAD_STATE_NOT_YET, NULL, 0, 0);
        thread_state = atomic_load_explicit(&tcb->state_detach, memory_order_relaxed);
    }
    if(_LD_THREAD_STATE_DETACHED) {
        // TODO: clean up the thread resources
    }
    tcb->exit_code = exit_code;
    sys_exit(exit_code);
}

static _RT_Status _rt_thread_join(_RT_Thread *thread, int *out_exit_code) {
    _LD_Thread_Block *tcb = thread->handle;
    // Signal the thread that we want it to be joined
    atomic_store_explicit(&tcb->state_detach, _LD_THREAD_STATE_JOINED, memory_order_relaxed);
    syscall(SYS_futex, &tcb->state_detach, FUTEX_WAKE, 0, NULL, 0, 0);
    // Wait until the thread signals that it has completed the execution
    while(tcb->state_finish != 0) {
        syscall(SYS_futex, &tcb->state_finish, FUTEX_WAIT, 0, NULL, 0, 0);
    }
    // Set the exit code
    // NOTE(bumbread): this is not a bug, because calling thrd_detach from one thread
    // and thrd_join on the same thrd_t, from a different thread is supposed to be UB.
    *out_exit_code = tcb->exit_code;
    // TODO(bumbread): thread cleanup: destroy the TCB and thread-local storage
    return _RT_STATUS_OK;
}

static _RT_Status _rt_thread_detach(_RT_Thread *thread) {
    _LD_Thread_Block *tcb = thread->handle;
    atomic_store_explicit(&tcb->state_detach, _LD_THREAD_STATE_DETACHED, memory_order_relaxed);
    return _RT_STATUS_OK;
}

static _RT_Status _rt_thread_yield() {
    i64 status = syscall(SYS_sched_yield);
    if(status != 0) {
        // shouldn't happen on linux
        return _RT_ERROR_GENERIC;
    }
    return _RT_STATUS_OK;
}

static _RT_Status _rt_thread_terminate(_RT_Thread *thread) {
    u32 tgid = sys_getpid();
    _LD_Thread_Block *tcb = thread->handle;
    atomic_store_explicit(&tcb->is_cancelled, 1, memory_order_release);
    sys_tkill(tcb->thread_id, SIGCANCEL);
    // syscall(SYS_rt_tgsigqueueinfo, tgid, tcb->thread_id, SIGCANCEL, NULL);
    return _RT_STATUS_OK;
}

static _RT_Status _rt_thread_sleep(u64 time) {
    return _RT_ERROR_NOT_IMPLEMENTED;
}

static _RT_Status _rt_thread_get_timer_freq(u64 *freq) {
    return _RT_ERROR_NOT_IMPLEMENTED;
}

static _RT_Status _rt_thread_cancell_all_running() {
    u32 tgid = sys_getpid();
    _LD_Thread_Block *tcb_cur = thread_blocks_head;
    while(tcb_cur != NULL) {
        _LD_Thread_Block *tcb_next = tcb_cur->next_tcb;
        u32 thread_id = tcb_cur->thread_id;
        atomic_store_explicit(&tcb_cur->is_cancelled, 1, memory_order_release);
        //sys_tgkill(tgid, thread_id, SIGCANCEL);
        //SYS_rt_tgsigqueueinfo(tgid)
        tcb_cur = tcb_next;
    }
    return _RT_STATUS_OK;
}
