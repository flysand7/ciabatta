
#pragma once

#include <stdint.h>

#define PRIi8       "hhi"
#define PRIu8       "hhu"
#define PRIo8       "hho"
#define PRId8       "hhd"
#define PRIx8       "hhx"
#define PRIX8       "hhX"

#define PRIi16      "hi"
#define PRIu16      "hu"
#define PRIo16      "ho"
#define PRId16      "hd"
#define PRIx16      "hx"
#define PRIX16      "hX"

#define PRIi32      "i"
#define PRIu32      "u"
#define PRIo32      "o"
#define PRId32      "d"
#define PRIx32      "x"
#define PRIX32      "X"

#if defined(_WIN64)
    #define PRIi64  "lli"
    #define PRIu64  "llu"
    #define PRIo64  "llo"
    #define PRId64  "lld"
    #define PRIx64  "llx"
    #define PRIX64  "llX"
#else
    #define PRIi64  "li"
    #define PRIu64  "lu"
    #define PRIo64  "lo"
    #define PRId64  "ld"
    #define PRIx64  "lx"
    #define PRIX64  "lX"
#endif

#define PRIdLEAST8  "hhi"
#define PRIuLEAST8  "hhu"
#define PRIoLEAST8  "hho"
#define PRIiLEAST8  "hhd"
#define PRIxLEAST8  "hhx"
#define PRIXLEAST8  "hhX"

#define PRIiLEAST16 "hi"
#define PRIuLEAST16 "hu"
#define PRIoLEAST16 "ho"
#define PRIdLEAST16 "hd"
#define PRIxLEAST16 "hx"
#define PRIXLEAST16 "hX"

#define PRIiLEAST32 "i"
#define PRIuLEAST32 "u"
#define PRIoLEAST32 "o"
#define PRIdLEAST32 "d"
#define PRIxLEAST32 "x"
#define PRIXLEAST32 "X"

#if defined(_WIN32)
    #define PRIiLEAST64 "lli"
    #define PRIuLEAST64 "llu"
    #define PRIoLEAST64 "llo"
    #define PRIdLEAST64 "lld"
    #define PRIxLEAST64 "llx"
    #define PRIXLEAST64 "llX"
#else
    #define PRIiLEAST64 "li"
    #define PRIuLEAST64 "lu"
    #define PRIoLEAST64 "lo"
    #define PRIdLEAST64 "ld"
    #define PRIxLEAST64 "lx"
    #define PRIXLEAST64 "lX"
#endif

#define PRIiFAST8   "i"
#define PRIuFAST8   "u"
#define PRIoFAST8   "o"
#define PRIdFAST8   "d"
#define PRIxFAST8   "x"
#define PRIXFAST8   "X"

#define PRIiFAST16  "i"
#define PRIuFAST16  "u"
#define PRIoFAST16  "o"
#define PRIdFAST16  "d"
#define PRIxFAST16  "x"
#define PRIXFAST16  "X"

#define PRIiFAST32  "i"
#define PRIuFAST32  "u"
#define PRIoFAST32  "o"
#define PRIdFAST32  "d"
#define PRIxFAST32  "x"
#define PRIXFAST32  "X"

#if defined(_WIN32)
    #define PRIiFAST64  "lli"
    #define PRIuFAST64  "llu"
    #define PRIoFAST64  "llo"
    #define PRIdFAST64  "lld"
    #define PRIxFAST64  "llx"
    #define PRIXFAST64  "llX"
    #define PRIiPTR     "lli"
    #define PRIuPTR     "llu"
    #define PRIoPTR     "llo"
    #define PRIdPTR     "lld"
    #define PRIxPTR     "llx"
    #define PRIXPTR     "llX"
    #define PRIiMAX     "lli"
    #define PRIuMAX     "llu"
    #define PRIoMAX     "llo"
    #define PRIdMAX     "lld"
    #define PRIxMAX     "llx"
    #define PRIXMAX     "llX"
#else
    #define PRIiFAST64  "li"
    #define PRIuFAST64  "lu"
    #define PRIoFAST64  "lo"
    #define PRIdFAST64  "ld"
    #define PRIxFAST64  "lx"
    #define PRIXFAST64  "lX"
    #define PRIiPTR     "li"
    #define PRIuPTR     "lu"
    #define PRIoPTR     "lo"
    #define PRIdPTR     "ld"
    #define PRIxPTR     "lx"
    #define PRIXPTR     "lX"
    #define PRIiMAX     "li"
    #define PRIuMAX     "lu"
    #define PRIoMAX     "lo"
    #define PRIdMAX     "ld"
    #define PRIxMAX     "lx"
    #define PRIXMAX     "lX"
