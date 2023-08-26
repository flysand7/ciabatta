
#pragma once

#include <cia/def.h>

typedef u64 Elf64_Addr;
typedef u64 Elf64_Off;
typedef u16 Elf64_Half;
typedef u32 Elf64_Word;
typedef i32 Elf64_Sword;
typedef u64 Elf64_Xword;
typedef i64 Elf64_Sxword;

// e_type definitions
#define ET_NONE   0x0000
#define ET_REL    0x0001
#define ET_EXEC   0x0002
#define ET_DYN    0x0003
#define ET_CORE   0x0004
#define ET_LOOS   0xfe00
#define ET_HIOS   0xfeff
#define ET_LOPROC 0xff00
#define ET_HIPROC 0xffff

// e_machine definitions
#define EM_NONE               0
#define EM_M32                1
#define EM_SPARC              2
#define EM_386                3
#define EM_68K                4
#define EM_88K                5
#define EM_IAMCU              6
#define EM_860                7
#define EM_MIPS               8
#define EM_S370               9
#define EM_MIPS_RS3_LE       10
#define EM_PARISC            15
#define EM_VPP500            17
#define EM_SPARC32PLUS       18
#define EM_960               19
#define EM_PPC               20
#define EM_PPC64             21
#define EM_S390              22
#define EM_SPU               23
#define EM_V800              36
#define EM_FR20              37
#define EM_RH32              38
#define EM_RCE               39
#define EM_ARM               40
#define EM_ALPHA             41
#define EM_SH                42
#define EM_SPARCV9           43
#define EM_TRICORE           44
#define EM_ARC               45
#define EM_H8_300            46
#define EM_H8_300H           47
#define EM_H8S               48
#define EM_H8_500            49
#define EM_IA_64             50
#define EM_MIPS_X            51
#define EM_COLDFIRE          52
#define EM_68HC12            53
#define EM_MMA               54
#define EM_PCP               55
#define EM_NCPU              56
#define EM_NDR1              57
#define EM_STARCORE          58
#define EM_ME16              59
#define EM_ST100             60
#define EM_TINYJ             61
#define EM_X86_64            62
#define EM_PDSP              63
#define EM_PDP10             64
#define EM_PDP11             65
#define EM_FX66              66
#define EM_ST9PLUS           67
#define EM_ST7               68
#define EM_68HC16            69
#define EM_68HC11            70
#define EM_68HC08            71
#define EM_68HC05            72
#define EM_SVX               73
#define EM_ST19              74
#define EM_VAX               75
#define EM_CRIS              76
#define EM_JAVELIN           77
#define EM_FIREPATH          78
#define EM_ZSP               79
#define EM_MMIX              80
#define EM_HUANY             81
#define EM_PRISM             82
#define EM_AVR               83
#define EM_FR30              84
#define EM_D10V              85
#define EM_D30V              86
#define EM_V850              87
#define EM_M32R              88
#define EM_MN10300           89
#define EM_MN10200           90
#define EM_PJ                91
#define EM_OPENRISC          92
#define EM_ARC_COMPACT       93
#define EM_XTENSA            94
#define EM_VIDEOCORE         95
#define EM_TMM_GPP           96
#define EM_NS32K             97
#define EM_TPC               98
#define EM_SNP1K             99
#define EM_ST200            100
#define EM_IP2K             101
#define EM_MAX              102
#define EM_CR               103
#define EM_F2MC16           104
#define EM_MSP430           105
#define EM_BLACKFIN         106
#define EM_SE_C33           107
#define EM_SEP              108
#define EM_ARCA             109
#define EM_UNICORE          110
#define EM_EXCESS           111
#define EM_DXP              112
#define EM_ALTERA_NIOS2     113
#define EM_CRX              114
#define EM_XGATE            115
#define EM_C166             116
#define EM_M16C             117
#define EM_DSPIC30F         118
#define EM_CE               119
#define EM_M32C             120
#define EM_TSK3000          131
#define EM_RS08             132
#define EM_SHARC            133
#define EM_ECOG2            134
#define EM_SCORE7           135
#define EM_DSP24            136
#define EM_VIDEOCORE3       137
#define EM_LATTICEMICO32    138
#define EM_SE_C17           139
#define EM_TI_C6000         140
#define EM_TI_C2000         141
#define EM_TI_C5500         142
#define EM_TI_ARP32         143
#define EM_TI_PRU           144
#define EM_MMDSP_PLUS       160
#define EM_MMDSP_PLUS       160
#define EM_CYPRESS_M8C      161
#define EM_R32C             162
#define EM_TRIMEDIA         163
#define EM_QDSP6            164
#define EM_8051             165
#define EM_STXP7X           166
#define EM_NDS32            167
#define EM_ECOG1            168
#define EM_ECOG1X           168
#define EM_MAXQ30           169
#define EM_XIMO16           170
#define EM_MANIK            171
#define EM_CRAYNV2          172
#define EM_RX               173
#define EM_METAG            174
#define EM_MCST_ELBRUS      175
#define EM_ECOG16           176
#define EM_CR16             177
#define EM_ETPU             178
#define EM_SLE9X            179
#define EM_L10M             180
#define EM_K10M             181
#define EM_AARCH64          183
#define EM_AVR32            185
#define EM_STM8             186
#define EM_TILE64           187
#define EM_TILEPRO          188
#define EM_MICROBLAZE       189
#define EM_CUDA             190
#define EM_TILEGX           191
#define EM_CLOUDSHIELD      192
#define EM_COREA_1ST        193
#define EM_COREA_2ND        194
#define EM_ARC_COMPACT2     195
#define EM_OPEN8            196
#define EM_RL78             197
#define EM_VIDEOCORE5       198
#define EM_78KOR            199
#define EM_56800EX          200
#define EM_BA1              201
#define EM_BA2              202
#define EM_XCORE            203
#define EM_MCHP_PIC         204
#define EM_INTEL205         205
#define EM_INTEL206         206
#define EM_INTEL207         207
#define EM_INTEL208         208
#define EM_INTEL209         209
#define EM_KM32             210
#define EM_KMX32            211
#define EM_KMX16            212
#define EM_KMX8             213
#define EM_KVARC            214
#define EM_CDP              215
#define EM_COGE             216
#define EM_COOL             217
#define EM_NORC             218
#define EM_CSR_KALIMBA      219
#define EM_Z80              220
#define EM_VISIUM           221
#define EM_FT32             222
#define EM_MOXIE            223
#define EM_AMDGPU           224
#define EM_RISCV            243

