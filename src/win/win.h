
#pragma once

#include <_os.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

struct _os_heap {
    HANDLE handle;
};

void _os_timing_init(void);