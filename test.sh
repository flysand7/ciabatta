#!/bin/bash

if [ "$1" != "-shared" ]; then
    clang -static -nostdlib tests/empty.c lib/ciabatta.a -Iinclude
else
    clang -fPIE tests/empty.c -c -o tests/empty.o
    ld -no-pie -nostdlib lib/entry.o tests/empty.o lib/ciabatta.so lib/ctors.o -Iinclude
fi