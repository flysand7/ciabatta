
#include <cia/def.h>

// Base includes
#include <cia/def.h>

// Platform-dependent sources
#include _CIA_OS_CONF

// Forward declarations
#include <stdlib.h>
#include <stdio.h>
#include <threads.h>
#include <cia/mem.h>
#include <cia/sync.h>

// Module cia_memory
#include "src/cia-mem/util.c"
#include "src/cia-mem/allocator.c"
#include "src/cia-mem/arena.c"
#include "src/cia-mem/pool.c"

// Module cia_sync
#include "src/cia-sync/mutex.c"

// Module stdlib_program
#include "src/stdlib-program/program.c"

// Module stdlib_threads
#include "src/stdlib-thread/thread.c"

// Module stdlib_file
#include "src/stdlib-file/file.c"

// Module stdlib_string
#include "src/stdlib-string/mem.c"
#include "src/stdlib-string/str.c"
