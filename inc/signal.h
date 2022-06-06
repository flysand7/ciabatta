
#pragma once

typedef int sig_atomic_t;

// TODO: implement this
#define SIG_DFL 0
#define SIG_ERR 1
#define SIG_IGN 2

// not sure why but windows picked these, we can change it later
#define SIGINT          2
#define SIGILL          4
#define SIGFPE          8
#define SIGSEGV         11
#define SIGTERM         15
#define SIGBREAK        21
#define SIGABRT         22

void (*signal(int sig, void (*func)(int)))(int);
int raise(int sig);
