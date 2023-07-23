
#include <cia_definitions.h>

#if os_is_linux()
    #include "linux/syscall.c"
    #include "linux/errno.c"
    #include "linux/entry.c"
    // TinyRT interface
    #include "linux/tinyrt.iface.h"
    #include "tinyrt.h"
    #include "linux/tinyrt.c"
#elif os_is_windows()
    #error "Not implemented yet"
#endif
