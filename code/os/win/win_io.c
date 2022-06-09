
#include "win.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// It's just mapped directly to HANDLE
struct FILE {
    int unused;
};

int _os_del_file(char const *filename) {
    int ok = DeleteFileA(filename);
    return ok != 0;
}

int _os_mov_file(char const *old, char const *new) {
    int ok = MoveFileA(old, new);
    return ok != 0;
}

char *_os_tmpname(char *buffer) {
    static UINT uniq = 0;
    DWORD path_len = GetTempPathA(L_tmpnam, buffer);
    if(path_len == 0) return NULL;
    UINT ok = GetTempFileNameA(buffer, "", uniq, buffer);
    if(ok == 0) return NULL;
    return buffer;
}

FILE *_os_fopen(char const *restrict name, _OS_ModeFlags flags) {
    DWORD desaddr = 0;
    DWORD share = 0;
    DWORD disp = 0;
    switch(flags.base_mode) {
        case 'r': {
            desaddr = GENERIC_READ;
            if(!flags.update) {
                share = FILE_SHARE_READ;
            }
            disp = OPEN_EXISTING;
            if(flags.update) {
                disp = OPEN_ALWAYS;
            }
        } break;
        case 'w': {
            desaddr = GENERIC_WRITE;
            disp = CREATE_ALWAYS;
        } break;
        case 'a': {
            desaddr = GENERIC_WRITE;

        } break;
    }
    if(flags.exclusive) {
        disp = CREATE_NEW;
    }

    HANDLE fileHandle = CreateFileA(
                                    name,
                                    desaddr,
                                    share,
                                    NULL,
                                    disp,
                                    FILE_ATTRIBUTE_NORMAL,
                                    NULL
                                    );
    FILE *file = (FILE *)fileHandle;
    if(fileHandle == INVALID_HANDLE_VALUE) {
        file = NULL;
    }

    return file;
}

int _os_fclose(FILE *file) {
    HANDLE fileHandle = (HANDLE)file;
    BOOL ok = CloseHandle(fileHandle);
    return ok != 0;
}

void _os_file_write(void* ctx, size_t n, const char str[]) {
    DWORD written = 0;
    WriteFile((HANDLE) ctx, str, n, &written, NULL);
}

int system(const char* string) {
    int wchars_required = MultiByteToWideChar(65001 /* UTF8 */, 0, string, -1, NULL, 0);
    wchar_t* cmd_line = malloc(sizeof(L"cmd.exe ") + (wchars_required * sizeof(wchar_t)));
    if (cmd_line == NULL) {
        goto error;
    }

    memcpy(cmd_line, L"cmd.exe ", sizeof(L"cmd.exe "));
    MultiByteToWideChar(65001 /* UTF8 */, 0, string, -1, cmd_line + sizeof("cmd.exe ") - 1, wchars_required);

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
