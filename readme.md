
# Ciabatta

![Linux](https://github.com/flysand7/ciabatta/actions/workflows/build-linux.yml/badge.svg?event=push)
![Windows](https://github.com/flysand7/ciabatta/actions/workflows/build-windows.yml/badge.svg?event=push)

Ciabatta - An implementation of cross-platform C standard library with the
following goals:

- Providing C standard library that fully implements all C features. Some
  standard libraries lack many features of C11 and C23, like threads.h or
  aligned_alloc in case of MSVCRT.
- Making standard library that is easy to port to other platforms, for example
  an embedded platform or a custom operating system.
- Allowing applications to debug and step into standard library functions
  (Hello MSVCRT!)
- Reasonably fast CRT compared to MSVCRT and glibc, e.g. making malloc use
  a faster allocator, avoid runtime checks where possible, heavely rely on
  inlining.
- Extend the possibilities of C standard library with commonly used
  functionality: implementing POSIX standard, capability for unicode
  processing, API to traverse directories, hash functions, better strings API,
  crash handlers.

Ciabatta is not binary-compatible with other CRT libraries. That means that any
libraries that your project uses have to also be compiled with Ciabatta,
otherwise you will run into issues.

Please note that as of today ciabatta is still during development and does not
implement many of the features that need to be implemented. Using it at current
time is discouraged.

## Current Status

### C23 standard library

| Header          | Description                          | Status      |
| --------------- | ------------------------------------ | ----------- |
| <assert.h>      | Diagnostics.                         | none        |
| <complex.h>     | Complex arithmetic.                  | none        |
| <ctype.h>       | Character handling.                  | none        |
| <errno.h>       | Errors.                              | none        |
| <fenv.h>        | Floating-point environment.          | none        |
| <float.h>       | Characteristics of floating types.   | none        |
| <inttypes.h>    | Format conversion of integer types.  | none        |
| <iso646.h>      | Alternative spellings.               | none        |
| <limits.h>      | Sizes of integer types.              | none        |
| <locale.h>      | Localization.                        | none        |
| <math.h>        | Mathematics.                         | none        |
| <setjmp.h>      | Nonlocal jumps.                      | [1]         |
| <signal.h>      | Signal handling.                     | none        |
| <stdalign.h>    | Alignment.                           | none        |
| <stdarg.h>      | Variable arguments.                  | none        |
| <stdatomic.h>   | Atomics.                             | none        |
| <stdbit.h>      | Bit and byte utilities               | none        |
| <stdbool.h>     | Boolean type and values.             | none        |
| <stddef.h>      | Common definitions.                  | none        |
| <stdint.h>      | Integer types.                       | Implemented |
| <stdio.h>       | Input/output.                        | WIP         |
| <stdlib.h>      | General utilities.                   | none        |
| <stdnoreturn.h> | `_Noreturn`.                         | none        |
| <string.h>      | String handling.                     | none        |
| <tgmath.h>      | Type-generic math.                   | none        |
| <threads.h>     | Threads.                             | none        |
| <time.h>        | Date and time.                       | none        |
| <uchar.h>       | Unicode utilities.                   | none        |
| <wchar.h>       | Wide character utilities.            | none        |
| <wctype.h>      | Wide character utilities.            | none        |

[1] Implemented by compilers

## Support

Operating System:
- Windows
- Linux

Processor Architecture:
- x86-64

This library is supposed to be extensible to other platforms, meaning that
you can write an OS layer for another OS and use the rest of the CRT
functionality.

## Building ciabatta

Before proceeding please note that ciabatta can only be compiled and used
with `clang` and `cuik`. It may be able to work with `gcc` with some minor adjustments
but I didn't test.

You can get `cuik` in [the GitHub repository](https://github.com/RealNeGate/Cuik)

For executing the script you will need at least python 3.3 and the pyjson5 package

```
$ pip install pyjson5
```

### Building

On linux you can simply run `./build.py` script. On windows can run
it with `py build.py` command.

## Usage

Grab the following files into your project's (or any other) directory:

- The `./include` folder
- The `./lib` folder
- (Windows only) The `./utf8` folder

In order to compile your project with ciabatta see the following sections,
assuming you put all files mentioned above in the `./ciabatta` folder

### Compiling with ciabatta on windows

1. Add the following flags to your compilation command:
   `-nostdlib -I ./ciabatta/include utf8.obj -mfma`
2. Add the following sources to the compile command:
   `./ciabatta/lib/cia.lib`

**Note:** The `include` folder refers to the folder you copied from ciabatta. Set the path to it accordingly.

### Compiling with ciabatta on linux

1. Add the following flags to your compilation command:
   - `-nostdlib -I ./ciabatta/include -mfma`
2. Link to the following libraries
   - `./ciabatta/lib/cia.a`
3. Specify the ciabatta dynamic loader in linker options
   - `-Wl,-dynamic-linker,lib/ld-cia.so`

## Contributing

Pull requests welcome and accepted in any form.

## License

See [the license file](license)