#endif

#define SCNi8       "hhi"
#define SCNu8       "hhu"
#define SCNo8       "hho"
#define SCNd8       "hhd"
#define SCNx8       "hhx"

#define SCNiLEAST8  "hhi"
#define SCNuLEAST8  "hhu"
#define SCNoLEAST8  "hho"
#define SCNdLEAST8  "hhd"
#define SCNxLEAST8  "hhx"

#define SCNdFAST8   "hhd"
#define SCNiFAST8   "hhi"
#define SCNoFAST8   "hho"
#define SCNuFAST8   "hhu"
#define SCNxFAST8   "hhx"

#define SCNi16      "hi"
#define SCNu16      "hu"
#define SCNo16      "ho"
#define SCNd16      "hd"
#define SCNx16      "hx"

#define SCNiLEAST16 "hi"
#define SCNuLEAST16 "hu"
#define SCNoLEAST16 "ho"
#define SCNdLEAST16 "hd"
#define SCNxLEAST16 "hx"

#define SCNiFAST16  "i"
#define SCNuFAST16  "u"
#define SCNoFAST16  "o"
#define SCNdFAST16  "d"
#define SCNxFAST16  "x"

#define SCNi32      "i"
#define SCNu32      "u"
#define SCNo32      "o"
#define SCNd32      "d"
#define SCNx32      "x"

#define SCNiFAST32  "i"
#define SCNuFAST32  "u"
#define SCNoFAST32  "o"
#define SCNdFAST32  "d"
#define SCNxFAST32  "x"

#define SCNiLEAST32 "i"
#define SCNuLEAST32 "u"
#define SCNoLEAST32 "o"
#define SCNdLEAST32 "d"
#define SCNxLEAST32 "x"

#if defined(_WIN32)
    #define SCNi64      "lli"
    #define SCNu64      "llu"
    #define SCNo64      "llo"
    #define SCNd64      "lld"
    #define SCNx64      "llx"

    #define SCNiLEAST64 "lli"
    #define SCNuLEAST64 "llu"
    #define SCNoLEAST64 "llo"
    #define SCNdLEAST64 "lld"
    #define SCNxLEAST64 "llx"

    #define SCNiFAST64  "lli"
    #define SCNuFAST64  "llu"
    #define SCNoFAST64  "llo"
    #define SCNdFAST64  "lld"
    #define SCNxFAST64  "llx"

    #define SCNiMAX     "lli"
    #define SCNuMAX     "llu"
    #define SCNoMAX     "llo"
    #define SCNdMAX     "lld"
    #define SCNxMAX     "llx"

    #define SCNiPTR     "lli"
    #define SCNuPTR     "llu"
    #define SCNoPTR     "llo"
    #define SCNdPTR     "lld"
    #define SCNxPTR     "llx"
#else
    #define SCNi64      "li"
    #define SCNu64      "lu"
    #define SCNo64      "lo"
    #define SCNd64      "ld"
    #define SCNx64      "lx"

    #define SCNiLEAST64 "li"
    #define SCNuLEAST64 "lu"
    #define SCNoLEAST64 "lo"
    #define SCNdLEAST64 "ld"
    #define SCNxLEAST64 "lx"

    #define SCNiFAST64  "li"
    #define SCNuFAST64  "lu"
    #define SCNoFAST64  "lo"
    #define SCNdFAST64  "ld"
    #define SCNxFAST64  "lx"

    #define SCNiMAX     "li"
    #define SCNuMAX     "lu"
    #define SCNoMAX     "lo"
    #define SCNdMAX     "ld"
    #define SCNxMAX     "lx"

    #define SCNiPTR     "li"
    #define SCNuPTR     "lu"
    #define SCNoPTR     "lo"
    #define SCNdPTR     "ld"
    #define SCNxPTR     "lx"
#endif

#if defined(_WIN32)
    typedef unsigned short wchar_t;
#else
    typedef int wchar_t;
#endif

typedef struct imaxdiv_t imaxdiv_t;
struct imaxdiv_t {
    intmax_t quot;
    intmax_t rem;
};

intmax_t  imaxabs(intmax_t j);
imaxdiv_t imaxdiv(intmax_t numer, intmax_t denom);
intmax_t  strtoimax(const char *restrict s, char **restrict end, int base);
uintmax_t strtoumax(const char *restrict s, char ** restrict end, int base);
intmax_t  wcstoimax(const wchar_t *restrict s, wchar_t **restrict end, int base);
uintmax_t wcstoumax(const wchar_t *restrict s, wchar_t **restrict end, int base);

