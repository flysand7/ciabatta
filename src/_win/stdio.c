
enum stream_width_t {
    STREAM_CHAR_UNSET,
    STREAM_CHAR_NARROW,
    STREAM_CHAR_WIDE,
} typedef stream_width_t;

enum stream_io_mode_t {
    STREAM_INPUT,
    STREAM_OUTPUT,
    STREAM_UPDATE,
} typedef stream_io_mode_t;

enum stream_bt_mode_t {
    STREAM_BINARY,
    STREAM_TEXT,
} typedef stream_bt_mode_t;

struct stream_buffer_t typedef stream_buffer_t;
struct stream_buffer_t {
    int    is_internal;
    int    mode;
    size_t size;
    void  *data;
    size_t written;
};

struct FILE {
    HANDLE           handle;
    stream_width_t   char_width; // This technically needs not be stored
    mbstate_t        mbstate;
    stream_buffer_t  buffer;
    stream_io_mode_t io_mode;
    stream_bt_mode_t bt_mode;
    int              eof;
    int              err;
    mtx_t            lock;
    FILE            *prev;
    FILE            *next;
};

FILE *stdout;
FILE *stdin;
FILE *stderr;

// We hold a linked list of all file streams in order to flush all the buffers
// after the program terminates. It might be not a good idea to store all the
// files into this linked list, but for now performance is not a concern.
static FILE *streams_to_close = NULL;

static void close_list_add(FILE *stream) {
    if(streams_to_close != NULL) {
        streams_to_close->next = stream;
        stream->prev = streams_to_close;
        stream->next = NULL;
        streams_to_close = stream;
    }
    else {
        streams_to_close = stream;
        stream->prev = NULL;
        stream->next = NULL;
    }
}

static void close_list_remove(FILE *stream) {
    FILE *prev = stream->prev;
    FILE *next = stream->next;
    if(prev != NULL) prev->next = next;
    if(next != NULL) next->prev = prev;
    if(next == NULL) streams_to_close = prev;
}

static inline FILE *create_stream(
    HANDLE handle,
    stream_io_mode_t io_mode,
    stream_bt_mode_t bt_mode
) {
    FILE *stream = malloc(sizeof(FILE));
    if(stream == NULL) return NULL;
    stream->handle = handle;
    stream->char_width = STREAM_CHAR_UNSET;
    stream->mbstate = (mbstate_t){0};
    stream->buffer.mode = _IONBF;
    stream->io_mode = io_mode;
    stream->bt_mode = bt_mode;
    stream->eof = 0;
    stream->err = 0;
    mtx_init(&stream->lock, mtx_recursive);
    close_list_add(stream);
    return stream;
}

static inline void delete_stream(FILE *stream) {
    mtx_t lock = stream->lock;
    mtx_lock(&lock);
    CloseHandle(stream->handle);
    close_list_remove(stream);
    free(stream);
    mtx_unlock(&lock);
    mtx_destroy(&lock);
}

void _setup_io() {
    HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hstderr = GetStdHandle(STD_ERROR_HANDLE);
    HANDLE hstdin  = GetStdHandle(STD_INPUT_HANDLE);

    stdout = create_stream(hstdout, STREAM_UPDATE, STREAM_TEXT);
    stderr = create_stream(hstderr, STREAM_UPDATE, STREAM_TEXT);
    stdin  = create_stream(hstdin,  STREAM_INPUT,  STREAM_BINARY);

    char *in_buf  = calloc(BUFSIZ, sizeof(char));
    char *out_buf = calloc(BUFSIZ, sizeof(char));
    stdin->buffer  = (stream_buffer_t){1, _IOLBF, BUFSIZ, in_buf};
    stdout->buffer = (stream_buffer_t){1, _IOLBF, BUFSIZ, out_buf};
    stderr->buffer = (stream_buffer_t){1, _IONBF,      0, NULL};
}

