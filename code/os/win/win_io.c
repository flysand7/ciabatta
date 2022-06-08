
#include "win.h"

// It's just mapped directly to HANDLE
struct FILE {
    int unused;
};

void _os_file_write(void* ctx, size_t n, const char str[]) {
    DWORD written = 0;
    WriteFile((HANDLE) ctx, str, n, &written, NULL);
}
