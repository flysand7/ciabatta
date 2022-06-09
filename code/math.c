
#include <stdint.h>

#define ftype        float
#define itype        uint32_t
#define f_bits       32
#define f_sig_bits   24
#define f_exp_max    127
#define suffix(name) #name ## "f"
#include "math/floats.h"
#include "math/basic.h"
#include "math/exponential.h"
#include "math/hyperbolic.h"
#include "math/nearest-int.h"
#include "math/power.h"
#include "math/probability-statistics.h"
#undef ftype
#undef itype
#undef f_bits
#undef f_sig_bits
#undef f_exp_max
#undef suffix

#define ftype        double
#define itype        uint64_t
#define f_bits       64
#define f_sig_bits   53
#define f_exp_max    1023
#define suffix(name) #name
#include "math/basic.h"
#include "math/exponential.h"
#include "math/floats.h"
#include "math/hyperbolic.h"
#include "math/nearest-int.h"
#include "math/power.h"
#include "math/probability-statistics.h"
#undef ftype
#undef itype
#undef f_bits
#undef f_sig_bits
#undef f_exp_max
#undef suffix
