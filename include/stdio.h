
#pragma once

#include <stdarg.h>

// 7.23.3 p.2: Header version
#define __STDC_VERSION_STDIO_H__ 202311L

// 7.23.3 p.3: Types
#if defined(_WIN32)
    typedef unsigned long long size_t;
#else
    typedef unsigned long size_t;
#endif

typedef struct FILE FILE;

typedef struct {
    unsigned long long offset;
    union {
        unsigned short leftover;
        unsigned short high_surrogate;
    } mbstate;
} fpos_t;

// 7.23.3 p.4: Macros
#if !defined(NULL)
    #define NULL ((void *)0)
#endif

#define _IONBF    0
#define _IOFBF    1
#define _IOLBF    2
#define BUFSIZ    512
#define EOF       (-1)
#define FOPEN_MAX 32

#ifdef _os_win
    #define FILENAME_MAX 260
#else
    #define FILENAME_MAX 4096
#endif

#define _PRINTF_NAN_LEN_MAX 20

#define L_tmpnam FILENAME_MAX

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#ifdef _os_win
    #define TMP_MAX  SHORT_MAX
#else
    #define TMP_MAX INT_MAX
#endif

#define stdout _internal_stdout
#define stderr _internal_stderr
#define stdin  _internal_stdin

extern FILE *_internal_stdout;
extern FILE *_internal_stderr;
extern FILE *_internal_stdin;

// File manipulation
int remove(const char *filename);
int rename(const char *oldname, const char *newname);

// Opening and closing files
char *tmpnam(char *s);
FILE *tmpfile(void);
FILE *fopen  (const char *restrict filename, const char *restrict mode);
FILE *freopen(const char *restrict filename, const char *restrict mode, FILE *restrict stream);
int   fclose (FILE *stream);

// I/O buffering control
void setbuf  (FILE *restrict stream, char *restrict buf);
int  setvbuf (FILE *restrict stream, char *restrict buf, int mode, size_t size);
int  fflush  (FILE *stream);

// String formatted print
int vsnprintf(char *restrict s, size_t n, const char *restrict format, va_list arg);
int vsprintf (char *restrict s, const char *restrict format, va_list arg);
int snprintf (char *restrict s, size_t n, const char *restrict format, ...);
int sprintf  (char *restrict s, const char *restrict format, ...);

// File formatted print
int vfprintf (FILE *restrict stream, const char *restrict format, va_list arg);
int fprintf  (FILE *restrict stream, const char *restrict format, ...);
int vprintf  (const char *restrict format, va_list arg);
int printf   (const char *restrict format, ...);

// String formatted scan
int vsscanf(const char *restrict s, const char *restrict format, va_list arg);
int vscanf (const char *restrict format, va_list arg);
int sscanf (const char *restrict s, const char *restrict format, ...);

// String formatted scan
int vfscanf(FILE *restrict stream, const char *restrict format, va_list arg);
int fscanf (FILE *restrict stream, const char *restrict format, ...);
int scanf  (const char *restrict format, ...);

// File reading
#define getc fgetc
int    fgetc  (FILE *stream);
int    getchar(void);
int    ungetc (int c, FILE *stream);
char  *fgets  (char *restrict s, int n, FILE *restrict stream);
size_t fread  (void *restrict ptr, size_t size, size_t nmemb, FILE *restrict stream);

// File writing
#define putc fputc
int    fputc  (int c, FILE *stream);
int    putchar(int c);
int    fputs  (const char *restrict s, FILE *restrict stream);
int    puts   (const char *s);
size_t fwrite (const void *restrict ptr, size_t size, size_t nmemb, FILE *restrict stream);

int  fgetpos   (FILE *restrict stream, fpos_t *restrict pos);
int  fseek     (FILE *stream, long int offset, int whence);
int  fsetpos   (FILE *stream, const fpos_t *pos);
long int ftell (FILE *stream);
void rewind    (FILE *stream);

void clearerr(FILE *stream);
int  feof    (FILE *stream);
int  ferror  (FILE *stream);
void perror  (const char *s);

#if __STDC_WANT_LIB_EXT1__ == 1
    #define L_tmpnam_s L_tmpnam
    #define TMP_MAX_S  TMP_MAX

    errno_t tmpfile_s(FILE *restrict *restrict streamptr);
    errno_t tmpnam_s (char *s, rsize_t maxsize);
#endif