FILE *fopen(const char *restrict name, const char *restrict mode) {
    DWORD access = 0;
    DWORD share = 0;
    DWORD disp = 0;
    DWORD flags = FILE_FLAG_WRITE_THROUGH;
    stream_io_mode_t io_mode = 0;
    stream_bt_mode_t bt_mode;
    int flag_p = 0;
    int flag_b = 0;
    int flag_x = 0;
    switch(*mode++) {
        case 'r': io_mode = STREAM_INPUT;  break;
        case 'w': io_mode = STREAM_OUTPUT; break;
        case 'a': io_mode = STREAM_UPDATE; break;
        default: return NULL;
    }
    while(*mode) switch(*mode++) {
        case '+': flag_p = 1; break;
        case 'b': flag_b = 1; break;
        case 'x': flag_x = 1; break;
        default: return NULL;
    }
    bt_mode = flag_b? STREAM_BINARY : STREAM_TEXT;
    // Not sure about the sharing modes
    switch(io_mode) {
        case STREAM_INPUT: {
            access = GENERIC_READ | (flag_p? GENERIC_WRITE : 0);
            share  = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;
            disp   = OPEN_EXISTING;
        } break;
        case STREAM_OUTPUT: {
            access = GENERIC_WRITE | (flag_p? GENERIC_READ : 0);
            share  = 0;
            disp   = CREATE_ALWAYS;
            if(flag_x) {
                disp = CREATE_NEW;
            }
        } break;
        case STREAM_UPDATE: {
            access = GENERIC_READ | GENERIC_WRITE;
            share  = 0;
            disp   = OPEN_ALWAYS;
        } break;
    }
    HANDLE handle = CreateFileA(name, access, share, NULL, disp, flags, NULL);
    if(handle == INVALID_HANDLE_VALUE) {
        return NULL;
    }
    FILE *stream = create_stream(handle, io_mode, bt_mode);
    void *buffer_data = malloc(BUFSIZ);
    stream->buffer = (stream_buffer_t) {1, _IOFBF, BUFSIZ, buffer_data};
    return stream;
}

FILE *freopen(const char *restrict name, const char *restrict mode, FILE *restrict stream) {
    return NULL;
}

FILE *tmpfile(void) {
    return NULL;
}

int fclose(FILE *stream) {
    return 0;
}

void _close_io() {
    while(streams_to_close != NULL) {
        FILE *stream = streams_to_close;
        fflush(stream);
        delete_stream(stream);
    }
}

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
    printf("%s: %s\n", s, strerror(errno));
}

int setvbuf(FILE *restrict stream, char *restrict ptr, int mode, size_t size) {
    if(mode != _IOFBF && mode != _IOLBF && mode != _IONBF) {
        return 1;
    }
    mtx_lock(&stream->lock);
    stream_buffer_t *buffer = &stream->buffer;
    buffer->mode = mode;
    if(ptr == NULL) {
        buffer->data = realloc(buffer->data, size);
        buffer->size = size;
    }
    else {
        buffer->data = ptr;
        buffer->size = size;
    }
    mtx_unlock(&stream->lock);
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
    mtx_lock(&stream->lock);
    int res = 0;
    stream_buffer_t *buffer = &stream->buffer;
    void *data = buffer->data;
    size_t size = buffer->written;
    DWORD bytes_written;
    BOOL ok = WriteFile(stream->handle, data, size, &bytes_written, NULL);
    if(!ok) {
        res = EOF;
        stream->eof = 1;
    }
    buffer->written = 0;
    mtx_unlock(&stream->lock);
    return res;
}

int fputc(int c, FILE *stream) {
    mtx_lock(&stream->lock);
    stream_buffer_t *buffer = &stream->buffer;
    int res = c;
    if(buffer->mode == _IONBF) {
        unsigned char str[1] = {c};
        DWORD bytes_written;
        BOOL ok = WriteFile(stream->handle, &str, 1, &bytes_written, NULL);
        if(!ok) {
            res = EOF;
            stream->err = 1;
            goto cum;
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
                goto cum;
            }
        }
    }
cum:
    mtx_unlock(&stream->lock);
    return res;
}

int fgetc(FILE *stream) {
    mtx_lock(&stream->lock);
    int res = 0;
    stream_buffer_t *buffer = &stream->buffer;
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
            goto cum;
        }
        if(!ok) {
            res = EOF;
            goto cum;
        }
        res = buf[0];
    }
cum:
    mtx_unlock(&stream->lock);
    return res;
}

int ungetc(int c, FILE *stream) {
    mtx_lock(&stream->lock);
    int res;
    stream_buffer_t *buffer = &stream->buffer;
    if(buffer->mode == _IONBF) {
        res = EOF;
        goto cum;
    }
    else {
        if(c == EOF) {
            res = EOF;
            goto cum;
        }
        if(buffer->written == buffer->size) {
            res = EOF;
            goto cum;
        }
        unsigned char *data = buffer->data;
        data[buffer->written++] = (unsigned char)c;
        res = c;
    }
cum:
    mtx_unlock(&stream->lock);
    return 0;
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
    mtx_lock(&stream->lock);
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
    mtx_unlock(&stream->lock);
    return str;
}

int fputs(char const *str, FILE *stream) {
    mtx_lock(&stream->lock);
    int res = 0;
    while(*str) {
        int c = fputc(*str++, stream);
        if(c == EOF) {
            res = EOF;
            break;
        }
    }
    mtx_unlock(&stream->lock);
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
