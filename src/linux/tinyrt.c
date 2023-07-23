
// See src/tinyrt.h file for the interface this file implements

static RT_Status rt_file_open(RT_File *file, char *name, int rt_flags) {
    if((rt_flags & 0x3) == 0) {
        return RT_ERROR_BAD_PARAM;
    }
    int mode = 0;
    int flags = 0;
    if((rt_flags & 0x03) == 0x03)     mode = O_RDWR;
    else if(rt_flags & RT_FILE_READ)  mode = O_RDONLY;
    else if(rt_flags & RT_FILE_WRITE) mode = O_RDWR;
    if(rt_flags & RT_FILE_CREATE)     flags |= O_CREAT;
    if(rt_flags & RT_FILE_EXCLUSIVE)  flags |= O_EXCL;
    if(rt_flags & RT_FILE_TRUNCATE)   flags |= O_TRUNC;
    i64 fd = syscall_open(name, flags, mode);
    if(-fd == EACCES)  return RT_STATUS_FILE_ACCESS;
    if(-fd == EEXIST)  return RT_STATUS_FILE_EXISTS;
    if(-fd == ENOENT)  return RT_STATUS_FILE_NOT_EXISTS;
    if(-fd == EINVAL)  return RT_ERROR_BAD_PARAM;
    if(-fd == EISDIR)  return RT_STATUS_FILE_DIRECTORY;
    // I'm too lazy to fill in the rest so lets leave it at that for now
    if(fd < 0) return RT_STATUS_FILE_IO_ERROR;
    file->fd = (u64)fd;
    file->flags = rt_flags;
    return RT_STATUS_OK;
}

static RT_Status rt_file_read(u64 size, void *buffer, RT_File *from, u64 *out_bytes_read) {
    i64 bytes_read = syscall_read(from->fd, buffer, size);
    if(bytes_read == 0) {
        return RT_STATUS_FILE_EOF;
    }
    if(bytes_read < 0) {
        return RT_STATUS_FILE_IO_ERROR;
    }
    *out_bytes_read = bytes_read;
    return RT_STATUS_OK;
}

static RT_Status rt_file_write(RT_File *to, u64 size, void *buffer, u64 *out_bytes_written) {
    i64 bytes_written = syscall_write(to->fd, buffer, size);
    if(bytes_written < 0) {
        return RT_STATUS_FILE_IO_ERROR;
    }
    *out_bytes_written = bytes_written;
    return RT_STATUS_OK;
}

static RT_Status rt_file_close(RT_File *file) {
    i64 result = syscall_close(file->fd);
    if(result < 0) {
        return RT_STATUS_FILE_IO_ERROR;
    }
    return RT_STATUS_OK;
}
