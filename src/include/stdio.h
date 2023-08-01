
#pragma once

#include <cia-def.h>
#include <tinyrt.h>

typedef u64 size_t;

typedef struct FILE FILE;
struct FILE {
    _RT_File rt_file;
};

#define EOF (-1)

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

FILE *fopen(const char *restrict filename, const char *restrict mode);
int fgetc(FILE *file);
int fputc(int c, FILE *file);
size_t fread(void *restrict buf, size_t size, size_t count, FILE *restrict file);
size_t fwrite(void const *restrict buf, size_t size, size_t count, FILE *restrict file);
int fclose(FILE *file);
