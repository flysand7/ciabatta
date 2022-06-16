
#include <stdio.h>
#include <_os.h>

FILE *stdout;
FILE *stderr;
FILE *stdin;

int remove(const char *filename)
{
    return _os_del_file(filename);
}

int rename(const char *old, const char *new)
{
    return _os_mov_file(old, new);
}

char *tmpnam(char *s) {
    static char static_buffer[L_tmpnam];
    char *buffer = s;
    if(s == NULL) buffer = static_buffer;
    return _os_tmpname(buffer);
}

FILE *fopen(const char *restrict filename, const char *restrict mode)
{
    // Basically defined UB here by introducing missing modes
    // It is simpler to implement that way I think.
    int base_mode = mode[0];
    int binary = 0;
    int exclusive = 0;
    int update = 0;
    for(; *mode != 0; ++mode) {
        if(*mode == 'x') exclusive = 1;
        if(*mode == 'b') binary    = 1;
        if(*mode == '+') update    = 1;
    }
    if(base_mode == 'r' && exclusive) return NULL;
    if(base_mode == 'a' && exclusive) return NULL;
    
    _OS_ModeFlags mode_flags = {
        .base_mode = base_mode,
        .binary    = binary,
        .update    = update,
        .exclusive = exclusive,
    };

    return _os_fopen(filename, mode_flags);
}

int fclose(FILE *stream)
{
    return _os_fclose(stream);
}

// TODO:kekw:
FILE *freopen(
    const char *restrict filename,
    const char *restrict mode,
    FILE *restrict stream)
{
    fclose(stream);
    return NULL;
}
