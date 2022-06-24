
#define __STDC_WANT_LIB_EXT2__ 1

#include <stdio.h>
#include <win.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <threads.h>
#include <uchar.h>
#include <errno.h>
#include <string.h>

enum str_type {
    STR_R,
    STR_W,
    STR_A,
} typedef str_type;

enum str_flags {
    STR_U = 1,
    STR_X = 2,
    STR_B = 4,
} typedef str_flags;

enum str_mode {
    STR_BIN,
    STR_TEXT,
} typedef str_mode;

struct FILE {
    HANDLE    handle;
    char     *name;
    str_type  type;
    str_flags flags;
    str_mode  mode;
    int       buftype;
    size_t    bufsize;
    int       bufidx;
    char     *buffer;
    mbstate_t mbstate;
    mtx_t     lock;
    FILE     *prev;
    FILE     *next;
};

FILE *stdout;
FILE *stdin;
FILE *stderr;

// Linked list because I'm based and you can't say I'm wrong
static FILE *file_list_last = NULL;

static inline FILE *new_file(
    HANDLE handle,
    char *name,
    str_type type,
    str_flags flags,
    int mode,
    int buftype,
    size_t bufsize,
    char *buffer
) {
    FILE *file = malloc(sizeof(FILE));
    if(file == NULL) return NULL;
    mtx_init(&file->lock, mtx_recursive);
    file->handle = handle;
    file->name   = strdup(name);
    file->type   = type;
    file->flags  = flags;
    file->mode   = mode;
    // Buffer for cache
    file->buftype = buftype;
    file->bufsize = bufsize;
    file->buffer = buffer;
    file->bufidx = 0;
    // Multibyte state
    file->mbstate = (mbstate_t){0};
    // Append to list of all streams
    if(file_list_last != NULL) {
        file_list_last->next = file;
        file->prev = file_list_last;
        file->next = NULL;
        file_list_last = file;
    }
    else {
        file_list_last = file;
        file->prev = NULL;
        file->next = NULL;
    }
    return file;
}

static inline void dispose_file(FILE *file) {
    mtx_t lock = file->lock;
    mtx_lock(&lock);
    // Only close the file if it's not a standard handle
    if(file->name) {
        CloseHandle(file->handle);
        free(file->name);
    }
    FILE *prev = file->prev;
    FILE *next = file->next;
    if(prev != NULL) prev->next = next;
    if(next != NULL) next->prev = prev;
    if(next == NULL) file_list_last = prev;
    free(file);
    mtx_unlock(&lock);
    mtx_destroy(&lock);
}

void _setup_io() {
    HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hstderr = GetStdHandle(STD_ERROR_HANDLE);
    HANDLE hstdin  = GetStdHandle(STD_INPUT_HANDLE);

    char *out_buf = calloc(BUFSIZ, sizeof(char));
    char *err_buf = out_buf;
    if(hstdout != hstderr) {
        err_buf = calloc(BUFSIZ, sizeof(char));
    }

    stdout = new_file(hstdout, NULL, STR_W, 0, 0, _IOLBF, BUFSIZ, out_buf);
    stderr = new_file(hstderr, NULL, STR_W, 0, 0, _IOLBF, BUFSIZ, err_buf);
    stdin  = new_file(hstdin,  NULL, STR_R, 0, 0, _IONBF, 0, NULL);
}

void _close_io() {
    while(file_list_last != NULL) {
        fflush(file_list_last);
        dispose_file(file_list_last);
    }
}

int setvbuf(
    FILE *restrict stream,
    char *restrict buf,
    int mode,
    size_t size
) {
    mtx_lock(&stream->lock);
    if(mode == _IONBF) {
        stream->buftype = mode;
        stream->buffer  = NULL;
        stream->bufsize = 0;
        return 0;
    }
    if(mode != _IOFBF && mode != _IOLBF)
        return 1;
    if(buf == NULL && size != 0) {
        buf = malloc(size);
    }
    if(size == 0) {
        buf = NULL;
        mode = _IONBF;
    }
    stream->buftype = mode;
    stream->buffer  = buf;
    stream->bufsize = size;
    mtx_unlock(&stream->lock);
    return 0;
}

void setbuf(FILE *restrict stream, char *restrict buf) {
    int mode = _IOFBF;
    if(buf == NULL) {
        mode = _IONBF;
    }
    setvbuf(stream, buf, mode, BUFSIZ);
}

int fflush(FILE *stream) {
    mtx_lock(&stream->lock);
    if(stream->buftype != _IONBF) {
        DWORD written;
        BOOL ok = WriteFile(
            stream->handle,
            stream->buffer,
            stream->bufidx,
            &written,
            NULL
        );
        if(!ok) {
            errno = EIO;
            return EOF;
        }
        stream->bufidx = 0;
    }
    mtx_unlock(&stream->lock);
    return 0;
}

static int try_fputc(FILE *stream, char c) {
    if(stream->buftype != _IONBF) {
        stream->buffer[stream->bufidx++] = c;
        if(stream->bufidx > stream->bufsize) {
            return fflush(stream);
        }
        if(stream->buftype == _IOLBF && c == '\n') {
            return fflush(stream);
        }
        return c;
    }
    DWORD written;
    BOOL ok = WriteFile(stream->handle, &c, 1, &written, NULL);
    if(!ok) {
        errno = EIO;
        return EOF;
    }
    return c;
}

static int try_fputs(FILE *stream, char *str, size_t len) {
    for(size_t i = 0; i != len; ++i) {
        int cw = try_fputc(stream, str[i]);
        if(cw == EOF) return EOF;
    }
    return 1;
}

int fputc(int c, FILE *stream) {
    mtx_lock(&stream->lock);
    int res = try_fputc(stream, c);
    mtx_unlock(&stream->lock);
    return res;
}

int fputs(const char *restrict s, FILE *stream) {
    mtx_lock(&stream->lock);
    while(*s) {
        int res = try_fputc(stream, *s);
        if(res == EOF) return EOF;
        ++s;
    }
    mtx_unlock(&stream->lock);
    return 1;
}
