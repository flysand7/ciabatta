
#pragma once

#if defined (_MSC_VER)
    #define _Thread_local __declspec(thread)
#endif

#define EDOM   1
#define EILSEQ 2
#define ERANGE 3

extern _Thread_local int errno;
