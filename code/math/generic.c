
#include <math.h>
#include <fenv.h>
#include <errno.h>
#include <stdint.h>

#define ln2 0.69314718055994530941723212145817

#define ftype float
#define suffix(name) name ## f
#include "generic.h"
#undef ftype
#undef suffix

#define ftype double
#define suffix(name) name
#include "generic.h"
#undef ftype
#undef suffix

#define ftype long double
#define suffix(name) name ## l
#include "generic.h"
#undef ftype
#undef suffix
