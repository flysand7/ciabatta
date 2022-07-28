
// Windows symbols because windows
int _fltused=0;

extern int main(int argc, char** argv);

// Exit routines
#define ATEXIT_FUNC_COUNT  64
#define ATQEXIT_FUNC_COUNT 64
static void (*atexit_funcs [ATEXIT_FUNC_COUNT])(void);
static void (*atqexit_funcs[ATQEXIT_FUNC_COUNT])(void);
static int atexit_func_count;
static int atqexit_func_count;

static char **get_command_args(int *argc_ptr);

void mainCRTStartup() {
    // Set-up some platform stuff
    _setup_eh();
    _setup_heap();
    _setup_timer();
    _setup_io();

    // Set-up CRT stuff
    srand(0);
    setlocale(LC_ALL, "C");

    // Parse command-line arguments
    int argc;
    char **args = get_command_args(&argc);
    int exit_code = main(argc, args);

    // we call exit because we want atexit routines run
    // and all the file handles to be freed
    exit(exit_code);
}


_Noreturn void quick_exit(int status) {
    while(atqexit_func_count--) {
        atqexit_funcs[atqexit_func_count]();
    }
    ExitProcess(status);
}

_Noreturn void exit(int status) {
    while(atexit_func_count--) {
        atexit_funcs[atqexit_func_count]();
    }
    _close_io();
    ExitProcess(status);
}

_Noreturn void _Exit(int status) {
    ExitProcess(status);
}

_Noreturn void abort(void) {
    raise(SIGABRT);
    ExitProcess(-69);
}

int atexit(void (*func)(void)) {
    if (atexit_func_count >= ATEXIT_FUNC_COUNT) {
        return 0;
    }
    atexit_funcs[atexit_func_count++] = func;
    return 1;
}

int at_quick_exit(void (*func)(void)) {
    if(atqexit_func_count >= ATQEXIT_FUNC_COUNT) {
        return 0;
    }
    atqexit_funcs[atqexit_func_count++] = func;
    return 1;
}

char *getenv(const char *name) {
    // The string pointed to shall not be modified by the program, but may be
    // overwritten by a subsequent call to the getenv function
    static size_t env_string_cap;
    static char* env_string;

    DWORD env_length = GetEnvironmentVariable(name, NULL, 0);
    if (env_length == 0) {
        return 0;
    }

    // Upscale the internal string
    if (env_length > env_string_cap) {
        char* newstr = realloc(env_string, env_length);
        if (newstr == NULL) {
            free(env_string);
            return 0;
        }

        env_string = newstr;
        env_string_cap = env_length;
    }

    GetEnvironmentVariable(name, env_string, env_length);
    return env_string;
}

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

int _wcsicmp(wchar_t const* s1, wchar_t const* s2) {
    int diff;
    do {
        diff = *s1 - *s2;
    } while(diff != 0 && *s1 != 0 && *s2 != 0);
    return diff;
}

static size_t count_wide_chars(const wchar_t* str) {
    size_t len = 0;
    while (str[len] != 0) len++;
    return len;
}

static bool wchar_to_ansi(char* out, const wchar_t* str, size_t len) {
    for (size_t i = 0; i < len; i++) {
        wchar_t ch = *str++;
        if (ch < 0 || ch > 0x7F) {
            *out++ = 0;
            return false;
        }
        *out++ = ch;
    }

    *out++ = 0;
    return true;
}

static char **get_command_args(int *argc_ptr) {
    int argc;
    char** args;

    LPCWSTR command_line = GetCommandLineW();
    LPWSTR* args_wide = CommandLineToArgvW(command_line, &argc);
    if (!args_wide || argc <= 0) {
        ExitProcess(-69420);
    }

    args = calloc(argc, sizeof(char*));

    // Convert wide chars into ANSI
    for (int i = 0; i < argc; i++) {
        size_t arg_len = count_wide_chars(args_wide[i]);
        args[i] = malloc(arg_len+1);
        wchar_to_ansi(args[i], args_wide[i], arg_len+1);
    }

    *argc_ptr = argc;
    return args;
}
