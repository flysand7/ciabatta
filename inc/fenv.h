
#pragma once

typedef unsigned fexcept_t;
typedef unsigned fenv_t;

#define FE_INVALID   (1 << 0)
#define FE_DIVBYZERO (1 << 2)
#define FE_OVERFLOW  (1 << 3)
#define FE_UNDERFLOW (1 << 4)
#define FE_INEXACT   (1 << 5)
#define FE_ALL_EXCEPT   \
    ( FE_INVALID        \
    | FE_DIVBYZERO      \
    | FE_OVERFLOW       \
    | FE_UNDERFLOW      \
    | FE_INEXACT        )

#define FE_TONEAREST  0x00
#define FE_DOWNWARD   0x01
#define FE_UPWARD     0x02
#define FE_TOWARDZERO 0x03

extern fenv_t _fe_dfl_env;
#define FE_DFL_ENV (&_fe_dfl_env)

int feclearexcept(int excepts);
int fegetexceptflag(fexcept_t *flagp, int excepts);
int feraiseexcept(int excepts);
int fesetexceptflag(const fexcept_t *flagp, int excepts);
int fetestexcept(int excepts);

int fegetround(void);
int fesetround(int round);

int fegetenv(fenv_t *env);
int fesetenv(fenv_t *env);
int feholdexcept(fenv_t *envp);
int feupdateenv(fenv_t const *envp);
