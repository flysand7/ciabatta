#pragma once

#include <stdint.h>
#include <stdarg.h>

typedef struct FILE FILE;
typedef int64_t fpos_t;

typedef struct div_t div_t;
typedef struct ldiv_t ldiv_t;
typedef struct lldiv_t lldiv_t;

#if !defined(NULL)
    #define NULL ((void *)0)
#endif

#if !defined(__STDC_LIB_EXT1__)
    #define __STDC_LIB_EXT1__
    typedef int errno_t;
    typedef size_t rsize_t;
#endif

#ifdef __STDC_WANT_SECURE_LIB__
    #if !defined(__STDC_WANT_LIB_EXT1__)
        #define __STDC_WANT_LIB_EXT1__ 1
    #endif
#endif

#define _IOFBF 0x0000
#define _IOLBF 0x0040
#define _IONBF 0x0004

#define BUFSIZ 512

#define EOF    (-1)

#define FOPEN_MAX 20

#ifdef _os_win
    #define FILENAME_MAX 260
#else
    #define FILENAME_MAX 4096
#endif

#define L_tmpnam FILENAME_MAX

#define SEEK_CUR 1
#define SEEK_END 2
#define SEEK_SET 0

#ifdef _os_win
    #define TMP_MAX  SHORT_MAX
#else
    #define TMP_MAX INT_MAX
#endif

extern FILE *stdout, *stderr, *stdin;

int remove(const char *filename);
int rename(const char *oldname, const char *newname);
FILE *tmpfile(void);
char *tmpnam(char *s);
int fclose(FILE *stream);
int fflush(FILE *stream);
FILE *fopen(const char * restrict filename, const char * restrict mode);
FILE *freopen(const char * restrict filename, const char * restrict mode, FILE * restrict stream);
void setbuf(FILE * restrict stream, char * restrict buf);
int setvbuf(FILE * restrict stream, char * restrict buf, int mode, size_t size);
int fprintf(FILE * restrict stream, const char * restrict format, ...);
int fscanf(FILE * restrict stream, const char * restrict format, ...);
int printf(const char * restrict format, ...);
int scanf(const char * restrict format, ...);
int snprintf(char * restrict s, size_t n, const char * restrict format, ...);
int sprintf(char * restrict s, const char * restrict format, ...);
int sscanf(const char * restrict s, const char * restrict format, ...);
int vfprintf(FILE * restrict stream, const char * restrict format, va_list arg);
int vfscanf(FILE * restrict stream, const char * restrict format, va_list arg);
int vprintf(const char * restrict format, va_list arg);
int vscanf(const char * restrict format, va_list arg);
int vsnprintf(char * restrict s, size_t n, const char * restrict format, va_list arg);
int vsprintf(char * restrict s, const char * restrict format, va_list arg);
int vsscanf(const char * restrict s, const char * restrict format, va_list arg);
int fgetc(FILE *stream);
char *fgets(char * restrict s, int n, FILE * restrict stream);
int fputc(int c, FILE *stream);
int fputs(const char * restrict s, FILE * restrict stream);
int getc(FILE *stream);
int getchar(void);
int putc(int c, FILE *stream);
int putchar(int c);
int puts(const char *s);
int ungetc(int c, FILE *stream);
size_t fread(void * restrict ptr, size_t size, size_t nmemb, FILE * restrict stream);
size_t fwrite(const void * restrict ptr, size_t size, size_t nmemb, FILE * restrict stream);
int fgetpos(FILE * restrict stream, fpos_t * restrict pos);
int fseek(FILE *stream, long int offset, int whence);
int fsetpos(FILE *stream, const fpos_t *pos);
long int ftell(FILE *stream);
void rewind(FILE *stream);
void clearerr(FILE *stream);
int feof(FILE *stream);
int ferror(FILE *stream);
void perror(const char *s);

#if __STDC_WANT_LIB_EXT1__ == 1
    #define L_tmpnam_s L_tmpnam
    #define TMP_MAX_S  TMP_MAX
    errno_t tmpfile_s(FILE * restrict * restrict streamptr);
    errno_t tmpnam_s(char *s, rsize_t maxsize);
#endif
