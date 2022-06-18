
#include <math.h>
#include <fenv.h>
#include <errno.h>
#include <stdint.h>

#define ln2 0.69314718055994530941723212145817
static double HALF_PI   = 1.570796326794896619231321691639751;
static double PI        = 3.141592653589793238462643383279502;
static double LOG2E     = 1.442695040888963407359924681001892;

#define countof(arr) (sizeof arr/sizeof arr[0])

#define ftype float
#define suffix(name) name ## f
#include "cordic/cordic_dataf.c"
#include "generic.h"
#undef ftype
#undef suffix

#define ftype double
#define suffix(name) name
#include "cordic/cordic_data.c"
#include "generic.h"
#undef ftype
#undef suffix

#define ftype long double
#define suffix(name) name ## l
#include "cordic/cordic_datal.c"
#include "generic.h"
#undef ftype
#undef suffix
