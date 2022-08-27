
#pragma weak main
#pragma weak wmain
#pragma weak WinMain

int main(int argc, char** argv);
int wmain(int argc, wchar_t** argv, wchar_t **envp);
int WinMain(HINSTANCE inst, HINSTANCE pinst, LPSTR cmdline, int showcmd);

_Noreturn void mainCRTStartup() {
    _setup_eh();
    _setup_heap();
    _setup_timer();
    _setup_io();
    __security_init_cookie();

    srand(0);
    setlocale(LC_ALL, "C");

    int argc;
    char **args = get_command_args(&argc);
    int exit_code = main(argc, args);

    exit(exit_code);
}

_Noreturn void WinMainCRTStartup() {
    _setup_eh();
    _setup_heap();
    _setup_timer();
    _setup_io();
    __security_init_cookie();

    srand(0);
    setlocale(LC_ALL, "C");

    HINSTANCE inst = GetModuleHandle(NULL);
    LPSTR cmdline = GetCommandLineA();
    int exit_code = WinMain(inst, 0, cmdline, SW_SHOWDEFAULT);

    exit(exit_code);
}
