
#include <stdio.h>
#include <win.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <threads.h>
#include <uchar.h>
#include <errno.h>

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
    file->handle = handle;
    file->name   = name; // TODO: strdup this
    file->type   = type;
    file->flags  = flags;
    file->mode   = mode;
    file->mbstate = (mbstate_t){0};
    mtx_init(&file->lock, mtx_recursive);
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
        //TODO: free(file->name);
    }
    // TODO: flush streams
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
    return 0;
}

int fflush(FILE *stream) {
    if(stream->mode == _IOFBF) {

    }
    else if(stream->mode == _IONBF) {

    }
    return 0;
}

int fputc(int c, FILE *stream) {
    mtx_lock(&stream->lock);
    DWORD written;
    BOOL ok = WriteFile(stream->handle, &c, 1, &written, NULL);
    mtx_unlock(&stream->lock);
    if(!ok) {
        errno = EIO;
        return EOF;
    }
    return c;
}

void setbuf(FILE *restrict stream, char *restrict buf) {
    if(buf != NULL)
        setvbuf(stream, buf, _IOFBF, BUFSIZ);
    else
        setvbuf(stream, buf, _IONBF, BUFSIZ);
}
