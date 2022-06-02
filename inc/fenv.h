typedef unsigned long fexcept_t;
typedef struct fenv_t fenv_t;
	
#define FE_DIVBYZERO 0x04
#define FE_INEXACT   0x20
#define FE_INVALID   0x01
#define FE_OVERFLOW  0x08
#define FE_UNDERFLOW 0x10
#define FE_ALL_EXCEPT (FE_DIVBYZERO | FE_INEXACT | FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW)

#define FE_TONEAREST  0x000
#define FE_UPWARD     0x200
#define FE_DOWNWARD   0x100
#define FE_TOWARDZERO 0x300

// TODO: implement this
#define FE_DFL_ENV ((fenv_t*)0)

int feclearexcept(int excepts);
int fegetexceptflag(fexcept_t *flagp, int excepts);
int feraiseexcept(int excepts);
int fesetexceptflag(const fexcept_t *flagp, int excepts);
int fetestexcept(int excepts);
