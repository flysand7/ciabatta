
// See src/tinyrt.h file for the interface this file implements

extern i64 _cia_start_thread(
    u64 flags,
    void *stack_base,
    int *parent_tid,
    int *child_tid,
    void *tls,
    void (*thread_fn)(void *ctx),
    void *ctx
);

_Noreturn static void _rt_program_exit(int code) {
    sys_exit(code);
}

static _RT_Status _rt_thread_current(_RT_Thread *thread) {
    return _RT_ERROR_NOT_IMPLEMENTED;
}

static _RT_Status _rt_thread_create(_RT_Thread *thread, void (*thread_fn)(void *ctx), void *ctx) {
    // Create the memory for stack
    u64 mmap_prot = PROT_READ|PROT_WRITE;
    u64 mmap_flags = MAP_PRIVATE|MAP_ANONYMOUS|MAP_NORESERVE;
    u64 stack_size = 0x10000;
    void *stack_base = sys_mmap(0, stack_size, mmap_prot, mmap_flags, -1, 0);
    if((i64)stack_base < 0) {
        return _RT_ERROR_GENERIC;
    }
    void *stack = (u8*)stack_base + stack_size;
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
    int *temp_permanent_storage = stack_base;
    int *child_tid = &temp_permanent_storage[0];
    int *parent_tid = &temp_permanent_storage[1];
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

static _RT_Status _rt_file_std_handles_init() {
    _rt_file_stdin.fd = 0;
    _rt_file_stdin.flags = _RT_FILE_READ;
    _rt_file_stdout.fd = 1;
    _rt_file_stdout.flags = _RT_FILE_WRITE;
    _rt_file_stderr.fd = 2;
    _rt_file_stdout.flags = _RT_FILE_WRITE;
    return _RT_STATUS_OK;
}

static _RT_Status _rt_file_open(_RT_File *file, char const *name, int _rt_flags) {
    if((_rt_flags & 0x3) == 0) {
        return _RT_ERROR_BAD_PARAM;
    }
    int mode = 0;
    int flags = 0;
    if((_rt_flags & 0x03) == 0x03)     mode = O_RDWR;
    else if(_rt_flags & _RT_FILE_READ)  mode = O_RDONLY;
    else if(_rt_flags & _RT_FILE_WRITE) mode = O_RDWR;
    if(_rt_flags & _RT_FILE_CREATE)     flags |= O_CREAT;
    if(_rt_flags & _RT_FILE_EXCLUSIVE)  flags |= O_EXCL;
    if(_rt_flags & _RT_FILE_TRUNCATE)   flags |= O_TRUNC;
    i64 fd = sys_open(name, flags, mode);
    if(-fd == EACCES)  return _RT_STATUS_FILE_ACCESS;
    if(-fd == EEXIST)  return _RT_STATUS_FILE_EXISTS;
    if(-fd == ENOENT)  return _RT_STATUS_FILE_NOT_EXISTS;
    if(-fd == EINVAL)  return _RT_ERROR_BAD_PARAM;
    if(-fd == EISDIR)  return _RT_STATUS_FILE_DIRECTORY;
    // I'm too lazy to fill in the rest so lets leave it at that for now
    if(fd < 0) return _RT_STATUS_FILE_IO_ERROR;
    file->fd = (u64)fd;
    file->flags = _rt_flags;
    return _RT_STATUS_OK;
}

static _RT_Status _rt_file_read(u64 size, void *buffer, _RT_File *from, u64 *out_bytes_read) {
    i64 bytes_read = sys_read(from->fd, buffer, size);
    if(bytes_read == 0) {
        return _RT_STATUS_FILE_EOF;
    }
    if(bytes_read < 0) {
        return _RT_STATUS_FILE_IO_ERROR;
    }
    *out_bytes_read = bytes_read;
    return _RT_STATUS_OK;
}

static _RT_Status _rt_file_write(_RT_File *to, u64 size, void *buffer, u64 *out_bytes_written) {
    i64 status = sys_write(to->fd, buffer, size);
    if(-status == EBADF) return _RT_ERROR_BAD_PARAM;
    if(-status == EIO) return _RT_STATUS_FILE_IO_ERROR;
    if(-status > 0) return _RT_STATUS_FILE_IO_ERROR;
    *out_bytes_written = status;
    return _RT_STATUS_OK;
}

static _RT_Status _rt_file_close(_RT_File *file) {
    i64 result = sys_close(file->fd);
    if(result < 0) {
        return _RT_STATUS_FILE_IO_ERROR;
    }
    return _RT_STATUS_OK;
}

static _RT_Status _rt_mem_alloc(void *optional_desired_addr, u64 size, void **out_addr) {
    void *addr = sys_mmap((u64)optional_desired_addr, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
    if(addr == NULL) {
        return _RT_ERROR_GENERIC;
    }
    *out_addr = addr;
    return _RT_STATUS_OK;
}

static _RT_Status _rt_mem_free(void *ptr, u64 len) {
    i64 result = sys_munmap(ptr, len);
    if(result == -1) {
        return _RT_ERROR_GENERIC;
    }
    return _RT_STATUS_OK;
}

static _RT_Status _rt_sync_wait(u32 *addr, u32 compare_with, u64 sleep) {
    i64 result = syscall(SYS_futex, addr, FUTEX_WAIT_PRIVATE, compare_with, NULL, 0, 0);
    if(-result == EAGAIN) {
        return _RT_STATUS_OK;
    }
    else if(-result > 0) {
        return _RT_ERROR_GENERIC;
    }
    return _RT_STATUS_OK;
}

static _RT_Status _rt_sync_wake_one(u32 *addr, u32 *n_woken) {
    i64 result = syscall(SYS_futex, addr, FUTEX_WAKE_PRIVATE, 1, NULL, 0, 0);
    if(result < 0) {
        return _RT_ERROR_GENERIC;
    }
    *n_woken = (u32)result;
    return _RT_STATUS_OK;
}

static _RT_Status _rt_sync_wake_all(u32 *addr, u32 *n_woken) {
    i64 result = syscall(SYS_futex, addr, FUTEX_WAKE_PRIVATE, 0x7fffffff, NULL, 0, 0);
    if(result < 0) {
        return _RT_ERROR_GENERIC;
    }
    *n_woken = (u32)result;
    return _RT_STATUS_OK;
}
