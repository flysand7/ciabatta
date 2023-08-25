
#include <cia-def.h>

// NOTE: These symbols are provided by the linker
#define attribute_hidden __attribute__((__visibility__("hidden")))
extern void (*__preinit_array_start []) (int, char **, char **) attribute_hidden;
extern void (*__preinit_array_end []) (int, char **, char **) attribute_hidden;
extern void (*__init_array_start []) (int, char **, char **) attribute_hidden;
extern void (*__init_array_end []) (int, char **, char **) attribute_hidden;
extern void (*__fini_array_start []) (void) attribute_hidden;
extern void (*__fini_array_end []) (void) attribute_hidden;

// extern void _init();
// extern void _fini();

// These will be called by __libc_start_main
void __libc_global_init(int argc, char **argv, char **envp) {
    // _init();
    u64 size = (u64)__init_array_end - (u64)__init_array_start;
    for(u64 i = 0; i != size; ++i) {
        (*__init_array_start[i])(argc, argv, envp);
    }
}

void __libc_global_fini(int argc, char **argv, char **envp) {
    u64 size = (u64)__fini_array_end - (u64)__fini_array_start;
    u64 i = size;
    while(i-- > 0) {
        (*__fini_array_start[i])();
    }
    // _fini();
}
