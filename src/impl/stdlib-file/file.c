
FILE *stdin;
FILE *stdout;
FILE *stderr;

static Cia_Pool _page_allocator;
static Cia_Pool _file_pool;

static void _fileapi_init() {
    cia_pool_create(&_file_pool, cia_allocator_pages(), 0x1000, sizeof(FILE), 16);
    FILE *stdin = cia_pool_alloc(&_file_pool);
    FILE *stdout = cia_pool_alloc(&_file_pool);
    FILE *stderr = cia_pool_alloc(&_file_pool);
    _rt_file_std_handles_init();
    stdin->rt_file = _rt_file_stdin;
    stdout->rt_file = _rt_file_stdout;
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
    FILE *file = cia_pool_alloc(&_file_pool);
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
    cia_pool_free(&_file_pool, file);
    return 0;
}
