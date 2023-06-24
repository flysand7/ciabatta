
# Ciabatta

![Build Status](https://github.com/github/docs/actions/workflows/main.yml/badge.svg?event=push)

Ciabatta - An implementation of cross-platform C standard library with the
following goals:

- Providing C standard library that fully implements all C features. Some
  standard libraries lack many features of C11 and C23, like threads.h or
  aligned_alloc in case of msvcrt.
- Making standard library that is easy to port to other platforms, for example
  an embedded platform or a custom operating system.
- Allowing applications to debug and step into standard library functions
- Reasonably fast CRT compared to MSVCRT and glibc.
- Extend the possibilities of C standard library with commonly used
  functionality: implementing POSIX standard (including directories and
  sockets), capability for unicode processing.

Ciabatta is not binary-compatible with other CRT libraries. That means that any
libraries that your project uses have to also be compiled with Ciabatta,
otherwise you might run into issues.

Please note that as of today ciabatta is still during development and does not
implement many of the features that need to be implemented. Using it at current
time is discouraged.

## Support

Operating System:
- Windows
- Linux (planned)

Processor Architecture:
- x86-64

This library is supposed to be extensible to other platforms, meaning that
you can write an OS layer for another OS and use the rest of the CRT
functionality.

## Building & Usage

Building ciabatta with MSVC is not supported. The only compiler that the
library have been compiled and tested with is clang.

Note that the library can only be used with clang

1. Run ./build.cmd to compile ciabatta
  - (Optionally) Run `./test crt` to make sure there are no errors
2. Grab the following files into your project's directory:
  - The `./include` folder
  - The `./ciabatta.lib` archive file
  - The `./utf8.obj` object file
3. Add the following flags to your compilation command:
  `-I ./include utf8.obj -nostdlib -mfma`
4. Don't forget to link to the following libraries:
  `-lciabatta.lib`

## Contributing

Pull requests welcome and accepted in any form.

## License

TBD
