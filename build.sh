#!/bin/bash

[ ! -d "lib" ] && mkdir "lib"
[ ! -d "bin" ] && mkdir "bin"

[ "$1" != "-shared" ] && echo "static"

nasm -f elf64 "src/linux/crt_entry.asm" -o "bin/crt_entry.o"
clang -fPIC -nostdlib -I "include" -g "src/linux/crt_ctors.c" -c -o "bin/crt_ctors.o"
clang -fPIC -nostdlib -I "include" -g "src/ciabatta.c" -c -o "bin/ciabatta.o"

# Create a test executable
clang -pie -nostdlib -Iinclude \
    tests/empty.c bin/ciabatta.o bin/crt_ctors.o bin/crt_entry.o
