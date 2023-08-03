
#include <cia-def.h>
#include <bin/elf.h>
#include "syscall.c"

void loader_entry(u64 *sp, Elf64_Dyn *dynv, u64 *aux) {
    char str[] = "Hello world??\n";
    _syscall_write(STDOUT_FILENO, str, sizeof str);
    _syscall_exit(0);
}
