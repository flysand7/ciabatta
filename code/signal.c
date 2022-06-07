
#include <_os.h>
#include <signal.h>

void _signal_default_handler(int sig)
{

}

void _signal_ignore_handler(int sig)
{
    // ignore :kekw:
}

static void (*(handlers[]))(int) = {
    [SIGINT]   = _signal_ignore_handler,
    [SIGILL]   = _signal_ignore_handler,
    [SIGFPE]   = _signal_ignore_handler,
    [SIGSEGV]  = _signal_ignore_handler,
    [SIGTERM]  = _signal_ignore_handler,
    [SIGABRT]  = _signal_ignore_handler,
    [SIGBREAK] = _signal_ignore_handler,
    [SIGALIGN] = _signal_ignore_handler,
    [SIGSTEP]  = _signal_ignore_handler,
};

void (*signal(int sig, void (*func)(int)))(int)
{
    if(_SIG_MIN <= sig && sig <= _SIG_MAX) {
        handlers[sig] = func;
        return func;
    }
    return SIG_ERR;
}

int raise(int sig)
{
    if(_SIG_MIN <= sig && sig <= _SIG_MAX) {
        handlers[sig](sig);
        if(sig == SIGFPE || sig == SIGILL || sig == SIGSEGV) {
            _os_exit(-69420);
        }
        return 1;
    }
    return 0;
}
