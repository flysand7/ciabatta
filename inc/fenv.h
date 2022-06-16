
#pragma once

typedef unsigned fexcept_t;
typedef unsigned fenv_t;

// These constants correspond to bits in MXCSR register
#define FE_INVALID   (1 << 0)
#define FE_DIVBYZERO (1 << 2)
#define FE_OVERFLOW  (1 << 3)
#define FE_UNDERFLOW (1 << 4)
#define FE_INEXACT   (1 << 5)
#define FE_DENORM    (1 << 1)
#define FE_DAZ       (1 << 6)

#define FE_ALL_EXCEPT   \
    ( FE_INVALID        \
    | FE_DIVBYZERO      \
    | FE_OVERFLOW       \
    | FE_UNDERFLOW      \
    | FE_INEXACT        \
    | FE_DENORM         \
    | FE_DAZ            )

// These constants correspond to the rounding field in MXCSR register
#define FE_TONEAREST  0x00
#define FE_DOWNWARD   0x01
#define FE_UPWARD     0x02
#define FE_TOWARDZERO 0x03

extern fenv_t _fe_dfl_env;
#define FE_DFL_ENV (&_fe_dfl_env)

// Exceptions

int feclearexcept(int excepts);
int fegetexceptflag(fexcept_t *flagp, int excepts);
int feraiseexcept(int excepts);
int fesetexceptflag(const fexcept_t *flagp, int excepts);
int fetestexcept(int excepts);

// Rounding behaviour

int fegetround(void);
int fesetround(int round);

// Environment

int fegetenv(fenv_t *env);
int fesetenv(fenv_t *env);
int feholdexcept(fenv_t *envp);
int feupdateenv(fenv_t const *envp);

// Non-standard functions

int _feenabletraps(int excepts);
int _fedisabletraps(int excepts);

#if defined(_CIABATTA_EXT)
    #define feenabletraps  _feenabletraps
    #define fedisabletraps _fedisabletraps
#endif