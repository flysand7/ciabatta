
# Ciabatta

![Build Status](https://github.com/flysand7/ciabatta/actions/workflows/build.yml/badge.svg?event=push)

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
- Linux

Processor Architecture:
- x86-64

This library is supposed to be extensible to other platforms, meaning that
you can write an OS layer for another OS and use the rest of the CRT
functionality.

## Building ciabatta

Before proceeding please note that ciabatta can only be compiled and used
with `clang`. It may be able to work with `gcc` with some minor adjustments
but I didn't test.

### Building on windows

Run `./build.ps1` to compile ciabatta
- (Optionally) Run `./test crt` to make sure there are no errors
  
### Building and using on linux

There are two options: Use ciabatta as a shared library or as a static
library.

Run `./build.sh` to compile ciabatta
- If you wish to use shared library instead add `-shared` option to the build script

## Usage

Grab the following files into your project's (or any other) directory:

- The `./include` folder
- The `.lib` folder
- (Windows only) The `utf8` folder

In order to compile your project with ciabatta see the following sections

### Compiling with ciabatta on windows

1. Add the following flags to your compilation command:
   `-nostdlib -I ./include utf8.obj -mfma`
2. Link to the following libraries:
   `-l ./lib/ciabatta.lib`

**Note:** The `include` folder refers to the folder you copied from ciabatta. Set the path to it accordingly.

### Compiling with ciabatta on linux

- In case of static linking:
  1. Add the following flags to your compilation command:
     - `-nostdlib -static -I ./include`
  2. Link to the following libraries
     - `./lib/ciabatta.a`
- In case of dynamic linking:
  1. Add the following flags to your compilation command:
     - `-nostdlib -no-pie -I ./include`
  2. Link to the following libraries:
     - `./lib/ciabatta.so ./lib.ctors.o ./lib.entry.o`

## Contributing

Pull requests welcome and accepted in any form.

## License

See [the license file](license)
