
#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <processenv.h>
#include <DbgHelp.h>
#include <winternl.h>

void _setup_timer(void);
void _setup_eh();
void _setup_heap();
void _setup_io();
void _close_io();
void __security_init_cookie();