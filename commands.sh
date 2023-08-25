==> Compiling lib/ld-cia.so
  $ nasm -f "elf64" "loader/loader-entry.asm" -o "bin/loader-entry.o"
  $ clang -nostdlib -fno-stack-protector -g -O0 -fpic -D _DEBUG -I "include" -I "include/linux" -shared -nostdlib -ffreestanding -fno-stack-protector -Wl,-e,_dlstart -Wl,--sort-section,alignment -Wl,--sort-common -Wl,--gc-sections -Wl,--hash-style=both -Wl,--no-undefined -Wl,--exclude-libs=ALL  "bin/loader-entry.o" "loader/loader-self-reloc.c" "loader/loader.c" -o "lib/ld-cia.so"
==> Compiling crt.lib
  $ clang -nostdlib -fno-stack-protector -g -O0 -fpic -D _DEBUG -I "include" -I "include/linux" -c  "os/linux/crt-ctors.c" -o "bin/crt-ctors.o"
  $ llvm-ar -rcs "lib/crt.lib" "bin/crt-ctors.o"
==> Compiling cia.lib
  $ clang -nostdlib -fno-stack-protector -g -O0 -fpic -D _DEBUG -I "include" -I "include/linux" -c  "include/cia.c" -o "bin/cia.o"
  $ llvm-ar -rcs "lib/cia.lib" "bin/cia.o"
  $ clang -nostdlib -fno-stack-protector -g -O0 -fpic -D _DEBUG -I "include" -I "include/linux" -pie -Wl,-dynamic-linker,lib/ld-cia.so -fno-stack-protector  "tests/hello.c" "lib/crt.lib" "lib/cia.lib" -o "a"
