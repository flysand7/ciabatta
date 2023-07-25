
#include <cia_definitions.h>

#if _CIA_OS_LINUX()
    #include "linux/syscall.c"
    #include "linux/errno.c"
    #include "linux/entry.c"
    // TinyRT interface
    #include "linux/tinyrt.iface.h"
    #include "tinyrt.h"
    #include "linux/tinyrt.c"
#elif _CIA_OS_WINDOWS()
    #error "Not implemented yet"
#endif
