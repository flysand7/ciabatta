
enum FileMode {
    MODE_INPUT,
    MODE_OUTPUT,
    MODE_UPDATE,
} typedef FileMode;

enum FileType {
    FILE_BINARY,
    FILE_TEXT,
} typedef FileType;

struct FileBuffer typedef FileBuffer;
struct FileBuffer {
    int is_internal;
    int mode;
    size_t size;
    void  *data;
    size_t written;
};

struct FILE {
    HANDLE handle;
    mbstate_t mbstate;
    FileBuffer buffer;
    FileMode io_mode;
    FileType bt_mode;
    int eof;
    int err;
    mtx_t lock;
    bool temp;
    char *temp_name;
    FILE *prev;
    FILE *next;
};

FILE *stdout;
FILE *stdin;
FILE *stderr;

// We hold a linked list of all file streams in order to flush all the buffers
// after the program terminates. It might be not a good idea to store all the
// files into this linked list, but for now performance is not a concern.

static FILE *_file_tracker = NULL;

static void _file_track(FILE *stream) {
    if(_file_tracker != NULL) {
        _file_tracker->next = stream;
        stream->prev = _file_tracker;
        stream->next = NULL;
        _file_tracker = stream;
    }
    else {
        _file_tracker = stream;
        stream->prev = NULL;
        stream->next = NULL;
    }
}

static void _file_untrack(FILE *stream) {
    FILE *prev = stream->prev;
    FILE *next = stream->next;
    if(prev != NULL) prev->next = next;
    if(next != NULL) next->prev = prev;
    if(next == NULL) _file_tracker = prev;
}

// Multithreaded access

static inline void _file_lock_init(FILE *stream) {
    mtx_init(&stream->lock, mtx_plain);
}

static inline void _file_lock_destroy(FILE *stream) {
    mtx_destroy(&stream->lock);
}

static inline void _file_lock(FILE *stream) {
    mtx_lock(&stream->lock);
}

static inline void _file_unlock(FILE *stream) {
    mtx_unlock(&stream->lock);
}

// Managing file handles

static inline FILE *_file_create(HANDLE handle, FileMode io_mode, FileType bt_mode) {
    FILE *stream = calloc(1, sizeof(FILE));
    if(stream == NULL) {
        return NULL;
    }
    stream->handle = handle;
    stream->io_mode = io_mode;
    stream->bt_mode = bt_mode;
    _file_lock_init(stream);
    _file_track(stream);
    return stream;
}

static inline void _file_close(FILE *stream) {
    _file_lock(stream);
    CloseHandle(stream->handle);
    _file_untrack(stream);
    _file_unlock(stream);
    _file_lock_destroy(stream);
    free(stream);
}

static void _io_init() {
    HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hstderr = GetStdHandle(STD_ERROR_HANDLE);
    HANDLE hstdin  = GetStdHandle(STD_INPUT_HANDLE);

    stdout = _file_create(hstdout, MODE_UPDATE, FILE_TEXT);
    stderr = _file_create(hstderr, MODE_UPDATE, FILE_TEXT);
    stdin  = _file_create(hstdin,  MODE_INPUT,  FILE_BINARY);

    char *in_buf  = calloc(BUFSIZ, sizeof(char));
    char *out_buf = calloc(BUFSIZ, sizeof(char));
    stdin->buffer  = (FileBuffer){1, _IOLBF, BUFSIZ, in_buf};
    stdout->buffer = (FileBuffer){1, _IOLBF, BUFSIZ, out_buf};
    stderr->buffer = (FileBuffer){1, _IONBF,      0, NULL};

    SetConsoleCP(CP_UTF8); // Maybe it will work someday
    SetConsoleOutputCP(CP_UTF8);
}

static void _io_close() {
    while(_file_tracker != NULL) {
        FILE *stream = _file_tracker;
        fclose(stream);
        _file_tracker = _file_tracker->next;
    }
}

// File mode parsing

struct WindowsMode typedef WindowsMode;
struct WindowsMode {
    DWORD access;
    DWORD share;
    DWORD disp;
};

