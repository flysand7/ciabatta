
// Stage 2 of the dynamic loader
// the purpose of this module is to relocate itself
// and jump to the main body of the loader.

// Note that loader-entry.asm doesn't "jump" into
// the entry point of this module, but rather
// "falls-through". Which means the sections
// of this module and the module above must be
// contiguous, which we do by doing linker magic

// This also means that this module CANNOT have
// any externally-defined functions (static functions
// are fine with gcc and clang (I think))

#include <cia-def.h>
#include <bin/elf.h>
#include <sys/syscall.h>
#include <stdarg.h>

#define AUX_CNT 32
#define DYN_CNT 37

static void print_string_n(char *str, u64 len) {
    sys_write(STDOUT_FILENO, str, len);
}
static void print_char(char c) {
    print_string_n(&c, 1);
}
static void print_string(char *str) {
    int str_len = 0;
    while(str[str_len] != 0) {
        str_len += 1;
    }
    print_string_n(str, str_len);
}
static void print_int(i64 number) {
    if(number < 0) {
        print_char('-');
        number = -number;
    }
    char buf[20];
    buf[19] = 0;
    char *p = buf + sizeof buf - 1;
    do {
        *--p = (number%10) + '0';
        number /= 10;
    } while(number > 0);
    print_string(p);
}
static void print_uint(u64 number) {
    char buf[20];
    buf[19] = 0;
    char *p = buf + sizeof buf - 1;
    do {
        *--p = (number%10) + '0';
        number /= 10;
    } while(number > 0);
    print_string(p);
}
static void print_hex(u64 number) {
    // print_string("0x");
    char digits[] = "0123456789abcdef";
    char buf[20];
    buf[19] = 0;
    char *p = buf + sizeof buf - 1;
    for(int i = 0; i < 64; i += 4) {
        if(i != 0 && i % 16 == 0) {
            *--p = '_';
        }
        u8 bits = (number >> i) & 0x0f;
        char digit = digits[bits];
        *--p = digit;
    }
    print_string(p);
}
static void printf(char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char str_buf[256];
    i64 buf_i = 0;
    while(*fmt != 0) {
        while(*fmt != '%' && *fmt != 0 && buf_i != sizeof str_buf-1) {
            str_buf[buf_i] = *fmt;
            buf_i += 1;
            fmt++;
        }
        str_buf[buf_i] = 0;
        print_string_n(str_buf, buf_i);
        buf_i = 0;
        if(*fmt == '%') {
            ++fmt;
            if(*fmt == 'd') {
                i64 number = va_arg(args, i64);
                print_int(number);
            }
            else if(*fmt == 'u') {
                u64 number = va_arg(args, u64);
                print_uint(number);
            }
            else if(*fmt == 'x') {
                u64 number = va_arg(args, u64);
                print_hex(number);
            }
            else if(*fmt == 's') {
                char *str = va_arg(args, char *);
                print_string(str);
            }
            else if(*fmt == 'c') {
                int c = va_arg(args, int);
                print_char(c);
            }
            ++fmt;
        }
    }
    va_end(args);
}

#if defined(_DEBUG) || 1
    #define _dbg_print_char(c)       print_char(c)
    #define _dbg_print_string(s)     print_string(s)
    #define _dbg_print_string_n(s,n) print_string_n(s,n)
    #define _dbg_print_int(d)        print_int(d)
    #define _dbg_print_uint(u)       print_uint(u)
    #define _dbg_print_hex(x)        print_hex(x)
    #define _dbg_printf(fmt, ...)    printf(fmt, ##__VA_ARGS__)
#else
    #define _dbg_print_char(c)       do {} while(0)
    #define _dbg_print_string(s)     do {} while(0)
    #define _dbg_print_string_n(s,n) do {} while(0)
    #define _dbg_print_int(d)        do {} while(0)
    #define _dbg_print_uint(u)       do {} while(0)
    #define _dbg_print_hex(x)        do {} while(0)
    #define _dbg_printf(fmt, ...)    do {} while(0)
#endif

extern void loader_entry(u64 *sp, u64 *dyn, u64 *aux);

