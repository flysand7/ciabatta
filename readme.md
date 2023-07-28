
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

On linux you can simply run `./build.lua` script. On windows you have to run
it like `lua build.lua`. Reference for command-line options:

```
Usage: build.py [-h] [--clean] [--test TEST]
                [--mode {debug,release}] [--target TARGET]
                [--compiler {clang,cuik}] [--cflags [CFLAGS ...]]
Options:
  -h, --help                  show this help message and exit
  --clean                     Remove all object files and binaries
  --test TEST                 Compile ciabatta executable with given file
  --mode, -m {debug,release}  Select build configuration
  --target, -t TARGET         Select target OS (default=host)
  --compiler, -c {clang,cuik} Select compiler
  --cflags [CFLAGS ...]       Pass additional compiler flags
```

## Usage

Grab the following files into your project's (or any other) directory:

- The `./include` folder
- The `.lib` folder
- (Windows only) The `utf8` folder

In order to compile your project with ciabatta see the following sections

### Compiling with ciabatta on windows

1. Add the following flags to your compilation command:
   `-nostdlib -D _CIA_OS_WINDOWS -I /include utf8.obj -mfma`
2. Add the following sources to the compile command:
   `./lib/ciabatta.lib ./lib/cia.lib ./lib/crt.lib`

**Note:** The `include` folder refers to the folder you copied from ciabatta. Set the path to it accordingly.

### Compiling with ciabatta on linux

1. Add the following flags to your compilation command:
   - `-nostdlib -D _CIA_OS_LINUX -I ./include -mfma`
2. Link to the following libraries
   - `./lib/ciabatta.a`

## Contributing

Pull requests welcome and accepted in any form.

## License

See [the license file](license)
