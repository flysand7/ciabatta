
#include <stdlib.h>

#include "win.h"

_Noreturn void _os_exit(int code)
{
    ExitProcess(code);
}
