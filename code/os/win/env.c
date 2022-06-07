
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

void _os_exit(int code)
{
    ExitProcess(code);
}