// e_version identifiers
#define EV_NONE     0
#define EV_CURRENT  1

// e_ident indices
#define EI_MAG0         0
#define EI_MAG1         1
#define EI_MAG2         2
#define EI_MAG3         3
#define EI_CLASS        4
#define EI_DATA         5
#define EI_VERSION      6
#define EI_OSABI        7
#define EI_ABIVERSION   8
#define EI_PAD          9
#define EI_NIDENT      16

// e_ident values
#define ELFMAG0          0x7f
#define ELFMAG1            'E'
#define ELFMAG2            'L'
#define ELFMAG3            'F'
#define ELFCLASSNONE        0
#define ELFCLASS32          1
#define ELFCLASS64          2
#define ELFDATANONE         0
#define ELFDATA2LSB         1
#define ELFDATA2MSB         2
#define ELFOSABI_NONE       0
#define ELFOSABI_HPUX       1
#define ELFOSABI_NETBSD     2
#define ELFOSABI_GNU        3
#define ELFOSABI_LINUX      3
#define ELFOSABI_SOLARIS    6
#define ELFOSABI_AIX        7
#define ELFOSABI_IRIX       8
#define ELFOSABI_FREEBSD    9
#define ELFOSABI_TRU64     10
#define ELFOSABI_MODESTO   11
#define ELFOSABI_OPENBSD   12
#define ELFOSABI_OPENVMS   13
#define ELFOSABI_NSK       14
#define ELFOSABI_AROS      15
#define ELFOSABI_FENIXOS   16
#define ELFOSABI_CLOUDABI  17
#define ELFOSABI_OPENVOS   18

// special section header indices
#define SHN_UNDEF       0x0000
#define SHN_LORESERVE   0xff00
#define SHN_LOPROC      0xff00
#define SHN_HIPROC      0xff1f
#define SHN_LOOS        0xff20
#define SHN_HIOS        0xff3f
#define SHN_ABS         0xfff1
#define SHN_COMMON      0xfff2
#define SHN_XINDEX      0xffff
#define SHN_HIRESERVE   0xffff

