
#pragma once

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
void _os_file_write(void* ctx, size_t n, const char str[]);
void _os_exit(int code);
void _os_init_eh();
typedef struct FILE FILE;
