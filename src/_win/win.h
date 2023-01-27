
#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <processenv.h>
#include <DbgHelp.h>
#include <winternl.h>

static void _setup_timer(void);
static void _setup_eh();
static void _setup_heap();
static void _setup_io();
static void _close_io();