// sh_type values
#define SHT_NULL                     0
#define SHT_PROGBITS                 1
#define SHT_SYMTAB                   2
#define SHT_STRTAB                   3
#define SHT_RELA                     4
#define SHT_HASH                     5
#define SHT_DYNAMIC                  6
#define SHT_NOTE                     7
#define SHT_NOBITS                   8
#define SHT_REL                      9
#define SHT_SHLIB                   10
#define SHT_DYNSYM                  11
#define SHT_INIT_ARRAY              14
#define SHT_FINI_ARRAY              15
#define SHT_PREINIT_ARRAY           16
#define SHT_GROUP                   17
#define SHT_SYMTAB_SHNDX            18
#define SHT_LOOS            0x60000000
#define SHT_GNU_HASH        0x6ffffff6
#define SHT_HIOS            0x6fffffff
#define SHT_LOPROC          0x70000000
#define SHT_X86_64_UNWIND   0x70000001
#define SHT_HIPROC          0x7fffffff
#define SHT_LOUSER          0x80000000
#define SHT_HIUSER          0xffffffff

// sh_flags values
#define SHF_WRITE                   0x1
#define SHF_ALLOC                   0x2
#define SHF_EXECINSTR               0x4
#define SHF_MERGE                  0x10
#define SHF_STRINGS                0x20
#define SHF_INFO_LINK              0x40
#define SHF_LINK_ORDER             0x80
#define SHF_OS_NONCONFORMING      0x100
#define SHF_GROUP                 0x200
#define SHF_TLS                   0x400
#define SHF_COMPRESSED            0x800
#define SHF_MASKOS           0x0ff00000
#define SHF_MASKPROC         0xf0000000
#define SHF_X86_64_LARGE     0x10000000

// ch_type values
#define ELFCOMPRESS_ZLIB            1
#define ELFCOMPRESS_LOOS   0x60000000
#define ELFCOMPRESS_HIOS   0x6fffffff
#define ELFCOMPRESS_LOPROC 0x70000000
#define ELFCOMPRESS_HIPROC 0x7fffffff

// section group flags
#define GRP_COMDAT          0x1
#define GRP_MASKOS   0x0ff00000
#define GRP_MASKPROC 0xf0000000

// special symbol table indices
#define STN_UNDEF 0

// st_info fields
#define ELF64_ST_BIND(i)   ((i)>>4)
#define ELF64_ST_TYPE(i)   ((i)&0xf)
#define ELF64_ST_INFO(b,t) (((b)<<4)+((t)&0xf))

// st_other fields
#define ELF64_ST_VISIBILITY(o) ((o)&0x3)

// st_info binding
#define STB_LOCAL    0
#define STB_GLOBAL   1
#define STB_WEAK     2
#define STB_LOOS    10
#define STB_HIOS    12
#define STB_LOPROC  13
#define STB_HIPROC  15

// st_info type
#define STT_NOTYPE   0
#define STT_OBJECT   1
#define STT_FUNC     2
#define STT_SECTION  3
#define STT_FILE     4
#define STT_COMMON   5
#define STT_TLS      6
#define STT_LOOS    10
#define STT_HIOS    12
#define STT_LOPROC  13
#define STT_HIPROC  15

// st_info visibility
#define STV_DEFAULT   0
#define STV_INTERNAL  1
#define STV_HIDDEN    2
#define STV_PROTECTED 3

// r_info field extraction
#define ELF64_R_SYM(i)    ((i)>>32)
#define ELF64_R_TYPE(i)   ((i)&0xffffffffL)
#define ELF64_R_INFO(s,t) (((s)<<32)+((t)&0xffffffffL))

