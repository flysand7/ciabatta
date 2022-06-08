
#include "win.h"

#include <stddef.h>
#include <stdio.h>

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
