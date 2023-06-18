
#pragma once

typedef int sig_atomic_t;

// TODO: idk about SIG_ERR, for now this
#define SIG_ERR ((void(*)(int))0)
#define SIG_DFL _signal_default_handler
#define SIG_IGN _signal_ignore_handler

// Note(bumbread): from the impl standpoint the numbers are arbitrary
#define _SIG_MIN 0
#define SIGINT   1
#define SIGILL   2
#define SIGFPE   3
#define SIGSEGV  4
#define SIGTERM  5
#define SIGABRT  6
// These guys are impl defined
#define SIGBREAK 7
#define SIGALIGN 8
#define SIGSTEP  9
#define _SIG_MAX 9

void (*signal(int sig, void (*func)(int)))(int);
int raise(int sig);
