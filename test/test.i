
#line 3 "w:/workspace/cuik/crt/include/xmmintrin.h"	
/* line   3 */	typedef _Vector ( float , 4 ) __m128 ; 
/* line   4 */	typedef _Vector ( unsigned char , 16 ) __m128i ; 
/* line   7 */	float __builtin_ia32_sqrtss ( __m128 a ) ; 
/* line   8 */	__m128 __builtin_ia32_sqrtps ( __m128 a ) ; 
/* line   9 */	float __builtin_ia32_rsqrtss ( __m128 a ) ; 
/* line  10 */	__m128 __builtin_ia32_rsqrtps ( __m128 a ) ; 
/* line  11 */	__m128 __builtin_ia32_rcpss ( __m128 a , __m128 b ) ; 
/* line  12 */	__m128 __builtin_ia32_rcpps ( __m128 a , __m128 b ) ; 
/* line  13 */	__m128 __builtin_ia32_minss ( __m128 a , __m128 b ) ; 
/* line  14 */	__m128 __builtin_ia32_minps ( __m128 a , __m128 b ) ; 
/* line  15 */	__m128 __builtin_ia32_maxss ( __m128 a , __m128 b ) ; 
/* line  16 */	__m128 __builtin_ia32_maxps ( __m128 a , __m128 b ) ; 
/* line  18 */	static inline __m128 _mm_add_ss ( __m128 a , __m128 b ) { 
/* line  19 */	a [ 0 ] += b [ 0 ] ; 
/* line  20 */	return a ; 
/* line  21 */	} 
/* line  23 */	static inline __m128 _mm_sub_ss ( __m128 a , __m128 b ) { 
/* line  24 */	a [ 0 ] -= b [ 0 ] ; 
/* line  25 */	return a ; 
/* line  26 */	} 
/* line  28 */	static inline __m128 _mm_mul_ss ( __m128 a , __m128 b ) { 
/* line  29 */	a [ 0 ] *= b [ 0 ] ; 
/* line  30 */	return a ; 
/* line  31 */	} 
/* line  33 */	static inline __m128 _mm_div_ss ( __m128 a , __m128 b ) { 
/* line  34 */	a [ 0 ] /= b [ 0 ] ; 
/* line  35 */	return a ; 
/* line  36 */	} 
/* line  38 */	static inline __m128 _mm_add_ps ( __m128 a , __m128 b ) { return a + b ; } 
/* line  39 */	static inline __m128 _mm_sub_ps ( __m128 a , __m128 b ) { return a - b ; } 
/* line  40 */	static inline __m128 _mm_mul_ps ( __m128 a , __m128 b ) { return a * b ; } 
/* line  41 */	static inline __m128 _mm_div_ps ( __m128 a , __m128 b ) { return a / b ; } 
/* line  42 */	static inline __m128 _mm_and_ps ( __m128 a , __m128 b ) { return a & b ; } 
/* line  43 */	static inline __m128 _mm_or_ps ( __m128 a , __m128 b ) { return a | b ; } 
/* line  44 */	static inline __m128 _mm_xor_ps ( __m128 a , __m128 b ) { return a ^ b ; } 
/* line  45 */	static inline __m128 _mm_andnot_ps ( __m128 a , __m128 b ) { return ~ ( a & b ) ; } 
/* line  47 */	static inline __m128 _mm_sqrt_ss ( __m128 a ) { return ( __m128 ) __builtin_ia32_sqrtss ( a ) ; } 
/* line  48 */	static inline __m128 _mm_sqrt_ps ( __m128 a ) { return __builtin_ia32_sqrtps ( a ) ; } 
/* line  50 */	static inline __m128 _mm_rsqrt_ss ( __m128 a ) { return ( __m128 ) __builtin_ia32_rsqrtss ( a ) ; } 
/* line  51 */	static inline __m128 _mm_rsqrt_ps ( __m128 a ) { return __builtin_ia32_rsqrtps ( a ) ; } 
/* line  53 */	static inline __m128 _mm_rcp_ss ( __m128 a ) { return ( __m128 ) __builtin_ia32_rcpss ( a ) ; } 
/* line  54 */	static inline __m128 _mm_rcp_ps ( __m128 a ) { return __builtin_ia32_rcpps ( a ) ; } 
/* line  56 */	static inline __m128 _mm_min_ss ( __m128 a , __m128 b ) { return __builtin_ia32_minss ( a , b ) ; } 
/* line  57 */	static inline __m128 _mm_min_ps ( __m128 a , __m128 b ) { return __builtin_ia32_minps ( a , b ) ; } 
/* line  58 */	static inline __m128 _mm_max_ss ( __m128 a , __m128 b ) { return __builtin_ia32_maxss ( a , b ) ; } 
/* line  59 */	static inline __m128 _mm_max_ps ( __m128 a , __m128 b ) { return __builtin_ia32_maxps ( a , b ) ; } 
/* line  61 */	static inline __m128 _mm_setr_ps ( float z , float y , float x , float w ) { return ( __m128 ) { z , y , x , w } ; } 
/* line  62 */	static inline __m128 _mm_setzero_ps ( void ) { return ( __m128 ) { 0 } ; } 
/* line  65 */	static inline __m128i _mm_loadu_si128 ( const __m128i * src ) { return * src ; } 
/* line  66 */	static inline __m128i _mm_load_si128 ( const __m128i * src ) { return * src ; } 
/* line  67 */	static inline void _mm_store_si128 ( __m128i * dst , __m128i src ) { * dst = src ; } 
#line 3 "w:/workspace/cuik/crt/include/intrin.h"	
/* line   3 */	extern void __debugbreak ( void ) ; 
#line 5 "w:/workspace/ciabatta/inc/assert.h"	
/* line   5 */	extern void _assert_error ( 
/* line   6 */	char * cond , 
/* line   7 */	char const * func , 
/* line   8 */	char const * file , 
/* line   9 */	char const * line 
/* line  10 */	) ; 
#line 3 "w:/workspace/ciabatta/inc/ctype.h"	
/* line   3 */	int isalnum ( int c ) ; 
/* line   4 */	int isalpha ( int c ) ; 
/* line   5 */	int isblank ( int c ) ; 
/* line   6 */	int iscntrl ( int c ) ; 
/* line   7 */	int isdigit ( int c ) ; 
/* line   8 */	int isgraph ( int c ) ; 
/* line   9 */	int islower ( int c ) ; 
/* line  10 */	int isprint ( int c ) ; 
/* line  11 */	int ispunct ( int c ) ; 
/* line  12 */	int isspace ( int c ) ; 
/* line  13 */	int isupper ( int c ) ; 
/* line  14 */	int isxdigit ( int c ) ; 
/* line  15 */	int tolower ( int c ) ; 
/* line  16 */	int toupper ( int c ) ; 
#line 3 "w:/workspace/cuik/crt/include/stdint.h"	
/* line   3 */	typedef signed char int8_t ; 
/* line   4 */	typedef short int16_t ; 
/* line   5 */	typedef int int32_t ; 
/* line   6 */	typedef long long int64_t ; 
/* line   7 */	typedef unsigned char uint8_t ; 
/* line   8 */	typedef unsigned short uint16_t ; 
/* line   9 */	typedef unsigned int uint32_t ; 
/* line  10 */	typedef unsigned long long uint64_t ; 
/* line  12 */	typedef signed char int_least8_t ; 
/* line  13 */	typedef short int_least16_t ; 
/* line  14 */	typedef int int_least32_t ; 
/* line  15 */	typedef long long int_least64_t ; 
/* line  16 */	typedef unsigned char uint_least8_t ; 
/* line  17 */	typedef unsigned short uint_least16_t ; 
/* line  18 */	typedef unsigned int uint_least32_t ; 
/* line  19 */	typedef unsigned long long uint_least64_t ; 
/* line  21 */	typedef signed char int_fast8_t ; 
/* line  22 */	typedef int int_fast16_t ; 
/* line  23 */	typedef int int_fast32_t ; 
/* line  24 */	typedef long long int_fast64_t ; 
/* line  25 */	typedef unsigned char uint_fast8_t ; 
/* line  26 */	typedef unsigned int uint_fast16_t ; 
/* line  27 */	typedef unsigned int uint_fast32_t ; 
/* line  28 */	typedef unsigned long long uint_fast64_t ; 
/* line  30 */	typedef long long intmax_t ; 
/* line  31 */	typedef unsigned long long uintmax_t ; 
#line 3 "w:/workspace/cuik/crt/include/stdarg.h"	
/* line   3 */	typedef char * va_list ; 
#line 6 "w:/workspace/ciabatta/inc/stdio.h"	
/* line   6 */	typedef struct FILE FILE ; 
/* line   7 */	typedef int64_t fpos_t ; 
/* line   9 */	typedef struct div_t div_t ; 
/* line  10 */	typedef struct ldiv_t ldiv_t ; 
/* line  11 */	typedef struct lldiv_t lldiv_t ; 
/* line  19 */	typedef int errno_t ; 
/* line  20 */	typedef size_t rsize_t ; 
/* line  57 */	extern FILE * stdout , * stderr , * stdin ; 
/* line  59 */	int remove ( const char * filename ) ; 
/* line  60 */	int rename ( const char * oldname , const char * newname ) ; 
/* line  61 */	FILE * tmpfile ( void ) ; 
/* line  62 */	char * tmpnam ( char * s ) ; 
/* line  63 */	int fclose ( FILE * stream ) ; 
/* line  64 */	int fflush ( FILE * stream ) ; 
/* line  65 */	FILE * fopen ( const char * restrict filename , const char * restrict mode ) ; 
/* line  66 */	FILE * freopen ( const char * restrict filename , const char * restrict mode , FILE * restrict stream ) ; 
/* line  67 */	void setbuf ( FILE * restrict stream , char * restrict buf ) ; 
/* line  68 */	int setvbuf ( FILE * restrict stream , char * restrict buf , int mode , size_t size ) ; 
/* line  69 */	int fprintf ( FILE * restrict stream , const char * restrict format , ... ) ; 
/* line  70 */	int fscanf ( FILE * restrict stream , const char * restrict format , ... ) ; 
/* line  71 */	int printf ( const char * restrict format , ... ) ; 
/* line  72 */	int scanf ( const char * restrict format , ... ) ; 
/* line  73 */	int snprintf ( char * restrict s , size_t n , const char * restrict format , ... ) ; 
/* line  74 */	int sprintf ( char * restrict s , const char * restrict format , ... ) ; 
/* line  75 */	int sscanf ( const char * restrict s , const char * restrict format , ... ) ; 
/* line  76 */	int vfprintf ( FILE * restrict stream , const char * restrict format , va_list arg ) ; 
/* line  77 */	int vfscanf ( FILE * restrict stream , const char * restrict format , va_list arg ) ; 
/* line  78 */	int vprintf ( const char * restrict format , va_list arg ) ; 
/* line  79 */	int vscanf ( const char * restrict format , va_list arg ) ; 
/* line  80 */	int vsnprintf ( char * restrict s , size_t n , const char * restrict format , va_list arg ) ; 
/* line  81 */	int vsprintf ( char * restrict s , const char * restrict format , va_list arg ) ; 
/* line  82 */	int vsscanf ( const char * restrict s , const char * restrict format , va_list arg ) ; 
/* line  83 */	int fgetc ( FILE * stream ) ; 
/* line  84 */	char * fgets ( char * restrict s , int n , FILE * restrict stream ) ; 
/* line  85 */	int fputc ( int c , FILE * stream ) ; 
/* line  86 */	int fputs ( const char * restrict s , FILE * restrict stream ) ; 
/* line  87 */	int getc ( FILE * stream ) ; 
/* line  88 */	int getchar ( void ) ; 
/* line  89 */	int putc ( int c , FILE * stream ) ; 
/* line  90 */	int putchar ( int c ) ; 
/* line  91 */	int puts ( const char * s ) ; 
/* line  92 */	int ungetc ( int c , FILE * stream ) ; 
/* line  93 */	size_t fread ( void * restrict ptr , size_t size , size_t nmemb , FILE * restrict stream ) ; 
/* line  94 */	size_t fwrite ( const void * restrict ptr , size_t size , size_t nmemb , FILE * restrict stream ) ; 
/* line  95 */	int fgetpos ( FILE * restrict stream , fpos_t * restrict pos ) ; 
/* line  96 */	int fseek ( FILE * stream , long int offset , int whence ) ; 
/* line  97 */	int fsetpos ( FILE * stream , const fpos_t * pos ) ; 
/* line  98 */	long int ftell ( FILE * stream ) ; 
/* line  99 */	void rewind ( FILE * stream ) ; 
/* line 100 */	void clearerr ( FILE * stream ) ; 
/* line 101 */	int feof ( FILE * stream ) ; 
/* line 102 */	int ferror ( FILE * stream ) ; 
/* line 103 */	void perror ( const char * s ) ; 
#line 13 "w:/workspace/cuik/crt/include/stddef.h"	
/* line  13 */	typedef long long ptrdiff_t ; 
/* line  14 */	typedef unsigned long long size_t ; 
/* line  15 */	typedef long double max_align_t ; 
/* line  18 */	typedef short wchar_t ; 
#line 18 "w:/workspace/ciabatta/inc/stdlib.h"	typedef struct div_t { 
/* line  19 */	int quot ; 
/* line  20 */	int rem ; 
/* line  21 */	} div_t ; 
/* line  23 */	typedef struct ldiv_t { 
/* line  24 */	long quot ; 
/* line  25 */	long rem ; 
/* line  26 */	} ldiv_t ; 
/* line  28 */	typedef struct lldiv_t { 
/* line  29 */	long long quot ; 
/* line  30 */	long long rem ; 
/* line  31 */	} lldiv_t ; 
/* line  44 */	double atof ( const char * nptr ) ; 
/* line  45 */	int atoi ( const char * nptr ) ; 
/* line  46 */	long int atol ( const char * nptr ) ; 
/* line  47 */	long long int atoll ( const char * nptr ) ; 
/* line  48 */	double strtod ( const char * restrict nptr , char * * restrict endptr ) ; 
/* line  49 */	float strtof ( const char * restrict nptr , char * * restrict endptr ) ; 
/* line  50 */	long double strtold ( const char * restrict nptr , char * * restrict endptr ) ; 
/* line  51 */	long int strtol ( const char * restrict nptr , char * * restrict endptr , int base ) ; 
/* line  52 */	long long int strtoll ( const char * restrict nptr , char * * restrict endptr , int base ) ; 
/* line  53 */	unsigned long int strtoul ( const char * restrict nptr , char * * restrict endptr , int base ) ; 
/* line  54 */	unsigned long long int strtoull ( const char * restrict nptr , char * * restrict endptr , int base ) ; 
/* line  55 */	int rand ( void ) ; 
/* line  56 */	void srand ( unsigned int seed ) ; 
/* line  58 */	typedef struct _os_heap _os_heap ; 
/* line  59 */	void _heap_setup ( _os_heap * heap ) ; 
/* line  60 */	void * aligned_alloc ( size_t alignment , size_t size ) ; 
/* line  61 */	void * calloc ( size_t nmemb , size_t size ) ; 
/* line  62 */	void free ( void * ptr ) ; 
/* line  63 */	void * malloc ( size_t size ) ; 
/* line  64 */	void * realloc ( void * ptr , size_t size ) ; 
/* line  66 */	_Noreturn void abort ( void ) ; 
/* line  67 */	int atexit ( void ( * func ) ( void ) ) ; 
/* line  69 */	_Noreturn void exit ( int status ) ; 
/* line  70 */	_Noreturn void _Exit ( int status ) ; 
/* line  71 */	char * getenv ( const char * name ) ; 
/* line  73 */	int system ( const char * string ) ; 
/* line  74 */	const void * bsearch ( const void * key , const void * base , size_t nmemb , size_t size , int ( * compar ) ( const void * , const void * ) ) ; 
#line 9 "w:/workspace/ciabatta/inc/errno.h"	
/* line   9 */	extern int errno ; 
#line 708 "w:/windows kits/10/include/10.0.22000.0/shared/sal.h"	
/* line 708 */	_Pragma ( "region Input Buffer SAL 1 compatibility macros" ) 
/* line 1472 */	_Pragma ( "endregion Input Buffer SAL 1 compatibility macros" ) 
#line 15 "w:/visual studio/2019/community/vc/tools/msvc/14.29.30133/include/vadefs.h"	
/* line  15 */	_Pragma ( "pack(push, _CRT_PACKING)" ) 
/* line  47 */	_Pragma ( "warning(push)" ) 
/* line  48 */	_Pragma ( "warning(disable: _VCRUNTIME_DISABLED_WARNINGS)" ) 
/* line  61 */	typedef unsigned 
/* line   1 */	long long 
/* line  61 */	uintptr_t ; 
/* line  72 */	typedef char * va_list ; 
/* line 155 */	void __cdecl __va_start ( va_list * , ... ) ; 
/* line 207 */	_Pragma ( "warning(pop)" ) 
/* line 208 */	_Pragma ( "pack(pop)" ) 
#line 60 "w:/visual studio/2019/community/vc/tools/msvc/14.29.30133/include/vcruntime.h"	
/* line  60 */	_Pragma ( "warning(push)" ) 
/* line  61 */	_Pragma ( "warning(disable: _VCRUNTIME_DISABLED_WARNINGS)" ) 
/* line   1 */	_Pragma ( "pack(push, _CRT_PACKING)" ) 
/* line 193 */	typedef unsigned 
/* line   1 */	long long 
/* line 193 */	size_t ; 
/* line 194 */	typedef 
/* line   1 */	long long 
/* line 194 */	ptrdiff_t ; 
/* line 195 */	typedef 
/* line   1 */	long long 
/* line 195 */	intptr_t ; 
/* line 209 */	typedef _Bool __vcrt_bool ; 
/* line 228 */	typedef unsigned short wchar_t ; 
/* line 342 */	void __cdecl __security_init_cookie ( void ) ; 
/* line 351 */	void __cdecl __security_check_cookie ( uintptr_t _StackCookie ) ; 
/* line 352 */	__declspec ( noreturn ) void __cdecl __report_gsfailure ( uintptr_t _StackCookie ) ; 
/* line 356 */	extern uintptr_t __security_cookie ; 
/* line   1 */	_Pragma ( "pack(pop)" ) 
/* line 366 */	_Pragma ( "warning(pop)" ) 
#line 13 "w:/visual studio/2019/community/vc/tools/msvc/14.29.30133/include/limits.h"	
/* line  13 */	_Pragma ( "warning(push)" ) 
/* line  14 */	_Pragma ( "warning(disable: _VCRUNTIME_DISABLED_WARNINGS)" ) 
/* line   1 */	_Pragma ( "pack(push, _CRT_PACKING)" ) _Pragma ( "pack(pop)" ) 
/* line  78 */	_Pragma ( "warning(pop)" ) 
#line 27 "w:/workspace/ciabatta/inc/string.h"	
/* line  27 */	void * memcpy ( void * restrict s1 , const void * restrict s2 , size_t n ) ; 
/* line  28 */	void * memmove ( void * s1 , const void * s2 , size_t n ) ; 
/* line  29 */	char * strcpy ( char * restrict s1 , const char * restrict s2 ) ; 
/* line  30 */	char * strncpy ( char * restrict s1 , const char * restrict s2 , size_t n ) ; 
/* line  31 */	char * strcat ( char * restrict s1 , const char * restrict s2 ) ; 
/* line  32 */	char * strncat ( char * restrict s1 , const char * restrict s2 , size_t n ) ; 
/* line  33 */	int memcmp ( const void * s1 , const void * s2 , size_t n ) ; 
/* line  34 */	int strcmp ( const char * s1 , const char * s2 ) ; 
/* line  35 */	int strcoll ( const char * s1 , const char * s2 ) ; 
/* line  36 */	int strncmp ( const char * s1 , const char * s2 , size_t n ) ; 
/* line  37 */	size_t strxfrm ( char * restrict s1 , const char * restrict s2 , size_t n ) ; 
/* line  38 */	void * memchr ( const void * s , int c , size_t n ) ; 
/* line  39 */	char * strchr ( const char * s , int c ) ; 
/* line  40 */	size_t strcspn ( const char * s1 , const char * s2 ) ; 
/* line  41 */	char * strpbrk ( const char * s1 , const char * s2 ) ; 
/* line  42 */	char * strrchr ( const char * s , int c ) ; 
/* line  43 */	size_t strspn ( const char * s1 , const char * s2 ) ; 
/* line  44 */	char * strstr ( const char * s1 , const char * s2 ) ; 
/* line  45 */	char * strtok ( char * restrict s1 , const char * restrict s2 ) ; 
/* line  46 */	void * memset ( void * s , int c , size_t n ) ; 
/* line  47 */	char * strerror ( int errnum ) ; 
/* line  48 */	size_t strlen ( const char * s ) ; 
/* line  51 */	char * strtok_r ( char * restrict s1 , const char * restrict s2 , char * * restrict ctx ) ; 
#line 4 "w:/workspace/ciabatta/inc/signal.h"	
/* line   4 */	typedef int sig_atomic_t ; 
/* line  25 */	void ( * signal ( int sig , void ( * func ) ( int ) ) ) ( int ) ; 
/* line  26 */	int raise ( int sig ) ; 
#line 13 "test/test.c"	
/* line  13 */	int test ( ) { 
/* line  14 */	static int a = 2 ; 
/* line  15 */	a += 1 ; 
/* line  16 */	return a ; 
/* line  17 */	} 
/* line  19 */	void wack ( ) { 
/* line  20 */	printf ( "BYE!!!\n" ) ; 
/* line  21 */	} 
/* line  23 */	int main ( int argc , char * * argv ) { 
/* line  24 */	atexit ( wack ) ; 
/* line  26 */	char * path_env = getenv ( "PATH" ) ; 
/* line  27 */	char * ctx , * path_token = strtok_r ( path_env , ";" , & ctx ) ; 
/* line  28 */	while ( path_token ) { 
/* line  29 */	printf ( "%s\n" , path_token ) ; 
/* line  30 */	path_token = strtok_r ( 
/* line   1 */	( ( void * ) 0 ) 
/* line  30 */	, ";" , & ctx ) ; 
/* line  31 */	} 
/* line  33 */	test ( ) ; 
/* line  34 */	char input [ ] = "A bird came down the walk" ; 
/* line  35 */	printf ( "Parsing the input string '%s'\n" , input ) ; 
/* line  36 */	char * token = strtok ( input , " " ) ; 
/* line  37 */	while ( token ) { 
/* line  38 */	printf ( "%s\n" , token ) ; 
/* line  39 */	token = strtok ( 
/* line   1 */	( ( void * ) 0 ) 
/* line  39 */	, " " ) ; 
/* line  40 */	} 
/* line  42 */	printf ( "Contents of the input string now: '" ) ; 
/* line  43 */	for ( size_t n = 0 ; n < sizeof input ; ++ n ) 
/* line  44 */	input [ n ] ? printf ( "%c" , input [ n ] ) : printf ( "\\0" ) ; 
/* line  45 */	printf ( "'" ) ; 
/* line  46 */	return 0 ; 
/* line  47 */	} 
/* line   1 */	 