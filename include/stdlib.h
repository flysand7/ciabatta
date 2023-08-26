
#pragma once

#include <cia/def.h>

int atexit(void (*func)(void));
int at_quick_exit(void (*func)(void));
_Noreturn void abort(void);
_Noreturn void exit(int code);
_Noreturn void _Exit(int code);
_Noreturn void quick_exit(int code);