// x86-64 relocation types
#define R_X86_64_NONE             0 // none none
#define R_X86_64_64               1 // word64 S + A
#define R_X86_64_PC32             2 // word32 S + A - P
#define R_X86_64_GOT32            3 // word32 G + A
#define R_X86_64_PLT32            4 // word32 L + A - P
#define R_X86_64_COPY             5 // none none
#define R_X86_64_GLOB_DAT         6 // word64 S
#define R_X86_64_JUMP_SLOT        7 // word64 S
#define R_X86_64_RELATIVE         8 // word64 B + A
#define R_X86_64_GOTPCREL         9 // word32 G + GOT + A - P
#define R_X86_64_32              10 // word32 S + A
#define R_X86_64_32S             11 // word32 S + A
#define R_X86_64_16              12 // word16 S + A
#define R_X86_64_PC16            13 // word16 S + A - P
#define R_X86_64_8               14 // word8 S + A
#define R_X86_64_PC8             15 // word8 S + A - P
#define R_X86_64_DTPMOD64        16 // word64
#define R_X86_64_DTPOFF64        17 // word64
#define R_X86_64_TPOFF64         18 // word64
#define R_X86_64_TLSGD           19 // word32
#define R_X86_64_TLSLD           20 // word32
#define R_X86_64_DTPOFF32        21 // word32
#define R_X86_64_GOTTPOFF        22 // word32
#define R_X86_64_TPOFF32         23 // word32
#define R_X86_64_PC64            24 // word64 S + A - P
#define R_X86_64_GOTOFF64        25 // word64 S + A - GOT
#define R_X86_64_GOTPC32         26 // word32 GOT + A - P
#define R_X86_64_GOT64           27 // word64 G + A
#define R_X86_64_GOTPCREL64      28 // word64 G + GOT - P + A
#define R_X86_64_GOTPC64         29 // word64 GOT - P + A
#define R_X86_64_GOTPLT64        30 // word64 G + A
#define R_X86_64_PLTOFF64        31 // word64 L - GOT + A
#define R_X86_64_SIZE32          32 // word32 Z + A
#define R_X86_64_SIZE64          33 // word64 Z + A
#define R_X86_64_GOTPC32_TLSDESC 34 // word32
#define R_X86_64_TLSDESC_CALL    35 // none
#define R_X86_64_TLSDESC         36 // word64×2
#define R_X86_64_IRELATIVE       37 // word64 indirect (B + A)

// p_type values
#define PT_NULL             0
#define PT_LOAD             1
#define PT_DYNAMIC          2
#define PT_INTERP           3
#define PT_NOTE             4
#define PT_SHLIB            5
#define PT_PHDR             6
#define PT_TLS              7
#define PT_LOOS    0x60000000
#define PT_HIOS    0x6fffffff
#define PT_LOPROC  0x70000000
#define PT_HIPROC  0x7fffffff

// p_flags values
#define PF_X        0x00000001
#define PF_W        0x00000002
#define PF_R        0x00000004
#define PF_MASKOS   0x0ff00000
#define PF_MASKPROC 0xf0000000

// d_tag values
#define DT_NULL                      0
#define DT_NEEDED                    1
#define DT_PLTRELSZ                  2
#define DT_PLTGOT                    3
#define DT_HASH                      4
#define DT_STRTAB                    5
#define DT_SYMTAB                    6
#define DT_RELA                      7
#define DT_RELASZ                    8
#define DT_RELAENT                   9
#define DT_STRSZ                    10
#define DT_SYMENT                   11
#define DT_INIT                     12
#define DT_FINI                     13
#define DT_SONAME                   14
#define DT_RPATH                    15
#define DT_SYMBOLIC                 16
#define DT_REL                      17
#define DT_RELSZ                    18
#define DT_RELENT                   19
#define DT_PLTREL                   20
#define DT_DEBUG                    21
#define DT_TEXTREL                  22
#define DT_JMPREL                   23
#define DT_BIND_NOW                 24
#define DT_INIT_ARRAY               25
#define DT_FINI_ARRAY               26
#define DT_INIT_ARRAYSZ             27
#define DT_FINI_ARRAYSZ             28
#define DT_RUNPATH                  29
#define DT_FLAGS                    30
#define DT_ENCODING                 32
#define DT_PREINIT_ARRAY            32
#define DT_PREINIT_ARRAYSZ          33
#define DT_SYMTAB_SHNDX             34
#define DT_LOOS             0x6000000D
#define DT_HIOS             0x6ffff000
#define DT_LOPROC           0x70000000
#define DT_HIPROC           0x7fffffff

// DT_FLAGS values
#define DF_ORIGIN     0x01
#define DF_SYMBOLIC   0x02
#define DF_TEXTREL    0x04
#define DF_BIND_NOW   0x08
#define DF_STATIC_TLS 0x10

