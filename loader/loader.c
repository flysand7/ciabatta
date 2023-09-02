
#include <cia/def.h>

#include <bin/elf.h>
#include <sys/syscall.h>
#include <stdarg.h>
#include "loader.h"

#include <asm/prctl.h>
#include <futex.h>
#include <sys/mman.h>
#include <sched.h>
#include <errno.h>
#include <fcntl.h>
#include <tinyrt.h>
#include "../os/linux/tinyrt.c"

#include <cia/mem.h>
#include "../src/cia-mem/util.c"
#include "../src/cia-mem/allocator.c"
#include "../src/cia-mem/arena.c"
#include "../src/cia-mem/pool.c"

#include "stack.c"

struct Thread_Control_Block typedef Thread_Control_Block;
struct Thread_Control_Block {
    u64 thread_id;
    u64 pad0[4];
    u64 stack_canary;
    u64 pad1[2];
};

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
static Cia_Arena tmp_arena;

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

struct Stage3_Info_Struct typedef Stage3_Info_Struct;
struct Stage3_Info_Struct {
    Elf_Image *app;
    Elf_Image *ldso;
    void *stack_base;
    u64 stack_size;
    u64 tls_size;
};

static void ld_stage3_entry(u64 has_new_stack, void *ctx);

void ld_stack_trampoline(
    void *stack_base
    , void *old_stack_base
    , u64 stack_size
    , u64 old_stack_size
    , void (*fn)(u64 has_new_stack, void *ctx)
    , void *ctx
);

void ld_stage2_entry(Loader_Info *ld_info) {
    _dbg_printf("Loader entry point reached!\n");
    // Get our loader data back
    u64 *sp = ld_info->sp;
    u8 *ldso_base = ld_info->ldso_base;
    u64 *dyn = ld_info->dyn;
    u64 *aux = ld_info->aux;
    cia_pool_create(&image_pool, cia_allocator_pages(), 1*KB, sizeof(Elf_Image), 0x10);
    Elf_Image *ldso = cia_pool_alloc(&image_pool);
    Elf_Image *app  = cia_pool_alloc(&image_pool);
    // Save some stuff for stage3 to eat
    cia_arena_create(&tmp_arena, cia_allocator_pages(), 1*MB);
    Stage3_Info_Struct *stage3 = cia_arena_alloc(&tmp_arena, sizeof(Stage3_Info_Struct));
    stage3->app = app;
    stage3->ldso = ldso;
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
        Elf64_Sym *symtab = elf_addr(app, (void *)app->dyn[DT_SYMTAB]);
        char *strtab = elf_addr(app, app->dyn[DT_STRTAB]);
        _dbg_printf("APP STRTAB: %x\n", strtab);
        if(app->dyn[DT_REL] != 0) {
            printf("REL Relocations found. This part isn't implemented\n");
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
                sys_exit(0);
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
                u32 type = ELF64_R_TYPE(rela->r_info);
                Elf64_Sym *sym = &symtab[ELF64_R_SYM(rela->r_info)];
                void *sym_addr = elf_addr(app, (u64)sym->st_value);
                void **reloc_addr = elf_addr(app, reloc_offs);
                {
                    u32 sym_name_offset = sym->st_name;
                    if(sym_name_offset == 0) {
                        _dbg_printf("  %x+%d, @%x (%d)", sym_addr, addend, reloc_offs, type);
                    }
                    else {
                        char *sym_name = &strtab[sym_name_offset];
                        _dbg_printf("  %s+%d, @%x (%d)", sym_name, addend, reloc_offs, type);
                    }
                }
                if(type == R_X86_64_GLOB_DAT) {
                    *reloc_addr = sym_addr;
                }
                else if(type == R_X86_64_RELATIVE) {
                    *reloc_addr = elf_addr(app, addend);
                }
                else {
                    printf("ERROR: unhandled relocation type: %d\n", type);
                    sys_exit(1);
                }
                _dbg_printf("  -> %x\n", *reloc_addr);
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
        // Get the size of the TLS initialization image for the main modules
        u64 tls_size = 0;
        {
            Elf64_Phdr *ph_tls = NULL;
            for(u64 i = 0; i < app->ph_num; ++i) {
                Elf64_Phdr *ph = (void *)(app->phdr + i*app->ph_ent);
                if(ph->p_type == PT_TLS) {
                    ph_tls = ph;
                    break;
                }
            }
            if(ph_tls != NULL) {
                tls_size += ph_tls->p_memsz;
            }
        }
        _dbg_printf("TLS image size: %x\n", tls_size);
        // Get the information about the main thread stack
        if(linux_read_stack_info()) {
            printf("ERROR: failed to read /proc/self/maps to get the stack info\n");
            sys_exit(1);
        }
        _dbg_printf("Found default stack at: %x-%x\n", stack_info.start_addr, stack_info.end_addr);
        void *old_stack_base = (void *)stack_info.start_addr;
        u64 old_stack_size = (u64)stack_info.end_addr - (u64)stack_info.start_addr;
        u64 stack_size = 2*MB;
        void *stack_base = sys_mmap(0, stack_size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS|MAP_NORESERVE, -1, 0);
        stage3->stack_size = stack_size;
        stage3->stack_base = stack_base;
        stage3->tls_size = tls_size;
        _dbg_printf("stage3 info struct: %x\n", stage3);
        // Will jump to ld_stage3_entry
        ld_stack_trampoline(stack_base, old_stack_base, stack_size, old_stack_size, &ld_stage3_entry, stage3);
    }
    sys_exit(0);
}

static void ld_stage3_entry(u64 has_new_stack, void *ctx) {
    if(!has_new_stack) {
        printf("ERROR: failed to switch the stack\n");
        sys_exit(1);
    }
    Stage3_Info_Struct *info = ctx;
    _dbg_printf("Stack: %x-%x\n", info->stack_base, (u8 *)info->stack_base+info->stack_size);   
    // Set up the thread control block
    Thread_Control_Block *tcb = cia_ptr_alignf((u8*)info->stack_base + info->tls_size, 1*MB);
    tcb->thread_id = 0;
    tcb->stack_canary = 0x12345678fedcba98;
    // Copy TLS initialization image below TCB
    {
        Elf_Image *app = info->app;
        Elf64_Phdr *ph_tls = NULL;
        for(u64 i = 0; i < app->ph_num; ++i) {
            Elf64_Phdr *ph = (void *)(app->phdr + i*app->ph_ent);
            if(ph->p_type == PT_TLS) {
                ph_tls = ph;
                break;
            }
        }
        if(ph_tls != NULL) {
            u8 *tls_image_base = elf_addr(app, ph_tls->p_vaddr);
            u64 tls_image_size = ph_tls->p_memsz;
            u8 *tls_image = (u8*)tcb - tls_image_size;
            for(int i = 0; i < tls_image_size; ++i) {
                tls_image[i] = tls_image_base[i];
            }
        }
    }
    // Set up the thread pointer
    int err = sys_arch_prctl(ARCH_SET_FS, (u64)tcb);
    if(err < 0) {
        printf("ERROR: failed to set up the thread pointer\n");
        sys_exit(1);
    }
    _dbg_printf("Entered loader stage 3. Entering main executable\n");
    void (*crt_entry)() = elf_addr(info->app, ((Elf64_Ehdr *)info->app->base)->e_entry);
    crt_entry();
}
