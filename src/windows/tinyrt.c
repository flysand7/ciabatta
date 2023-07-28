
// See src/tinyrt.h file for the interface this file implements

static _RT_Status _rt_file_std_handles_init() {
    _rt_file_stdin.handle = GetStdHandle(STD_INPUT_HANDLE);
    _rt_file_stdin.flags = _RT_FILE_READ;
    _rt_file_stdout.handle = GetStdHandle(STD_OUTPUT_HANDLE);
    _rt_file_stdout.flags = _RT_FILE_WRITE;
    _rt_file_stderr.handle = GetStdHandle(STD_ERROR_HANDLE);
    _rt_file_stdout.flags = _RT_FILE_WRITE;
    return _RT_STATUS_OK;
}

static _RT_Status _rt_file_open(_RT_File *file, char const *name, int _rt_flags) {
    u32 access = 0;
    if(_rt_flags & _RT_FILE_READ) access |= GENERIC_READ;
    if(_rt_flags & _RT_FILE_WRITE) access |= GENERIC_WRITE;
    u32 share = 0;
    if((_rt_flags & _RT_FILE_READ) == 0) share |= FILE_SHARE_READ;
    u32 creation = 0;
    if(_rt_flags & _RT_FILE_TRUNCATE) {
        creation = TRUNCATE_EXISTING;
    }
    else if(_rt_flags & _RT_FILE_CREATE) {
        if(_rt_flags & _RT_FILE_EXCLUSIVE) creation = CREATE_ALWAYS;
        else creation = CREATE_NEW;
    }
    else {
        if(_rt_flags & _RT_FILE_EXCLUSIVE) creation = OPEN_ALWAYS;
        else creation = OPEN_EXISTING;
    }
    HANDLE handle = CreateFileA(name, access, share, NULL, creation, FILE_ATTRIBUTE_NORMAL, NULL);
    if(file == INVALID_HANDLE_VALUE) {
        // ERROR_FILE_NOT_FOUND
        // ERROR_PATH_NOT_FOUND
        // ERROR_ACCESS_DENIED
        // ERROR_NOT_ENOUGH_MEMORY
        return _RT_STATUS_FILE_IO_ERROR;
        
    }
    file->handle = handle;
    file->flags = _rt_flags;
    return _RT_STATUS_OK;
}

static _RT_Status _rt_file_read(u64 size, void *buffer, _RT_File *from, u64 *out_bytes_read) {
    *out_bytes_read = 0;
    BOOL ok = ReadFile(from->handle, buffer, size, (unsigned long *)out_bytes_read, NULL);
    if(!ok) {
        return _RT_STATUS_FILE_IO_ERROR;
    }
    return _RT_STATUS_OK;
}

static _RT_Status _rt_file_write(_RT_File *to, u64 size, void *buffer, u64 *out_bytes_written) {
    *out_bytes_written = 0;
    BOOL ok = WriteFile(to->handle, buffer, size, (unsigned long *)out_bytes_written, NULL);
    if(!ok) {
        return _RT_STATUS_FILE_IO_ERROR;
    }
    return _RT_STATUS_OK;
}

static _RT_Status _rt_file_close(_RT_File *file) {
    BOOL ok = CloseHandle(file->handle);
    if(!ok) {
        return _RT_STATUS_FILE_BAD_FILE;
    }
    return _RT_STATUS_OK;
}

[[noreturn]] static void _rt_program_exit(int code) {
    ExitProcess(code);
    __builtin_unreachable();
}

