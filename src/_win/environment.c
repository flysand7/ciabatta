
// Windows symbols because windows
int _fltused=0;

#define CMDLINE_CMD_MAX  32767
#define CMDLINE_ARGV_MAX (16384+(98298+(int)sizeof(char*))/(int)sizeof(char*))

extern int main(int argc, char** argv);

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "DbgHelp.lib")

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


_Noreturn void _Exit(int status) {
    ExitProcess(status);
    #if defined(_MSC_VER)
        __assume(0);
    #elif defined(__GNUC__)
        __builtin_unreachable();
    #endif
}

_Noreturn void quick_exit(int status) {
    while(atqexit_func_count--) {
        atqexit_funcs[atqexit_func_count]();
    }
    _Exit(status);
}

_Noreturn void exit(int status) {
    while(atexit_func_count--) {
        atexit_funcs[atqexit_func_count]();
    }
    _close_io();
    _Exit(status);
}

_Noreturn void abort(void) {
    raise(SIGABRT);
    _Exit(-69);
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
    PROCESS_INFORMATION pi = {0};

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

static int cmdline_to_argv8(const wchar_t *cmd, char **argv);

static char **get_command_args(int *argc_ptr) {
    static char *argv_buffer[CMDLINE_ARGV_MAX];
    // That's right, that's where windows hid the command line
    TEB *teb = (TEB *)__readgsqword(offsetof(NT_TIB, Self));
    PEB *peb = teb->ProcessEnvironmentBlock;
    RTL_USER_PROCESS_PARAMETERS *params = peb->ProcessParameters;
    UNICODE_STRING command_line_str = params->CommandLine;
    wchar_t *cmdline = command_line_str.Buffer;
    // Now we can do the actual job
    *argc_ptr = cmdline_to_argv8(cmdline, argv_buffer);
    return argv_buffer;
}

// https://github.com/skeeto/scratch/blob/master/misc/cmdline.c#L27
static int cmdline_to_argv8(const wchar_t *cmd, char **argv) {
    int argc  = 1;  // worst case: argv[0] is an empty string
    int state = 6;  // special argv[0] state
    int slash = 0;
    char *buf = (char *)(argv + 16384);  // second half: byte buffer

    argv[0] = buf;
    while (*cmd) {
        int c = *cmd++;
        if (c>>10 == 0x36 && *cmd>>10 == 0x37) {  // surrogates?
            c = 0x10000 + ((c - 0xd800)<<10) + (*cmd++ - 0xdc00);
        }

        switch (state) {
        case 0: switch (c) {  // outside token
                case 0x09:
                case 0x20: continue;
                case 0x22: argv[argc++] = buf;
                           state = 2;
                           continue;
                case 0x5c: argv[argc++] = buf;
                           slash = 1;
                           state = 3;
                           break;
                default  : argv[argc++] = buf;
                           state = 1;
                } break;
        case 1: switch (c) {  // inside unquoted token
                case 0x09:
                case 0x20: *buf++ = 0;
                           state = 0;
                           continue;
                case 0x22: state = 2;
                           continue;
                case 0x5c: slash = 1;
                           state = 3;
                           break;
                } break;
        case 2: switch (c) {  // inside quoted token
                case 0x22: state = 5;
                           continue;
                case 0x5c: slash = 1;
                           state = 4;
                           break;
                } break;
        case 3:
        case 4: switch (c) {  // backslash sequence
                case 0x22: buf -= (1 + slash) >> 1;
                           if (slash & 1) {
                               state -= 2;
                               break;
                           } // fallthrough
                default  : cmd--;
                           state -= 2;
                           continue;
                case 0x5c: slash++;
                } break;
        case 5: switch (c) {  // quoted token exit
                default  : cmd--;
                           state = 1;
                           continue;
                case 0x22: state = 1;
                } break;
        case 6: switch (c) {  // begin argv[0]
                case 0x09:
                case 0x20: *buf++ = 0;
                           state = 0;
                           continue;
                case 0x22: state = 8;
                           continue;
                default  : state = 7;
                } break;
        case 7: switch (c) {  // unquoted argv[0]
                case 0x09:
                case 0x20: *buf++ = 0;
                           state = 0;
                           continue;
                } break;
        case 8: switch (c) {  // quoted argv[0]
                case 0x22: *buf++ = 0;
                           state = 0;
                           continue;
                } break;
        }

        switch (c & 0x1f0880) {  // WTF-8/UTF-8 encoding
        case 0x00000: *buf++ = 0x00 | ((c >>  0)     ); break;
        case 0x00080: *buf++ = 0xc0 | ((c >>  6)     );
                      *buf++ = 0x80 | ((c >>  0) & 63); break;
        case 0x00800:
        case 0x00880: *buf++ = 0xe0 | ((c >> 12)     );
                      *buf++ = 0x80 | ((c >>  6) & 63);
                      *buf++ = 0x80 | ((c >>  0) & 63); break;
        default     : *buf++ = 0xf0 | ((c >> 18)     );
                      *buf++ = 0x80 | ((c >> 12) & 63);
                      *buf++ = 0x80 | ((c >>  6) & 63);
                      *buf++ = 0x80 | ((c >>  0) & 63);
        }
    }

    *buf = 0;
    argv[argc] = 0;
    return argc;
}
