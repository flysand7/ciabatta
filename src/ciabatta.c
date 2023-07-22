
#include <cia_definitions.h>

#if os_is_linux()
    #include "linux/syscall.c"
    #include "linux/entry.c"
#endif
