
#pragma once

#include <stdint.h>
#include <limits.h>

// typedef struct imaxdiv_t imaxdiv_t
// It's C's fault not mine

#define _spec8 "hh"
#define _spec16 ""
#define _spec32 ""

#if INT64_MAX == LONG_MAX
    #define _spec64 "l"
#elif INT64_MAX == LLONG_MAX
    #define _spec64 "ll"
#endif

#if INT8_LEAST_MAX == CHAR_MAX
    #define _spec8l "hh"
#elif INT8_LEAST_MAX == SHORT_MAX || INT8_LEAST_MAX==INT_MAX
    #define _spec8l ""
#elif INT8_LEAST_MAX == LONG_MAX
    #define _spec8l "l"
#elif INT8_LEAST_MAX == LLONG_MAX
    #define _spec8l "ll"
#endif

#if INT16_LEAST_MAX == CHAR_MAX
    #define _spec16l "hh"
#elif INT16_LEAST_MAX == SHORT_MAX || INT16_LEAST_MAX==INT_MAX
    #define _spec16l ""
#elif INT16_LEAST_MAX == LONG_MAX
    #define _spec16l "l"
#elif INT16_LEAST_MAX == LLONG_MAX
    #define _spec16l "ll"
#endif

#if INT32_LEAST_MAX == CHAR_MAX
    #define _spec32l "hh"
#elif INT32_LEAST_MAX == SHORT_MAX || INT32_LEAST_MAX==INT_MAX
    #define _spec32l ""
#elif INT32_LEAST_MAX == LONG_MAX
    #define _spec32l "l"
#elif INT32_LEAST_MAX == LLONG_MAX
    #define _spec32l "ll"
#endif

#if INT64_LEAST_MAX == CHAR_MAX
    #define _spec64l "hh"
#elif INT64_LEAST_MAX == SHORT_MAX || INT64_LEAST_MAX==INT_MAX
    #define _spec64l ""
#elif INT64_LEAST_MAX == LONG_MAX
    #define _spec64l "l"
#elif INT64_LEAST_MAX == LLONG_MAX
    #define _spec64l "ll"
#endif


#if INT8_FAST_MAX == CHAR_MAX
    #define _spec8f "hh"
#elif INT8_FAST_MAX == SHORT_MAX || INT8_FAST_MAX==INT_MAX
    #define _spec8f ""
#elif INT8_FAST_MAX == LONG_MAX
    #define _spec8f "l"
#elif INT8_FAST_MAX == LLONG_MAX
    #define _spec8f "ll"
#endif

#if INT16_FAST_MAX == CHAR_MAX
    #define _spec16f "hh"
#elif INT16_FAST_MAX == SHORT_MAX || INT16_FAST_MAX==INT_MAX
    #define _spec16f ""
#elif INT16_FAST_MAX == LONG_MAX
    #define _spec16f "l"
#elif INT16_FAST_MAX == LLONG_MAX
    #define _spec16f "ll"
#endif

#if INT32_FAST_MAX == CHAR_MAX
    #define _spec32f "hh"
#elif INT32_FAST_MAX == SHORT_MAX || INT32_FAST_MAX==INT_MAX
    #define _spec32f ""
#elif INT32_FAST_MAX == LONG_MAX
    #define _spec32f "l"
#elif INT32_FAST_MAX == LLONG_MAX
    #define _spec32f "ll"
#endif

#if INT64_FAST_MAX == CHAR_MAX
    #define _spec64f "hh"
#elif INT64_FAST_MAX == SHORT_MAX || INT64_FAST_MAX==INT_MAX
    #define _spec64f ""
#elif INT64_FAST_MAX == LONG_MAX
    #define _spec64f "l"
#elif INT64_FAST_MAX == LLONG_MAX
    #define _spec64f "ll"
#endif

#define PRId8       _spec8   "d"
#define PRId16      _spec16  "d"
#define PRId32      _spec32  "d"
#define PRId64      _spec64  "d"
#define PRIdFAST8   _spec8f  "d"
#define PRIdFAST16  _spec16f "d"
#define PRIdFAST32  _spec32f "d"
#define PRIdFAST64  _spec64f "d"
#define PRIdLEAST8  _spec8l  "d"
#define PRIdLEAST16 _spec16l "d"
#define PRIdLEAST32 _spec32l "d"
#define PRIdLEAST64 _spec64l "d"
#define PRIdPTR     _specptr "d"
#define PRIi8       _spec8   "i"
#define PRIi16      _spec16  "i"
#define PRIi32      _spec32  "i"
#define PRIi64      _spec64  "i"
#define PRIiFAST8   _spec8f  "i"
#define PRIiFAST16  _spec16f "i"
#define PRIiFAST32  _spec32f "i"
#define PRIiFAST64  _spec64f "i"
#define PRIiLEAST8  _spec8l  "i"
#define PRIiLEAST16 _spec16l "i"
#define PRIiLEAST32 _spec32l "i"
#define PRIiLEAST64 _spec64l "i"
#define PRIiPTR     _specptr "i"
#define PRIo8       _spec8   "o"
#define PRIo16      _spec16  "o"
#define PRIo32      _spec16  "o"
#define PRIo64      _spec64  "o"
#define PRIoLEAST8  _spec8l  "o"
#define PRIoLEAST16 _spec16l "o"
#define PRIoLEAST32 _spec32l "o"
#define PRIoLEAST64 _spec64l "o"
#define PRIoFAST8   _spec8f  "o"
#define PRIoFAST16  _spec16f "o"
#define PRIoFAST32  _spec32f "o"
#define PRIoFAST64  _spec64f "o"
#define PRIoPTR     _specptr "o"
#define PRIu8       _spec8   "u"
#define PRIu16      _spec16  "u"
#define PRIu32      _spec16  "u"
#define PRIu64      _spec64  "u"
#define PRIuLEAST8  _spec8l  "u"
#define PRIuLEAST16 _spec16l "u"
#define PRIuLEAST32 _spec32l "u"
#define PRIuLEAST64 _spec64l "u"
#define PRIuFAST8   _spec8f  "u"
#define PRIuFAST16  _spec16f "u"
#define PRIuFAST32  _spec32f "u"
#define PRIuFAST64  _spec64f "u"
#define PRIuPTR     _specptr "u"
#define PRIx8       _spec8   "x"
#define PRIx16      _spec16  "x"
#define PRIx32      _spec32  "x"
#define PRIx64      _spec64  "x"
#define PRIxLEAST8  _spec8l  "x"
#define PRIxLEAST16 _spec16l "x"
#define PRIxLEAST32 _spec32l "x"
#define PRIxLEAST64 _spec64l "x"
#define PRIxFAST8   _spec8f  "x"
#define PRIxFAST16  _spec16f "x"
#define PRIxFAST32  _spec32f "x"
#define PRIxFAST64  _spec64f "x"
#define PRIxPTR     _specptr "x"
#define PRIX8       _spec8   "X"
#define PRIX16      _spec16  "X"
#define PRIX32      _spec32  "X"
#define PRIX64      _spec64  "X"
#define PRIXLEAST8  _spec8l  "X"
#define PRIXLEAST16 _spec16l "X"
#define PRIXLEAST32 _spec32l "X"
#define PRIXLEAST64 _spec64l "X"
#define PRIXFAST8   _spec8f  "X"
#define PRIXFAST16  _spec16f "X"
#define PRIXFAST32  _spec32f "X"
#define PRIXFAST64  _spec64f "X"
#define PRIXPTR     _specptr "X"

// TODO: scan formats for signed and unsigned integers