static int _mode_parse(char const *mode, FileMode *pio_mode, FileType *pbt_mode, WindowsMode *win_mode) {
    DWORD access = 0;
    DWORD share = 0;
    DWORD disp = 0;
    FileMode io_mode = 0;
    FileType bt_mode = 0;
    int flag_p = 0;
    int flag_b = 0;
    int flag_x = 0;
    switch(*mode++) {
        case 'r': io_mode = MODE_INPUT;  break;
        case 'w': io_mode = MODE_OUTPUT; break;
        case 'a': io_mode = MODE_UPDATE; break;
        default: return 0;
    }
    while(*mode) switch(*mode++) {
        case '+': flag_p = 1; break;
        case 'b': flag_b = 1; break;
        case 'x': flag_x = 1; break;
        default: return 0;
    }
    bt_mode = flag_b? FILE_BINARY : FILE_TEXT;
    switch(io_mode) {
        case MODE_INPUT: {
            access = GENERIC_READ | (flag_p? GENERIC_WRITE : 0);
            share  = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;
            disp   = OPEN_EXISTING;
        } break;
        case MODE_OUTPUT: {
            access = GENERIC_WRITE | (flag_p? GENERIC_READ : 0);
            share  = 0;
            disp   = CREATE_ALWAYS;
            if(flag_x) {
                disp = CREATE_NEW;
            }
        } break;
        case MODE_UPDATE: {
            access = GENERIC_READ | GENERIC_WRITE;
            share  = 0;
            disp   = OPEN_ALWAYS;
        } break;
    }
    win_mode->access = access;
    win_mode->share = share;
    win_mode->disp = disp;
    *pio_mode = io_mode;
    *pbt_mode = bt_mode;
    return 1;
}

// Operations on files

int remove(const char *filename) {
    BOOL ok = DeleteFileA(filename);
    return !ok;
}

int rename(const char *old, const char *new) {
    BOOL ok = MoveFileA(old, new);
    return !ok;
}

// Opening/closing files

char *tmpnam(char *s) {
    static char internal_buffer[L_tmpnam];
    char *buffer;
    if(s != NULL) {
        buffer = s;
    }
    else {
        buffer = &internal_buffer[0];
    }
    UINT num = GetTempFileNameA(".", ".t_", 0, buffer);
    if(num == 0) {
        return NULL;
    }
    else {
        return buffer;
    }
}

FILE *tmpfile(void) {
    char *file_name = malloc(L_tmpnam);
    UINT num = GetTempFileNameA(".", ".t_", 0, &file_name[0]);
    if(!num) {
        return NULL;
    }
    FILE *tmp_file = fopen(&file_name[0], "wb+");
    tmp_file->temp = true;
    tmp_file->temp_name = file_name;
    return tmp_file;
}

FILE *fopen(const char *restrict name, const char *restrict mode) {
    WindowsMode win_mode;
    DWORD flags = FILE_FLAG_WRITE_THROUGH;
    FileMode io_mode;
    FileType bt_mode;
    if(!_mode_parse(mode, &io_mode, &bt_mode, &win_mode)) {
        return NULL;
    }
    DWORD access = win_mode.access;
    DWORD share = win_mode.share;
    DWORD disp = win_mode.disp;
    HANDLE handle = CreateFileA(name, access, share, NULL, disp, flags, NULL);
    if(handle == INVALID_HANDLE_VALUE) {
        return NULL;
    }
    FILE *stream = _file_create(handle, io_mode, bt_mode);
    void *buffer_data = malloc(BUFSIZ);
    stream->buffer = (FileBuffer) {1, _IOFBF, BUFSIZ, buffer_data};
    return stream;
}

FILE *freopen(const char *restrict name, const char *restrict mode, FILE *restrict stream) {
    if(stream == NULL) {
        return NULL;
    }
    fflush(stream);
    WindowsMode win_mode;
    DWORD flags = FILE_FLAG_WRITE_THROUGH;
    FileMode io_mode;
    FileType bt_mode;
    if(!_mode_parse(mode, &io_mode, &bt_mode, &win_mode)) {
        return NULL;
    }
    DWORD access = win_mode.access;
    DWORD share = win_mode.share;
    DWORD disp = win_mode.disp;
    if(name == NULL) {
        HANDLE handle = ReOpenFile(stream->handle, access, share, flags);
        if(handle == INVALID_HANDLE_VALUE) {
            return NULL;
        }
    }
    else {
        CloseHandle(stream->handle);
        HANDLE handle = CreateFileA(name, access, share, NULL, disp, flags, NULL);
        if(handle == INVALID_HANDLE_VALUE) {
            return NULL;
        }
        *stream = (FILE) {0};
        stream->handle = handle;
        stream->io_mode = io_mode;
        stream->bt_mode = bt_mode;
    }
    return stream;
}

