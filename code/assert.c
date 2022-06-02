
#include <assert.h>
#include <stdlib.h>
#include <_platform.h>

#if defined(_os_win)
    #define WIN32_LEAN_AND_MEAN
    #include<Windows.h>

    // TODO: make it work via printf

    static int _assert_strlen(char const *str)
    {
        int len = 0;
        while(*str++!=0) ++len;
        return len;
    }

    static void _assert_win32_prints(HANDLE conout, char const *str)
    {
        DWORD written = 0;
        WriteConsole(conout, str, _assert_strlen(str), &written, NULL);
    }

    extern void _assert_print(char *cond, char const *func, char const *file, char const *line)
    {
        HANDLE conout = GetStdHandle(STD_OUTPUT_HANDLE);
        if(conout != INVALID_HANDLE_VALUE) {
            _assert_win32_prints(conout, "Assertion Failed: ");
            _assert_win32_prints(conout, cond);
            _assert_win32_prints(conout, "\n\tFile: ");
            _assert_win32_prints(conout, file);
            _assert_win32_prints(conout, "\n\tFunc: ");
            _assert_win32_prints(conout, func);
            _assert_win32_prints(conout, "\n\tLine: ");
            _assert_win32_prints(conout, line);
        }
        else {
            // TODO:
            MessageBoxA(NULL,
                "Assertion Failed Somewhere, good luck finding it!\n",
                "Error",
                MB_OK);
        }
    }
#endif