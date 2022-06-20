
#include <win.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// It's just mapped directly to HANDLE
struct FILE {
    int unused;
};

FILE *stdout;
FILE *stdin;
FILE *stderr;

int system(const char* string) {
    int wchars_required = MultiByteToWideChar(65001, 0, string, -1, NULL, 0);
    wchar_t* cmd_line = malloc(sizeof(L"cmd.exe ") + (wchars_required * sizeof(wchar_t)));
    if (cmd_line == NULL) {
        goto error;
    }

    memcpy(cmd_line, L"cmd.exe ", sizeof(L"cmd.exe "));
    MultiByteToWideChar(65001, 0, string, -1, cmd_line + sizeof("cmd.exe ") - 1, wchars_required);

    STARTUPINFOW si = {
        .cb = sizeof(STARTUPINFOW),
        .dwFlags = STARTF_USESTDHANDLES,
        .hStdInput = GetStdHandle(STD_INPUT_HANDLE),
        .hStdError = GetStdHandle(STD_ERROR_HANDLE),
        .hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE)
    };
    PROCESS_INFORMATION pi = {};

    if (!CreateProcessW(NULL, cmd_line, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        goto error;
    }

    // Wait until child process exits.
    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD exit_code;
    if (!GetExitCodeProcess(pi.hProcess, &exit_code)) {
        goto error;
    }

    // Close process and thread handles.
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    free(cmd_line);
    return exit_code;

    error:
    free(cmd_line);
    return -1;
}
