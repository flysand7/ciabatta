
#pragma once

static u64 cia_stack_size;
static u64 cia_tls_image_size;
static void *cia_tls_image_base;

#include <stdatomic.h>
#include <cia-ld/tcb.h>

#include <linux/futex.h>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <sched.h>
#include "entry.c"
#include <tinyrt.h>
#include "tinyrt.c"
#include "tinyrt-threads.c"

