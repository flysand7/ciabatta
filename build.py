#!/usr/bin/env python3

import importlib
import platform # Checking current OS
import os
import sys
import re


class colors:
    grey='\033[38;5;243m'
    cyan='\033[38;5;80m'
    red='\033[38;5;196m'
    purple='\033[38;5;141m'
    yellow='\033[38;5;220m'
    green='\033[32m'
    reset='\033[0m'

def print_step(step):
    print(f'{colors.green}==>{colors.reset} {step}', end='')

def path(str):
    l = str.split('/')
    return os.path.join(*l)

def quote(s):
    return '"' + s + '"'

def prefix(prefix):
    return (lambda s: prefix + s)

def run(cmdline):
    cmdline_colored = re.sub(r'"(.+?)"', f'{colors.grey}"\\1"{colors.reset}', cmdline)
    cmdline_colored = re.sub(r'^([a-zA-Z0-9-_]*)', f'{colors.yellow}\\1{colors.reset}', cmdline_colored)
    print('  $', cmdline_colored)
    code = os.system(cmdline)
    if code != 0:
        sys.exit(code)

cc_flags = []
cc_defines = []
cc_includes = []

def compile(output, inputs, additional_flags=[]):
    flags_cmdline = ' '.join(cc_flags + additional_flags)
    defines_cmdline = ' '.join(map(prefix('-D '), map(quote, cc_defines)))
    includes_cmdline = ' '.join(map(prefix('-I '), map(quote, cc_includes)))
    inputs_cmdline = ' '.join(inputs)
    cmdline = f'clang {flags_cmdline} {includes_cmdline} {defines_cmdline} {inputs_cmdline} -o {output}'
    run(cmdline)

def compile_obj(output, inputs, additional_flags=[]):
    compile(output, inputs, ['-c -fpic'] + additional_flags)

def compile_exe(output, inputs, additional_flags=[]):
    compile(output, inputs, ['-pie'] + additional_flags)

def compile_shared(output, inputs, additional_flags=[]):
    compile(output, inputs, ['-shared'] + additional_flags)

def assemble_obj(output, inputs, flags=[]):
    flags_cmdline = ' '.join(flags)
    inputs_cmdline = ' '.join(inputs)
    cmdline = f'nasm {flags_cmdline} {inputs_cmdline} -o {output}'
    run(cmdline)

def archive(output, inputs):
    inputs_cmdline = ' '.join(inputs)
    cmdline = f'llvm-ar -rcs {output} {inputs_cmdline}'
    run(cmdline)

# Ciabatta build spec
if not os.path.exists('lib'):
    os.mkdir('lib')
if not os.path.exists('bin'):
    os.mkdir('bin')

target_abi = 'sysv'
target_arch = 'x86-64'
target_os = 'linux'

cc_flags.extend(['-nostdlib', '-fno-stack-protector'])
cc_flags.extend(['-g', '-O0'])
cc_defines.append('_DEBUG')
cc_defines.append(f'_CIA_OS_CONF=\\"os/{target_os}/conf.h\\"')

cc_includes.append('include')
cc_includes.append('include/linux')

# Build the dynamic loader
loader_flags = [
    '-Wl,-e,_dlstart',
    '-Wl,--sort-section,alignment',
    '-Wl,--sort-common',
    '-Wl,--gc-sections',
    '-Wl,--hash-style=both',
    '-Wl,--no-undefined',
    '-Wl,--exclude-libs=ALL'
]
print_step("Building lib/ld-cia.so\n")
assemble_obj('bin/loader-entry.o', [f'arch/{target_abi}_{target_arch}/loader-entry.asm'], ['-f "elf64"'])
compile_shared('lib/ld-cia.so', ['bin/loader-entry.o','loader/loader-self-reloc.c','loader/loader.c'], loader_flags)

# Build the ciabatta
print_step("Building lib/cia.a\n")
assemble_obj('bin/thread-entry.o', [f'arch/{target_abi}_{target_arch}/thread-entry.asm'], ['-f "elf64"'])
compile_obj('bin/cia.o', ['cia.c'])
archive('lib/cia.a', ['bin/cia.o', 'bin/thread-entry.o'])

# Build the test
compile_exe('a', ['tests/threaded.c', 'lib/cia.a'], ['-Wl,-dynamic-linker,lib/ld-cia.so'])
