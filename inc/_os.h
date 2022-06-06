
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
