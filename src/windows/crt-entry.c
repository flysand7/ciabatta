
#include <cia-def.h>
#include "windows.c"

extern int main(int argc, char **argv, char **envp);
extern void _cia_init();

void mainCRTStartup() {
    _cia_init();
    __security_init_cookie();
    main(0, NULL, NULL);
    ExitProcess(0);
}

void wmainCRTStartup() {
    ExitProcess(0);    
}

void _WinMainCRTStartup() {
    ExitProcess(0);
}

void wWinMainCRTStartup() {
    ExitProcess(0);
}

BOOL _DllMainCRTStartup(HINSTANCE instance, DWORD reason, void *_reserved) {
    switch(reason) {
        case DLL_PROCESS_ATTACH: {
            // Initialize once for each new process.
            // Return FALSE to fail DLL load.
        } break;
        case DLL_THREAD_ATTACH: {
            // Do thread-specific initialization.
        } break;
        case DLL_THREAD_DETACH: {
            // Do thread-specific cleanup.
        } break;
        case DLL_PROCESS_DETACH: {
            // Perform any necessary cleanup.
        } break;
    }
    return TRUE;
}

u64 __security_cookie;

void __security_init_cookie() {
    // They say it's a random number so I generated
    // one using numbergenerator.org
    __security_cookie = 0xb26e04cc62ba48aULL;
}

void __security_check_cookie(u64 retrieved) {
    if(__security_cookie != retrieved) {
        char buf[] = "Buffer overrun detected!\n";
        HANDLE stdout = GetStdHandle(STD_OUTPUT_HANDLE);
        WriteFile(stdout, buf, sizeof buf, NULL, NULL);
        // TODO: abort-like behaviour here
        ExitProcess(1);
    }
}


