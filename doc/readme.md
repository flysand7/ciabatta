
# Ciabatta

Ciabatta is a C standard library implementation. The goal is to provide a
cross-platform implementation that provides it's own documentation, clarifying
implementation-defined details, as well as provides additional functionality,
like sockets, or unicode processing, the kind of functionality that is used
in different kinds of programs.

## Headers

The implementation doesn't provide all headers. The headers that aren't
provided are supposed to be provided by compiler. The list of such headers:

- float.h
- iso646.h
- limits.h
- stdalign.h
- stdarg.h
- stdbool.h
- stddef.h
- stdint.h
- stdnoreturn.h

The headers that are provided have documentation included:

- [assert.h](assert.md)
- [complex.h](complex.md)
- [ctype.h](ctype.md)
- [errno.h](errno.md)
- [fenv.h](fenv.md)
- [inttypes.h](inttypes.md)
- [locale.h](locale.md)
- [math.h](math.md)
- [signal.h](signal.md)
- [stdio.h](stdio.md)
- [stdlib.h](stdlib.md)
- [string.h](string.md)
- [tgmath.h](tgmath.md)
- [threads.h](threads.md)
- [time.h](time.md)
- [uchar.h](uchar.md)
- [wchar.h](wchar.md)
- [wctype.h](wctype.md)

## Locales

Not supported. The `char *` strings are assumed to be encoded using ASCII/UTF-8
encoding.
