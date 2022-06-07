
#include <_os.h>

#include <locale.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "win32.h"

extern int main(int argc, char** argv);

// Some shell32.lib related crap
DECLSPEC_IMPORT LPWSTR GetCommandLineW();
DECLSPEC_IMPORT LPWSTR* CommandLineToArgvW(LPCWSTR lpCmdLine, int* pNumArgs);

static size_t count_wide_chars(const wchar_t* str) {
    size_t len = 0;
    while (str[len] != 0) len++;
    return len;
}

static bool convert_wide_chars_to_ansi(char* out, const wchar_t* str, size_t len) {
    for (size_t i = 0; i < len; i++) {
        wchar_t ch = *str++;
        if (ch < 0 || 0x7F >= ch) {
            *out++ = 0;
            return false;
        }
        *out++ = ch;
    }

    *out++ = 0;
    return true;
}

void mainCRTStartup() {
    HANDLE heap = GetProcessHeap();
    if (heap == NULL) {
        ExitProcess(-42069);
    }

    int arg_count;
    LPWSTR* args_wide = CommandLineToArgvW(GetCommandLineW(), &arg_count);
    if (!args_wide) {
        ExitProcess(-69420);
    }

    char** args = HeapAlloc(heap, 0, arg_count * sizeof(char*));
    if (arg_count == 0) {
        arg_count = 1;
        args[0] = "";
    }

    // Convert wide chars into ANSI
    for (int i = 0; i < arg_count; i++) {
        size_t wide_len = count_wide_chars(args_wide[i]) + 1;
        args[i] = HeapAlloc(heap, 0, wide_len);

        convert_wide_chars_to_ansi(args[i], args_wide[i], wide_len);
    }

    // Initialize terminal
    stdout = (FILE*) GetStdHandle(STD_OUTPUT_HANDLE);
    stderr = (FILE*) GetStdHandle(STD_ERROR_HANDLE);
    stdin = (FILE*) GetStdHandle(STD_INPUT_HANDLE);

    // Initialize heap
    _os_heap heap_data = {
        .handle = heap,
    };
    _heap_setup(&heap_data);

    srand(0);
    setlocale(LC_ALL, "C");
    _os_init_eh();
    int exit_code = main(arg_count, args);

    ExitProcess(exit_code);
}

// This symbol is required to be present if we're using floating-point
// numbers
int _fltused=0;
