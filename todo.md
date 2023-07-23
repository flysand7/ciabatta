
general:
*   Start writing documentation concerning various implementation defined
    behaviours of the library.
*   (MSVC Ext.) Different kinds of main functions: wmain, WinMain, wWinMain, DllMain
*   (MSVC Ext.) envp argument for main functions

math.h:
*   exp
*   exp2
*   expml
*   log
*   log10
*   log2
*   log1p
*   pow
*   cbrt
*   hypot
*   atan2
*   erf
*   erfc
*   tgamma
*   lgamma
*   frexp
*   ldexp
*   M_* constants
*   M_E
*   M_LOG2
*   M_LOG10
*   other stuff?

stdint.h:
*   Implement 7.20.3

stdio.h:
*   The only reason to keep a linked list of all streams is to make sure they
    are flushed after main() returns. I wonder if only remembering the files
    with a buffer would increase performance.
*   Formatted scan
*   %s precision should specify how much characters to *write*
*   %s check for NULL
*   %Ls this is an actual weird territory. If (wchar_t *) is a unicode string
    then to determine it's "width" (to figure out field padding) would require
    to use a Unicode composition algorithm. But the problem is that windows
    Unicode rendering advances text by 2 units, even if it's 1 graphical
    character that was composed from 2 unicode characters. The question
    becomes: do we implement the thing correctly by using a more complex
    solution, or do we succumb to windows' bullshittery. I guess the answer is
    obvious. That's why I wrote this todo item :D
*   tmpfile
*   tmpnam
*   tmpfile_s
*   tmpnam_s
*   Printing 8.98846567431157854073e+307
*   Printing negative zero
*   Add functions for testing a file path, creating directories
*   Fix the bug where if a file wasn't closed the thing crashes when
    trying to flush the buffer

stdlib.h:
*   Strtod base 16 must be correctly rounded
*   Multibyte string functions
*   Better PRNG
*   Fix aligned_malloc
*   Constraint Handlers
*   getenv_s
*   aligned_alloc can't be used with free()

string.h:
*   strnlen_s
*   memset_s
*   memmove_s
*   memcpy_s
*   strncpy_s
*   strcpy_s
*   strcat_s
*   strncat_s
*   strtok_s
*   strerror_s
*   strerrorlen_s

tgmath.h:
*   Due to the use of conditional operator in type-generic macros the
    expression always expands to the value of type `long double`. This leads
    to warnings when used with printf, e.g. as in printf("%f\n", exp(0)).
    Using _Generic macros isn't a good choice either, because it won't work
    with typedef'd types.
    In other words there's no pure-C way to do tgmath.h.
    That's probably the reason msvc stdlib headers don't implement tgmath.h.
    clang implements it with __attribute__((__overloadable__)).

threads.h:
*   TODO: add todo items

time.h:
*   Probably some other funcs
*   asctime_s
*   ctime_s
*   gmtime_s
*   localtime_s

wchar.h:
*   Collation, IO, Tokenization, Conversions

