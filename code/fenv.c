
#include <fenv.h>
#include <intrin.h>

#define fe_masks(excepts) (((fexcept_t)(excepts)) << 7)
#define fe_flags(excepts) ((fexcept_t)(excepts))
#define fe_excepts(masks) ((int)(masks >> 7))

fenv_t _fe_dfl_env = 0x1f80; // Based (on my machine)

int feclearexcept(int excepts)
{
    if((excepts & FE_ALL_EXCEPT) != excepts) {
        return 1;
    }
    if(excepts == 0) {
        return 0;
    }
    fexcept_t csr = _mm_getcsr();
    csr &= ~fe_flags(excepts);
    _mm_setcsr(csr);
    return 0;
}

int fegetexceptflag(fexcept_t *flagp, int excepts) {
    if((excepts & FE_ALL_EXCEPT) != excepts) {
        return 1;
    }
    *flagp = fe_flags(excepts);
    return 0;
}

int feraiseexcept(int excepts) {
    if((excepts & FE_ALL_EXCEPT) != excepts) {
        return 1;
    }
    if(excepts == 0) {
        return 0;
    }
    fexcept_t csr = _mm_getcsr();
    csr |= fe_flags(excepts);
    _mm_setcsr(csr);
    return 0;
}

int fesetexceptflag(const fexcept_t *flagp, int excepts) {
    if((excepts & FE_ALL_EXCEPT) != excepts) {
        return 1;
    }
    if(excepts == 0) {
        return 0;
    }
    fexcept_t flags = *flagp;
    fexcept_t csr = _mm_getcsr();
    csr |= flags;
    _mm_setcsr(csr);
    return 0;
}

int fetestexcept(int excepts) {
    fexcept_t csr = _mm_getcsr();
    fexcept_t flags = fe_flags(excepts);
    return (int)(csr & flags);
}

int fegetround(void) {
    fexcept_t csr = _mm_getcsr();
    int round = (csr >> 13) & 0x3;
    return round;
}

int fesetround(int round) {
    if(!(0 <= round && round < 4)) {
        return 1;
    }
    fexcept_t csr = _mm_getcsr();
    csr &= ~(0x3 << 13);
    csr |= round << 13;
    _mm_setcsr(csr);
    return 0;
}

int fegetenv(fenv_t *env) {
    fenv_t csr = _mm_getcsr();
    *env = csr;
    return 0;
}

int fesetenv(fenv_t *env) {
    _mm_setcsr(*env);
    return 1;
}

int feholdexcept(fenv_t *envp) {
    fegetenv(envp);
    feclearexcept(FE_ALL_EXCEPT);
    return 0;
}

int feupdateenv(fenv_t const *envp) {
    int excepts = _mm_getcsr() & FE_ALL_EXCEPT;
    _mm_setcsr(*envp);
    feraiseexcept(excepts);
    return 0;
}