int fclose(FILE *stream) {
    if(fflush(stream) == EOF) {
        return EOF;
    }
    if(stream->buffer.is_internal) {
        free(stream->buffer.data);
    }
    if(!CloseHandle(stream->handle)) {
        return EOF;
    }
    if(stream->temp) {
        BOOL ok = DeleteFileA(stream->temp_name);
        free(stream->temp_name);
        if(!ok) {
            return EOF;
        }
    }
    _file_untrack(stream);
    return 0;
}

// Buffering

int setvbuf(FILE *restrict stream, char *restrict ptr, int mode, size_t size) {
    if(mode != _IOFBF && mode != _IOLBF && mode != _IONBF) {
        return 1;
    }
    _file_lock(stream);
    FileBuffer *buffer = &stream->buffer;
    buffer->mode = mode;
    if(ptr == NULL) {
        buffer->data = realloc(buffer->data, size);
        buffer->size = size;
    }
    else {
        buffer->data = ptr;
        buffer->size = size;
    }
    _file_unlock(stream);
    return 0;
}

void setbuf(FILE *restrict stream, char *restrict buf) {
    if(buf == NULL) {
        setvbuf(stream, NULL, _IONBF, 0);
    }
    else {
        setvbuf(stream, buf, _IOFBF, BUFSIZ);
    }
}

int fflush(FILE *stream) {
    _file_lock(stream);
    int res = 0;
    FileBuffer *buffer = &stream->buffer;
    if(buffer->written != 0) {
        size_t size = buffer->written;
        void *data = buffer->data;
        DWORD bytes_written;
        BOOL ok = WriteFile(stream->handle, data, size, &bytes_written, NULL);
        if(!ok) {
            res = EOF;
            stream->eof = 1;
        }
        buffer->written = 0;
    }
    _file_unlock(stream);
    return res;
}

// Errors

int feof(FILE *stream) {
    return stream->eof;
}

int ferror(FILE *stream) {
    return stream->err;
}

void clearerr(FILE *stream) {
    stream->eof = 0;
    stream->err = 0;
}

void perror(char const *s) {
    // printf("%s: %s\n", s, strerror(errno));
}

// File reads/writes

int fputc(int c, FILE *stream) {
    _file_lock(stream);
    FileBuffer *buffer = &stream->buffer;
    int res = c;
    if(buffer->mode == _IONBF) {
        unsigned char str[1] = {c};
        DWORD bytes_written;
        BOOL ok = WriteFile(stream->handle, &str, 1, &bytes_written, NULL);
        if(!ok) {
            res = EOF;
            stream->err = 1;
            goto end;
        }

    }
    else {
        unsigned char *data = buffer->data;
        data[buffer->written++] = (unsigned char)c;
        int needs_flush;
        needs_flush = (buffer->written == buffer->size);
        if(buffer->mode == _IOLBF) {
            needs_flush |= (c == '\n');
        }
        if(needs_flush) {
            if(fflush(stream) == EOF) {
                goto end;
            }
        }
    }
end:
    _file_unlock(stream);
    return res;
}

int fgetc(FILE *stream) {
    _file_lock(stream);
    int res = 0;
    FileBuffer *buffer = &stream->buffer;
    int read_from_disk = 1;
    if(buffer->mode != _IONBF) {
        unsigned char *data = buffer->data;
        if(buffer->written != 0) {
            read_from_disk = 0;
            res = data[--buffer->written];
        }
    }
    if(read_from_disk) {
        unsigned char buf[1];
        DWORD bytes_read;
        BOOL ok = ReadFile(stream->handle, buf, 1, &bytes_read, NULL);
        if(bytes_read != 1) {
            res = EOF;
            stream->eof = 1;
            goto end;
        }
        if(!ok) {
            res = EOF;
            goto end;
        }
        res = buf[0];
    }
end:
    _file_unlock(stream);
    return res;
}

int ungetc(int c, FILE *stream) {
    _file_lock(stream);
    int res;
    FileBuffer *buffer = &stream->buffer;
    if(buffer->mode == _IONBF) {
        res = EOF;
        goto end;
    }
    else {
        if(c == EOF) {
            res = EOF;
            goto end;
        }
        if(buffer->written == buffer->size) {
            res = EOF;
            goto end;
        }
        unsigned char *data = buffer->data;
        data[buffer->written++] = (unsigned char)c;
        res = c;
    }
end:
    _file_unlock(stream);
    return res;
}

