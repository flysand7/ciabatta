
#pragma once
#include <stddef.h>

#if defined(_WIN32)
    #define _os_win
#endif

#if defined(__linux__) && !defined(__ANDROID__)
    #define _os_linux
#endif

#if !(defined(_os_win) \
   || defined(_os_linux))
    #error "Unsupported OS"
#endif

// OS-dependent IO Functions

// TODO: see if we need this or will it be easier for linux to just pass
// the mode string.
typedef struct _OS_ModeFlags {
    int base_mode;
    int binary;
    int update;
    int exclusive;
} _OS_ModeFlags;

typedef struct FILE FILE;
int   _os_del_file(char const *filename);
int   _os_mov_file(char const *old, char const *new);
char *_os_tmpname(char *buffer);
FILE *_os_fopen(char const *restrict name, _OS_ModeFlags flags);
int   _os_fclose(FILE *file);
void  _os_file_write(void* ctx, size_t n, const char str[]);
void  _os_file_write_char(void* ctx, char ch);

void _os_init_eh();

_Noreturn void _os_exit(int code);
