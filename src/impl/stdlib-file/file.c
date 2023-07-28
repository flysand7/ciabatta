
FILE *stdin;
FILE *stdout;
FILE *stderr;

// TODO: memory allocation
static FILE _files[64];
static int _files_cnt = 0;

static void _fileapi_init() {
    _rt_file_std_handles_init();
    stdin = &_files[_files_cnt++];
    stdin->rt_file = _rt_file_stdin;
    stdout = &_files[_files_cnt++];
    stdout->rt_file = _rt_file_stdout;
    stderr = &_files[_files_cnt++];
    stderr->rt_file = _rt_file_stderr;
}

FILE *fopen(char const *restrict filename, char const *restrict mode) {
    _RT_File rt_file;
    int flags = 0;
    while(*mode) {
        if(*mode == 'r') flags |= _RT_FILE_READ;
        if(*mode == 'w') flags |= _RT_FILE_WRITE;
        // TODO: other flags
        mode += 1;
    }
    _RT_Status status = _rt_file_open(&rt_file, filename, flags);
    if(status != _RT_STATUS_OK) {
        return NULL;
    }
    FILE *file = &_files[_files_cnt];
    _files_cnt += 1;
    file->rt_file = rt_file;
    return file;
}

int fgetc(FILE *file) {
    int c = 0;
    u64 bytes_read;
    _RT_Status status = _rt_file_read(1, &c, &file->rt_file, &bytes_read);
    if(status == _RT_STATUS_FILE_EOF) {
        return EOF;
    }
    else if(status != _RT_STATUS_OK) {
        return EOF;
    }
    return c;
}

int fputc(int c, FILE *file) {
    u64 bytes_written;
    _RT_Status status = _rt_file_write(&file->rt_file, 1, &c, &bytes_written);
    if(status != _RT_STATUS_OK) {
        return EOF;
    }
    return c;
}

size_t fread(void *restrict buf, size_t size, size_t count, FILE *restrict file) {
    u64 bytes_read;
    _RT_Status status = _rt_file_read(size*count, buf, &file->rt_file, &bytes_read);
    if(status != _RT_STATUS_OK) {
        return 0;
    }
    return bytes_read / size;
}

size_t fwrite(void const *restrict buf, size_t size, size_t count, FILE *restrict file) {
    u64 bytes_written;
    _RT_Status status = _rt_file_write(&file->rt_file, size*count, (void *)buf, &bytes_written);
    if(status != _RT_STATUS_OK) {
        return 0;
    }
    return bytes_written / size;
}

int fclose(FILE *file) {
    _RT_Status status = _rt_file_close(&file->rt_file);
    if(status != _RT_STATUS_OK) {
        return EOF;
    }
    return 0;
}