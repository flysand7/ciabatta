#!/bin/bash

[ ! -d "lib" ] && mkdir "lib"
[ ! -d "bin" ] && mkdir "bin"

nasm -f elf64 "src/linux/crt_entry.asm" -o "bin/crt_entry.o"
clang -fPIC -nostdlib -I "include" -g "src/linux/crt_ctors.c" -c -o "bin/crt_ctors.o"
clang -fPIC -nostdlib -I "include" -g "src/ciabatta.c" -c -o "bin/ciabatta.o"

rm "$LIB_FILE" 2> /dev/null

if [ "$1" != "-shared" ]; then
    llvm-ar -q "lib/ciabatta.a" "bin/crt_ctors.o" "bin/crt_entry.o" "bin/ciabatta.o"
else
    clang -fPIC -nostdlib -shared -o "lib/ciabatta.so" "bin/ciabatta.o"
    cp "bin/crt_ctors.o" "lib/ctors.o"
    cp "bin/crt_entry.o" "lib/entry.o"
fi
