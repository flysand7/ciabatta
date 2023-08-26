
#pragma once

#include <wchar.h>
#include <stddef.h>

#if defined(__has_include_next)
    #if __has_include_next(<stdatomic.h>)
        #include_next <stdatomic.h>
    #else
        #define __STDC_NO_ATOMICS__
    #endif
#endif
