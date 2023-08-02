#!/bin/bash

nasm \
    -f elf64 \
    -o bin/loader-entry.o \
    src/loader/loader-entry.asm

clang -static -shared \
    -nostdlib \
    -ffreestanding \
    -I src/include \
    -o lib/ld.so \
    -Wl,-e,_dlstart \
    bin/loader-entry.o src/loader/loader.c
