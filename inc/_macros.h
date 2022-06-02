
#pragma once

#define _str_(x) #x
#define _str(x) _str_(x)

#if !defined(_func)
    #if defined(_compiler_msvc)
        #define _func __FUNCTION__
    #else
        #define _func __func__
    #endif
#endif
