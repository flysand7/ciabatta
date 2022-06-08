
#include <stdlib.h>

#include "win.h"

char* getenv(const char *name) {
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

_Noreturn void _os_exit(int code) {
    ExitProcess(code);
}
