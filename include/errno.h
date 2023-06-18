
#pragma once

#define EDOM   1
#define EILSEQ 2
#define ERANGE 3

// TODO: figure out why _Thread_local doesn't work
extern _Thread_local int errno;

#if __STDC_WANT_LIB_EXT1__ == 1
    typedef int errno_t;
#endif
