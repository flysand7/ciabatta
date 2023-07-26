
#pragma once

#include <cia-def.h>

int atexit(void (*func)(void));
int at_quick_exit(void (*func)(void));
noreturn void abort(void);
noreturn void exit(int code);
noreturn void _Exit(int code);
noreturn void quick_exit(int code);