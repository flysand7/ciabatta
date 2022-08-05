
#define CMDLINE_CMD_MAX  32767
#define CMDLINE_ARGV_MAX (16384+(98298+(int)sizeof(char*))/(int)sizeof(char*))

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

static wchar_t *get_wcmdline() {
    // That's right, that's where windows hid the command line
    TEB *teb = (TEB *)__readgsqword(offsetof(NT_TIB, Self));
    PEB *peb = teb->ProcessEnvironmentBlock;
    RTL_USER_PROCESS_PARAMETERS *params = peb->ProcessParameters;
    UNICODE_STRING command_line_str = params->CommandLine;
    return command_line_str.Buffer;
}

static char **get_command_args(int *argc_ptr) {
    static char *argv_buffer[CMDLINE_ARGV_MAX];
    wchar_t *cmdline = get_wcmdline();
    *argc_ptr = cmdline_to_argv8(cmdline, argv_buffer);
    return argv_buffer;
}
