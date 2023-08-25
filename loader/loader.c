
#include <cia-def.h>
#include <bin/elf.h>
#include <sys/syscall.h>
#include <stdarg.h>
#include "loader.h"

#include <sys/mman.h>
#include <sched.h>
#include <errno.h>
#include <fcntl.h>
#include <tinyrt.h>
#include "../os/linux/tinyrt.c"

#include <cia-mem.h>
#include "../src/cia-mem/util.c"
#include "../src/cia-mem/allocator.c"
#include "../src/cia-mem/arena.c"
#include "../src/cia-mem/pool.c"

struct Elf_Image typedef Elf_Image;
struct Elf_Image {
    Cia_Arena arena;
    char *name;
    u8 *base;
    u8 *phdr;
    u64 ph_num;
    u64 ph_ent;
    u8 *shdr;
    u64 sh_num;
    u64 sh_ent;
    u64 *dyn;
};

#define elf_addr(elf, off) (void *)((elf)->base + (u64)off)

static Cia_Pool image_pool;

static u32 elf_sym_gnu_hash(char *name) {
    unsigned char *s = (void *)name;
    u32 h = 5381;
    for(int i = 0; s[i] != 0; ++i) {
        char c = s[i];
        h = ((h << 5) + h) + c;
    }
    return h;
}

static u64 elf_sym_hash(char *name) {
    unsigned char *s = (void *)name;
    u64 h = 0;
    u64 g;
    for(int i = 0; s[i] != 0; ++i) {
        h = (h<<4) + s[i];
        g = (h & 0xf0000000);
        if(g) {
            h ^= g >> 24;
        }
        h &= 0x0fffffff;
    }
    return h;
}

static void elf_load(Elf_Image *image, int fd) {
    printf("ERROR: image loading from file not implemented\n");
    sys_exit(1);
}

static Elf64_Sym *elf_symbol_by_name(Elf_Image *image, char *name) {
    Elf64_Ehdr *eh = (void *)image->base;
    u8 *shdr = elf_addr(image, eh->e_shoff);
    u64 sh_num = eh->e_shnum;
    u64 sh_ent = eh->e_shentsize;
    // Get the GNU symbol hash table
    Elf64_Shdr *sh_gnu_hash = NULL;
    for(int i = 0; i < sh_num; ++i) {
        u64 sh_off = i * sh_ent;
        Elf64_Shdr *sh = (void *)(shdr + sh_off);
        if(sh->sh_type == SHT_GNU_HASH) {
            sh_gnu_hash = sh;
        }
    }
    // Check if we got gnu hash table
    _dbg_printf("gnu hash section: %x\n", sh_gnu_hash->sh_addr);
    if(sh_gnu_hash != NULL) {
    }
    return NULL;
}

