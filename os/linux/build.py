
loader_flags = [
    '-Wl,-e,_dlstart',
    '-Wl,--sort-section,alignment',
    '-Wl,--sort-common',
    '-Wl,--gc-sections',
    '-Wl,--hash-style=both',
    '-Wl,--no-undefined',
    '-Wl,--exclude-libs=ALL'
]

def build(toolchain):
    toolchain.compiler.flags.append('-fno-stack-protector')
    toolchain.assembler.flags.append('-f "elf64"')
    compiler = toolchain.compiler
    assembler = toolchain.assembler
    archiver = toolchain.archiver
    # Build dynamic loader
    toolchain.step("lib/ld-cia.so")
    assembler.obj('bin/loader-entry.o', ['loader/loader-entry.asm'])
    compiler.shared('lib/ld-cia.so', ['bin/loader-entry.c','loader/loader-self-reloc.c','loader/loader.c'], loader_flags)
    # Build the crt
    toolchain.step('crt.lib')
    compiler.obj('')