void _dlstart_reloc_c(u64 *sp, Elf64_Dyn *dynv) {
    _dbg_print_string("Entered dynamic loader\n");
    // Find argc, argv in stack
    int argc = *sp;
    char **argv = (void *)(sp+1);
    // Skip over environment
    char **envp = argv+argc+1;
    int envc = 0;
    for(int i = 0; envp[i]; i += 1) {
        envc += 1;
    }
    // Load aux vector, which is responsible
    // for holding information for the loader such as
    // the location of phdrs or its load address
    u64 aux[AUX_CNT];
    Elf64_Aux *auxv = (Elf64_Aux *)(envp + envc + 1);
    for(int i = 0; i < AUX_CNT; ++i) {
        aux[i] = 0;
    }
    for(int i = 0; auxv[i].a_type != AT_NULL; ++i) {
        int aux_type = auxv[i].a_type;
        if(aux_type < AUX_CNT) {
            aux[aux_type] = auxv[i].a_val;
        }
    }
    // Read the _DYNAMIC vector
    u64 dyn[DYN_CNT];
    for(int i = 0; i < DYN_CNT; ++i) {
        dyn[i] = 0;
    }
    _dbg_printf("Dynamic section contents:\n");
    for(int i = 0; dynv[i].d_tag != DT_NULL; ++i) {
        u64 dyn_tag = dynv[i].d_tag;
        _dbg_printf("  %x: %x\n", dyn_tag, dynv[i].d_un.d_val);
        if(dyn_tag < DYN_CNT) {
            dyn[dyn_tag] = dynv[i].d_un.d_val;
        }
    }
    // Find the dynamic section
    if(dynv == NULL) {
        _dbg_printf("_DYNAMIC section wasn't found in dynamic loader\n");
    }
    // If the dynamic linker is not invoked as a command
    // It's load address can't be found in the aux vector
    // Therefore we compute the base as the difference
    // between &_DYNAMIC and it's p_vaddr
    u8 *base = (void *)aux[AT_BASE];
    if(base == NULL) {
        _dbg_printf("trying to find base\n");
        u8 *phdrs = (void *)aux[AT_PHDR];
        u64 ph_ent = aux[AT_PHENT];
        u64 ph_num = aux[AT_PHNUM];
        for(int i = 1; i < ph_num; ++i) {
            u64 ph_off = i * ph_ent;
            Elf64_Phdr *ph = (void *)(phdrs + ph_off);
            if(ph->p_type == PT_DYNAMIC) {
                _dbg_printf("_DYNAMIC vaddr: %x\n", ph->p_vaddr);
                base = (void *)(dynv - ph->p_vaddr);
            }
        }
    }
    _dbg_printf("ld-cia.so base: %x\n", base);
    _dbg_printf("ld-cia.so _dynamic: %x\n", dynv);
    Elf64_Sym *symtab = symtab = (void *)(base + dyn[DT_SYMTAB]);
    if(symtab == NULL) {
        _dbg_printf("ERROR: No .dynsym found\n");
    }
    // Apply relocations
    if(dyn[DT_REL] != 0) {
        _dbg_printf("REL Relocations found. This part isn't implemented\n");
        u8 *rel_ents = (void *)(base + dyn[DT_REL]);
        u64 rel_ent = dyn[DT_RELENT];
        u64 rel_size = dyn[DT_RELSZ];
        u64 rel_offs = 0;
        while(rel_offs < rel_size) {
            Elf64_Rel *rel = (void *)(rel_ents + rel_offs);
            u64 offs = rel->r_offset;
            u32 sym = ELF64_R_SYM(rel->r_info);
            u32 type = ELF64_R_TYPE(rel->r_info);
            _dbg_printf("  rel sym: %d type %d\n", sym, type);
            // TODO: if needed
            rel_offs += rel_ent;
        }
    }
    if(dyn[DT_RELA] != 0) {
        _dbg_printf("RELA Relocations found\n");
        u8 *rela_ents = (void *)(base + dyn[DT_RELA]);
        u64 rela_ent = dyn[DT_RELAENT];
        u64 rela_size = dyn[DT_RELASZ];
        u64 rela_offs = 0;
        while(rela_offs < rela_size) {
            Elf64_Rela *rela = (void *)(rela_ents + rela_offs);
            u64 reloc_offs = rela->r_offset;
            u64 addend = rela->r_addend;
            u32 sym_idx = ELF64_R_SYM(rela->r_info);
            u32 type = ELF64_R_TYPE(rela->r_info);
            _dbg_printf("  rela sym: %x, rela offs: %x, type: %d, addend: %d\n",
                sym_idx, reloc_offs, type, addend
            );
            if(type == R_X86_64_GLOB_DAT) {
                Elf64_Sym *sym = &symtab[sym_idx];
                void *sym_addr = (void *)(base + sym->st_value);
                void **reloc_addr = (void *)(base + reloc_offs);
                _dbg_printf("  -> resolving with %x\n", sym_addr);
                *reloc_addr = sym_addr;
            }
            rela_offs += rela_ent;
        }
    }
    if(dyn[DT_PLTGOT] != 0) {
        _dbg_printf("PLT relocations found\n");
        if(dyn[DT_PLTREL] == DT_REL) {
            _dbg_printf("  PLT relocations use relocations of type REL\n");
        }
        else if(dyn[DT_PLTREL] == DT_RELA) {
            _dbg_printf("  PLT relocations use relocations of type RELA\n");
        }
        _dbg_printf("plt: %x, rela.plt: %x, rela.pltsz: %x\n", dyn[DT_PLTGOT], dyn[DT_JMPREL], dyn[DT_PLTRELSZ]);
        void *plt = (void *)(base + dyn[DT_PLTGOT]);
        void *rela_plt = (void *)(base + dyn[DT_JMPREL]);
        u64 rela_ent = sizeof(Elf64_Rela);
        u64 rela_size = dyn[DT_PLTRELSZ];
        u64 rela_offs = 0;
        while(rela_offs < rela_size) {
            Elf64_Rela *rela = (void *)(rela_plt + rela_offs);
            u64 reloc_offs = rela->r_offset;
            u64 addend = rela->r_addend;
            u32 sym_idx = ELF64_R_SYM(rela->r_info);
            u32 type = ELF64_R_TYPE(rela->r_info);
            _dbg_printf("  rela sym: %x, rela offs: %x, type: %d, addend: %d\n",
                sym_idx, reloc_offs, type, addend
            );
            if(type == R_X86_64_JUMP_SLOT) {
                Elf64_Sym *sym = &symtab[sym_idx];
                void *sym_addr = (void *)(base + sym->st_value);
                void **reloc_addr = (void *)(base + reloc_offs);
                _dbg_printf("  -> resolving with %x\n", sym_addr);
                *reloc_addr = sym_addr;
            }
            rela_offs += rela_ent;
            _dbg_printf("%x\n", rela_offs);
        }
        
    }
    _dbg_printf("Self-relocation finished. Entering the loader\n");
    loader_entry(sp, dyn, aux);
    sys_exit(0);
}