void loader_entry(Loader_Info *ld_info) {
    _dbg_printf("Loader entry point reached!\n");
    // Get our loader data back
    u64 *sp = ld_info->sp;
    u8 *ldso_base = ld_info->ldso_base;
    u64 *dyn = ld_info->dyn;
    u64 *aux = ld_info->aux;
    cia_pool_create(&image_pool, cia_allocator_pages(), 1*KB, sizeof(Elf_Image), 0x10);
    Elf_Image *ldso = cia_pool_alloc(&image_pool);
    Elf_Image *app  = cia_pool_alloc(&image_pool);
    cia_arena_create(&app->arena, cia_allocator_pages(), 1*MB);
    ldso->base = ldso_base;
    ldso->dyn = dyn;
    // Read ldso elf header
    {
        Elf64_Ehdr *eh = (void *)ldso->base;
        ldso->phdr = elf_addr(ldso, eh->e_phoff);
        ldso->ph_num = eh->e_phnum;
        ldso->ph_ent = eh->e_phentsize;
        ldso->name = "ld-cia.so";
    }
    int fd = 0;
    app->name = (void *)aux[AT_EXECFN];
    if(aux[AT_PHDR] != (u64)ldso->base) {
        _dbg_printf("Linux loaded the image with phdrs at: %x\n", aux[AT_PHDR]);
        // Figure out executable base from its phdrs
        u8 *phdr = (void *)aux[AT_PHDR];
        u64 ph_ent = aux[AT_PHENT];
        u64 ph_num = aux[AT_PHNUM];
        for(u64 i = 0; i < ph_num; ++i) {
            u64 ph_off = i * ph_ent;
            Elf64_Phdr *ph = (void *)(phdr + ph_off);
            if(ph->p_type == PT_PHDR) {
                app->base = (void *)(aux[AT_PHDR] - ph->p_vaddr);
            }
        }
        _dbg_printf("app base: %x\n", app->base);
        app->phdr = phdr;
        app->ph_ent = ph_ent;
        app->ph_num = ph_num;
    }
    else {        
        _dbg_printf("Exec filename: %x\n", aux[AT_EXECFN]);
        char *filename = (void *)aux[AT_EXECFN];
        app->name = filename;
        if(filename == NULL) {
            printf("ERROR: no data about the executable to load dynamically\n");
            sys_exit(1);
        }
        int fd = sys_open(filename, O_CLOEXEC, O_RDWR);
        if(fd < 0) {
            printf("ERROR: cannot open file %s\n", filename);
            sys_exit(1);
        }
        elf_load(app, fd);
    }
    if(fd != 0) {
        sys_close(fd);
    }
    // Find .dynamic section
    {
        u8 *phdr = (void *)aux[AT_PHDR];
        u64 ph_ent = aux[AT_PHENT];
        u64 ph_num = aux[AT_PHNUM];
        Elf64_Phdr *ph_dynamic = NULL;
        for(u64 i = 0; i < ph_num; ++i) {
            u64 ph_off = i * ph_ent;
            Elf64_Phdr *ph = (void *)(phdr + ph_off);
            if(ph->p_type == PT_DYNAMIC) {
                ph_dynamic = ph;
            }
        }
        // Read and decode dynamic vector
        Elf64_Dyn *dynv = elf_addr(app, ph_dynamic->p_vaddr);
        u64 dyn[DYN_CNT];
        for(int i = 0; i < DYN_CNT; ++i) {
            dyn[i] = 0;
        }
        for(int i = 0; dynv[i].d_tag != DT_NULL; ++i) {
            i64 d_tag = dynv[i].d_tag;
            if(d_tag < DYN_CNT) {
                dyn[0] |= 1ul << d_tag;
                dyn[d_tag] = dynv[i].d_un.d_val;
            }
        }
        app->dyn = &dyn[0];
    }
    // Load section headers for app
    _dbg_printf("Loading section headers\n");
    {
    }
    // Relocate app
    {
        _dbg_printf("Relocating app\n");
        Elf64_Ehdr *eh = (void *)app->base;
        // Resolve relocations
        Elf64_Sym *symtab = (void *)app->dyn[DT_SYMTAB];
        if(app->dyn[DT_REL] != 0) {
            _dbg_printf("REL Relocations found. This part isn't implemented\n");
            u8 *rel_ents = elf_addr(app, app->dyn[DT_REL]);
            u64 rel_ent = app->dyn[DT_RELENT];
            u64 rel_size = app->dyn[DT_RELSZ];
            u64 rel_offs = 0;
            while(rel_offs < rel_size) {
                Elf64_Rel *rel = (void *)(rel_ents + rel_offs);
                u64 offs = rel->r_offset;
                u32 sym = ELF64_R_SYM(rel->r_info);
                u32 type = ELF64_R_TYPE(rel->r_info);
                _dbg_printf("  %d @ %d (%d)\n", sym, offs, type);
                // TODO: if needed
                rel_offs += rel_ent;
            }
        }
        if(app->dyn[DT_RELA] != 0) {
            _dbg_printf("RELA:\n");
            u8 *rela_ents = elf_addr(app, app->dyn[DT_RELA]);
            u64 rela_ent = app->dyn[DT_RELAENT];
            u64 rela_size = app->dyn[DT_RELASZ];
            u64 rela_offs = 0;
            while(rela_offs < rela_size) {
                Elf64_Rela *rela = (void *)(rela_ents + rela_offs);
                u64 reloc_offs = rela->r_offset;
                u64 addend = rela->r_addend;
                u32 sym_idx = ELF64_R_SYM(rela->r_info);
                u32 type = ELF64_R_TYPE(rela->r_info);
                _dbg_printf("  %x+%d, @%x (%d)\n", sym_idx, addend, reloc_offs, type);
                if(type == R_X86_64_GLOB_DAT) {
                    Elf64_Sym *sym = &symtab[sym_idx];
                    void *sym_addr = elf_addr(app, sym->st_value);
                    void **reloc_addr = elf_addr(app, reloc_offs);
                    *reloc_addr = sym_addr;
                    _dbg_printf("  -> %x\n", sym_addr);
                }
                else if(type == R_X86_64_RELATIVE) {
                    void *addr = elf_addr(app, addend);
                    void **reloc_addr = elf_addr(app, reloc_offs);
                    *reloc_addr = addr;
                    _dbg_printf("  -> %x\n", addr);
                }
                else {
                    printf("ERROR: unhandled relocation type: %d\n", type);
                    sys_exit(1);
                }
                rela_offs += rela_ent;
            }
        }
        if(app->dyn[DT_PLTGOT] != 0) {
            _dbg_printf("PLT relocations found\n");
            if(app->dyn[DT_PLTREL] == DT_REL) {
                _dbg_printf("  PLT relocations use relocations of type REL\n");
                printf("ERROR: .plt relocations of type REL not implemented\n");
                sys_exit(1);
            }
            else if(app->dyn[DT_PLTREL] == DT_RELA) {
                _dbg_printf("  PLT relocations use relocations of type RELA\n");
            }
            void *plt = elf_addr(app, app->dyn[DT_PLTGOT]);
            void *rela_plt = elf_addr(app, app->dyn[DT_JMPREL]);
            u64 rela_ent = sizeof(Elf64_Rela);
            u64 rela_size = app->dyn[DT_PLTRELSZ];
            u64 rela_offs = 0;
            while(rela_offs < rela_size) {
                Elf64_Rela *rela = (void *)(rela_plt + rela_offs);
                u64 reloc_offs = rela->r_offset;
                u64 addend = rela->r_addend;
                u32 sym_idx = ELF64_R_SYM(rela->r_info);
                u32 type = ELF64_R_TYPE(rela->r_info);
                _dbg_printf("  %x+%d, @%x (%d)\n", sym_idx, addend, reloc_offs, type);
                if(type == R_X86_64_JUMP_SLOT) {
                    Elf64_Sym *sym = &symtab[sym_idx];
                    void *sym_addr = elf_addr(app, sym->st_value);
                    void **reloc_addr = elf_addr(app, reloc_offs);
                    _dbg_printf("  -> resolving with %x\n", sym_addr);
                    *reloc_addr = sym_addr;
                }
                rela_offs += rela_ent;
            }
        }
        _dbg_printf("%x\n", 0);
        // Get the app main
        // Elf64_Sym *app_main = elf_symbol_by_name(app, "main");
        // _dbg_printf("app main: %x\n", app_main);
        void (*crt_entry)() = elf_addr(app, eh->e_entry);
        _dbg_printf("Exiting the dynamic loader, trying to enter the main app\n");
        crt_entry();
    }
    sys_exit(0);
}