// aux entries
#define AT_NULL               0
#define AT_IGNORE             1
#define AT_EXECFD             2
#define AT_PHDR               3
#define AT_PHENT              4
#define AT_PHNUM              5
#define AT_PAGESZ             6
#define AT_BASE               7
#define AT_FLAGS              8
#define AT_ENTRY              9
#define AT_NOTELF            10
#define AT_UID               11
#define AT_EUID              12
#define AT_GID               13
#define AT_EGID              14
#define AT_CLKTCK            17
#define AT_PLATFORM          15
#define AT_HWCAP             16
#define AT_FPUCW             18
#define AT_DCACHEBSIZE       19
#define AT_ICACHEBSIZE       20
#define AT_UCACHEBSIZE       21
#define AT_IGNOREPPC         22
#define AT_SECURE            23
#define AT_BASE_PLATFORM     24
#define AT_RANDOM            25
#define AT_HWCAP2            26
#define AT_EXECFN            31
#define AT_SYSINFO           32
#define AT_SYSINFO_EHDR      33
#define AT_L1I_CACHESHAPE    34
#define AT_L1D_CACHESHAPE    35
#define AT_L2_CACHESHAPE     36
#define AT_L3_CACHESHAPE     37
#define AT_L1I_CACHESIZE     40
#define AT_L1I_CACHEGEOMETRY 41
#define AT_L1D_CACHESIZE     42
#define AT_L1D_CACHEGEOMETRY 43
#define AT_L2_CACHESIZE      44
#define AT_L2_CACHEGEOMETRY  45
#define AT_L3_CACHESIZE      46
#define AT_L3_CACHEGEOMETRY  47
#define AT_MINSIGSTKSZ       51

struct Elf64_Ehdr typedef Elf64_Ehdr;
struct Elf64_Ehdr {
    unsigned char   e_ident[EI_NIDENT];
    Elf64_Half      e_type;
    Elf64_Half      e_machine;
    Elf64_Word      e_version;
    Elf64_Addr      e_entry;
    Elf64_Off       e_phoff;
    Elf64_Off       e_shoff;
    Elf64_Word      e_flags;
    Elf64_Half      e_ehsize;
    Elf64_Half      e_phentsize;
    Elf64_Half      e_phnum;
    Elf64_Half      e_shentsize;
    Elf64_Half      e_shnum;
    Elf64_Half      e_shstrndx;
};

struct Elf64_Shdr typedef Elf64_Shdr;
struct Elf64_Shdr {
    Elf64_Word  sh_name;
    Elf64_Word  sh_type;
    Elf64_Xword sh_flags;
    Elf64_Addr  sh_addr;
    Elf64_Off   sh_offset;
    Elf64_Xword sh_size;
    Elf64_Word  sh_link;
    Elf64_Word  sh_info;
    Elf64_Xword sh_addralign;
    Elf64_Xword sh_entsize;
};

struct Elf64_Chdr typedef Elf64_Chdr;
struct Elf64_Chdr {
    Elf64_Word  ch_type;
    Elf64_Word  ch_reserved;
    Elf64_Xword ch_size;
    Elf64_Xword ch_addralign;
};

struct Elf64_Sym typedef Elf64_Sym;
struct Elf64_Sym {
    Elf64_Word    st_name;
    unsigned char st_info;
    unsigned char st_other;
    Elf64_Half    st_shndx;
    Elf64_Addr    st_value;
    Elf64_Xword   st_size;
};

struct Elf64_Rel typedef Elf64_Rel;
struct Elf64_Rel {
    Elf64_Addr   r_offset;
    Elf64_Xword  r_info;
};

struct Elf64_Rela typedef Elf64_Rela;
struct Elf64_Rela {
    Elf64_Addr   r_offset;
    Elf64_Xword  r_info;
    Elf64_Sxword r_addend;
};

struct Elf64_Phdr typedef Elf64_Phdr;
struct Elf64_Phdr {
    Elf64_Word  p_type;
    Elf64_Word  p_flags;
    Elf64_Off   p_offset;
    Elf64_Addr  p_vaddr;
    Elf64_Addr  p_paddr;
    Elf64_Xword p_filesz;
    Elf64_Xword p_memsz;
    Elf64_Xword p_align;
};

struct Elf64_Dyn typedef Elf64_Dyn;
struct Elf64_Dyn {
    Elf64_Sxword d_tag;
    union {
        Elf64_Xword d_val;
        Elf64_Addr  d_ptr;
    } d_un;
};

extern Elf64_Dyn _DYNAMIC[];

struct Elf64_Aux typedef Elf64_Aux;
struct Elf64_Aux {
    int a_type;
    union {
        long a_val;
        void *a_ptr;
        void (*a_fcn)();
    };
};


