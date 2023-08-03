
#include <cia-def.h>
#include <bin/elf.h>
#include <sys/syscall.h>

struct Elf_Image typedef Elf_Image;
struct Elf_Image {
    u8 *base;
    u8 *phdr;
    u64 ph_num;
    u64 ph_ent;
    char *name;
};

void loader_entry(u64 *sp, u64 *dynv, u64 *aux) {
    char str[] = "Hello world??\n";
    sys_write(STDOUT_FILENO, str, sizeof str);
    sys_exit(0);
}
