
// See src/tinyrt.h file for the interface this file implements

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
    i64 fd = syscall_open(name, flags, mode);
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
    i64 bytes_read = syscall_read(from->fd, buffer, size);
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
    i64 status = syscall_write(to->fd, buffer, size);
    if(-status == EBADF) return _RT_ERROR_BAD_PARAM;
    if(-status == EIO) return _RT_STATUS_FILE_IO_ERROR;
    if(-status > 0) return _RT_STATUS_FILE_IO_ERROR;
    *out_bytes_written = status;
    return _RT_STATUS_OK;
}

static _RT_Status _rt_file_close(_RT_File *file) {
    i64 result = syscall_close(file->fd);
    if(result < 0) {
        return _RT_STATUS_FILE_IO_ERROR;
    }
    return _RT_STATUS_OK;
}

_Noreturn static void _rt_program_exit(int code) {
    syscall_exit(code);
}

static _RT_Status _rt_mem_alloc(void *optional_desired_addr, u64 size, void **out_addr) {
    void *addr = syscall_mmap((u64)optional_desired_addr, size, PROT_READ|PROT_WRITE, MAP_ANONYMOUS, -1, 0);
    if(addr == NULL) {
        return _RT_ERROR_GENERIC;
    }
    *out_addr = addr;
    return _RT_STATUS_OK;
}

static _RT_Status _rt_mem_free(void *ptr, u64 len) {
    i64 result = syscall_munmap(ptr, len);
    if(result == -1) {
        return _RT_ERROR_GENERIC;
    }
    return _RT_STATUS_OK;
}