size_t fwrite(void const *restrict buffer, size_t size, size_t count, FILE *restrict stream) {
    _file_lock(stream);
    unsigned char const *bytes = (unsigned char const*)buffer;
    size_t num_written;
    for(num_written = 0; num_written < count; ++num_written) {
        for(size_t bytes_written = 0; bytes_written < size; ++bytes_written) {
            int ch = fputc(bytes[bytes_written], stream);
            if(ch == EOF) {
                goto end;
            }
        }
        bytes += size;
    }
end:
    _file_unlock(stream);
    return num_written;
}

size_t fread(void *restrict buffer, size_t size, size_t count, FILE *restrict stream) {
    _file_lock(stream);
    unsigned char *bytes = (unsigned char *)buffer;
    size_t num_read;
    for(num_read = 0; num_read < count; ++num_read) {
        for(size_t bytes_read = 0; bytes_read < size; ++bytes_read) {
            int ch = fgetc(stream);
            if(ch == EOF) {
                goto end;
            }
            bytes[bytes_read] = ch;
        }
        bytes += size;
    }
end:
    _file_unlock(stream);
    return num_read;
}

int getchar(void) {
    return fgetc(stdin);
}

int putchar(int ch) {
    return fputc(ch, stdout);
}

char *fgets(char *restrict str, int count, FILE *restrict stream) {
    if(count < 1) {
        return str;
    }
    if(count == 1) {
        str[0] = 0;
        return str;
    }
    _file_lock(stream);
    int i;
    for(i = 0; i < count-1; ++i) {
        int c = fgetc(stream);
        if(c == EOF) {
            stream->eof = 1;
            if(i == 0) {
                return NULL;
            }
            break;
        }
        str[i] = c;
        if(c == '\n') {
            ++i;
            break;
        }
    }
    str[i] = 0;
    _file_unlock(stream);
    return str;
}

int fputs(char const *str, FILE *stream) {
    _file_lock(stream);
    int res = 0;
    while(*str) {
        int c = fputc(*str++, stream);
        if(c == EOF) {
            res = EOF;
            break;
        }
    }
    _file_unlock(stream);
    return res;
}

int puts(char const *str) {
    int res = fputs(str, stdout);
    if(res == EOF) return EOF;
    return putchar('\n');
}

char *gets(char *str) {
    return fgets(str, 0x7fffffff, stdin);
}

// File positioning:

int fgetpos(FILE *restrict stream, fpos_t *restrict pos) {
    LONG pos_hi = 0;
    DWORD pos_lo = SetFilePointer(stream->handle, 0, &pos_hi, FILE_CURRENT);
    if(pos_lo == INVALID_SET_FILE_POINTER) {
        return 1;
    }
    int64_t offset = ((int64_t)pos_hi << 32) | (int64_t)pos_lo;
    pos->offset = offset;
    pos->mbstate.leftover = stream->mbstate.leftover;
    pos->mbstate.high_surrogate = stream->mbstate.high_surrogate;
    return 0;
}

int fseek(FILE *stream, long int offset, int whence) {
    // Note(bumbread): the SEEK_SET, SEEK_CUR and SEEK_END are defined to match
    // the Windows constants, so no conversion is requierd between them.
    DWORD pos_lo = SetFilePointer(stream->handle, offset, NULL, whence);
    if(pos_lo == INVALID_SET_FILE_POINTER) {
        return -1L;
    }
    return 0;
}

int fsetpos(FILE *stream, const fpos_t *pos) {
    LONG pos_lo = (LONG)(pos->offset & 0xffffffff);
    DWORD status = SetFilePointer(stream->handle, pos_lo, NULL, FILE_BEGIN);
    if(status == INVALID_SET_FILE_POINTER) {
        return 1;
    }
    stream->mbstate.leftover = pos->mbstate.leftover;
    stream->mbstate.high_surrogate = pos->mbstate.high_surrogate;
    return 0;
}

long int ftell(FILE *stream) {
    LONG pos_hi = 0;
    DWORD pos_lo = SetFilePointer(stream->handle, 0, &pos_hi, FILE_CURRENT);
    if(pos_lo == INVALID_SET_FILE_POINTER) {
        return -1L;
    }
    int64_t offset = ((int64_t)pos_hi << 32) | (int64_t)pos_lo;
    return offset;
}

void rewind(FILE *stream) {
    fseek(stream, 0, SEEK_SET);
}
