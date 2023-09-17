
#pragma once

static u64 cia_stack_size;
static u64 cia_tls_image_size;
static void *cia_tls_image_base;

#include <stdatomic.h>
#include <cia-ld/tcb.h>

#include <linux/futex.h>
#include <linux/sys/syscall.h>
#include <linux/sys/mman.h>
#include <linux/errno.h>
#include <linux/signal.h>
#include <linux/fcntl.h>
#include <linux/sched.h>
#include <tinyrt.h>
#include "tinyrt.c"
#include "tinyrt-threads.c"
#include "entry.c"

