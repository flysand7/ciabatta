
// clang src\unity.c -o ciabatta.lib -c -I inc -I fdec64 -I unicope\inc -I src\win -nodefaultlibs -g  -mfma -U__STDC_HOSTED__

// Platform-independent stuff
#include "code/fmt/gen_fmt.c"
#include "code/math/basic.c"
#include "code/math/division.c"
#include "code/math/gen_math.c"
#include "code/math/ieee754.c"
#include "code/math/round.c"
#include "code/stdlib/algorithm.c"
#include "code/stdlib/conversion.c"
#include "code/stdlib/multibyte.c"
#include "code/stdlib/random.c"
#include "code/ctype.c"
#include "code/errno.c"
#include "code/fenv.c"
#include "code/locale.c"
#include "code/string.c"
#include "code/uchar.c"
#include "code/wctype.c"

// Windows stuff
#include "win/win_assert.c"
#include "win/win_environment.c"
#include "win/win_heap.c"
#include "win/win_signal.c"
#include "win/win_stack_trace.c"
#include "win/win_stdio.c"
#include "win/win_threads.c"
#include "win/win_time.c"
