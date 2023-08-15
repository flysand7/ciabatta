#!/usr/bin/env python3

import argparse # Parsing command-line args
import platform # Checking current OS
import shutil
import os
import sys
import pyjson5
import re

# Colors
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

# Parse command line arguments
arg_parser = argparse.ArgumentParser('build.py')
arg_parser.add_argument('--clean', action='store_true', help='Remove all object files and binaries')
arg_parser.add_argument('--test', help='Compile ciabatta executable with given file')
arg_parser.add_argument('--mode', '-m', choices=['debug', 'release'], default='debug', help='Select build configuration')
arg_parser.add_argument('--target', '-t', help='Select target OS (default=host)')
arg_parser.add_argument('--compiler', '-c', choices=['clang', 'cuik'], default='clang', help='Select compiler')
arg_parser.add_argument('--cflags', nargs='*', default=[], help='Pass additional compiler flags')
args = arg_parser.parse_args()
compiler = args.compiler


# Perform cleaning if required
def rm(path):
    if os.path.exists(path):
        os.remove(path)
if args.clean:
    if os.path.exists('bin'):
        shutil.rmtree('bin')
    if os.path.exists('lib'):
        shutil.rmtree('lib')
    rm(os.path.join('src', 'ciabatta.c'))
    rm('a')
    rm('a.exe')
    rm('a.ilk')
    rm('a.pdb')
    sys.exit(0)

# Check host OS
print_step('Performing basic checks\n')
target = args.target
if target is None:
    target = platform.system().lower()
target_colored = f'{colors.cyan}{target}{colors.reset}'
print(f"  * Compiling for host OS: '{target_colored}'")
if not os.path.exists(os.path.join('src', target)):
    print(colors.red, f"  ERROR: OS '{colors.cyan}{target}{colors.red}' isn't implemented.", colors.reset)
    sys.exit(1)

# Add compiler to dependencies
dependencies = [
    'nasm',
    'llvm-ar'
]
dependencies.append(args.compiler)


# Figure out the flags
includes = ['src/include']
cc = args.compiler
cc_defines = []
cc_flags = ['-nostdlib', '-fno-stack-protector']
crt_file = 'crt.lib'
lib_file = 'cia.lib'
dl_file = 'ld-cia.so'
if args.mode == 'release':
    cc_flags.append('-O2')
    cc_defines.append('NDEBUG')
else: # 'debug'
    cc_flags.append('-g')
    cc_flags.append('-O0')
    cc_defines.append('_DEBUG')
if target != 'windows':
    cc_flags.append('-fpic')

if target == 'linux':
    includes.append('src/include/linux')

# Check dependencies
print_step('Checking dependencies... ')
for dependency in dependencies:
    if shutil.which(dependency) is None:
        print(colors.red, f"\n  -> [ERROR] Missing dependency: '{dependency}'", colors.reset)
        sys.exit(1)
print(colors.green, 'OK', colors.reset)

# Generate TinyRT headers for the target platform
print_step(f'Generating TinyRT header for: {target_colored}... ')
tinyrt_config_path = os.path.join('src', target, 'tinyrt.json')
tinyrt_apis = []
try:
    with open(tinyrt_config_path, 'r') as tinyrt_config_file:
        string = tinyrt_config_file.read()
        json = '{' + string + '}'
        tinyrt_config = pyjson5.loads(json)
except Exception as error:
    print(colors.red, f"\n  -> [ERROR] reading file '{tinyrt_config_path}'")
    print(f"  *  {error}", colors.reset)
    sys.exit(1)
tinyrt_header_path = os.path.join('src', target, 'tinyrt-iface.h')
try:
    with open(tinyrt_header_path, 'w') as tinyrt_header_file:
        tinyrt_header_file.write('\n')
        tinyrt_header_file.write(f'// This file is AUTO-GENERATED from {tinyrt_config_path}\n')
        tinyrt_header_file.write('\n')
        for tinyrt_api in tinyrt_config:
            api_name = tinyrt_api
            is_defined = tinyrt_config[tinyrt_api]
            if is_defined:
                tinyrt_apis.append(api_name)
            is_defined_int = 1 if is_defined else 0
            tinyrt_header_file.write(f'#define _{api_name.upper()} {is_defined_int}\n')
except Exception as error:
    print(colors.red, f"  -> [ERROR] writing to file '{tinyrt_header_path}'")
    print(f"  *  {error}", colors.reset)
    sys.exit(1)
print(colors.green, 'OK', colors.reset)

# Generate ciabatta header for the target platform and configuration
print_step('Generating ciabatta.c\n')
library_config_path = os.path.join('src', 'library.json')
try:
    with open(library_config_path, 'r') as library_config_file:
        string = library_config_file.read()
        json = '{' + string + '}'
        library_config = pyjson5.loads(json)
except Exception as error:
    print(colors.red, f"  ERROR when reading file '{library_config_path}':")
    print(f"  {error}", colors.reset)
    sys.exit(1)
ciabatta_header_path = os.path.join('src', 'ciabatta.c')
try:
    with open(ciabatta_header_path, 'w') as ciabatta_header:
        # Write heading
        ciabatta_header.write('\n')
        ciabatta_header.write('// This file is AUTO-GENERATED by library.json\n')
        ciabatta_header.write('\n')
        # Write main include
        ciabatta_header.write('#include <cia-def.h>\n')
        ciabatta_header.write('\n')
        # Write platform includes
        ciabatta_header.write('// Platform-dependent sources\n')
        platform_config = None
        for platform in library_config['platforms']:
            if platform['name'] == target:
                platform_config = platform
        if platform_config is None:
            print(colors.red, f"  ERROR: library config doesn't contain configuration for platform {target}", colors.reset, sep='')
        for include in platform_config['includes']:
            include_path = platform_config['path']
            ciabatta_header.write(f'#include <{include}>\n')
        for source in platform_config['sources']:
            source_path = platform_config['path']
            ciabatta_header.write(f'#include "{source_path}/{source}"\n')
        ciabatta_header.write(f'#include "{target}/tinyrt-iface.h"\n')
        ciabatta_header.write(f'#include <tinyrt.h>\n')
        for tinyrt_source in platform_config['tinyrt']:
            ciabatta_header.write(f'#include "{target}/{tinyrt_source}"\n')
        ciabatta_header.write('\n')
        # Write module includes
        ciabatta_header.write('// Forward declarations')
        for include in library_config['includes']:
            ciabatta_header.write(f'#include <{include}>\n')
        ciabatta_header.write('\n')
        # Write module sources
        mod_exports = []
        requirements_satisfied = True
        for api in library_config['apis']:
            api_name = api['name']
            api_path = api['path']
            if api_name in library_config['export']:
                reqs_satisfied = True
                # Check API dependencies
                for req in api['reqs']:
                    if req in tinyrt_apis:
                        continue
                    elif req in mod_exports:
                        continue
                    reqs_satisfied = False
                    break
                if not reqs_satisfied:
                    print(colors.red, f"  * Not exporting API '{api_name}'", colors.reset, sep='')
                    requirements_satisfied = False
                else:
                    print(colors.green, f"  * Exporting API '{api_name}'", colors.reset, sep='')
                    ciabatta_header.write(f'// Module {api_name}\n')
                    mod_exports.append(api_name)
                    for include in api['includes']:
                        ciabatta_header.write(f'#include "{api_path}/{include}"\n')
        if not requirements_satisfied:
            sys.exit(1)
except Exception as error:
    print(colors.red, f"  ERROR writing file '{ciabatta_header_path}':", sep='')
    print(f"  {error}", colors.reset)
    sys.exit(1)
print_step('Generating cia-conf.h\n')

def path(str):
    l = str.split('/')
    return os.path.join(*l)
def quote(s):
    return '"' + s + '"'
def prefix(prefix):
    return (lambda s: prefix + s)

cc_flags_str = ' '.join(
               cc_flags +
               list(map(prefix('-D '), cc_defines)) +
               list(map(prefix('-I '), map(quote, includes))))

try:
    with open(os.path.join('src', 'include', 'cia-conf.h'), 'w') as conf_file:
        os_config = open(os.path.join('src', target, 'conf.h'))
        conf_file.write('\n')
        conf_file.write('// This file is AUTO-GENERATED\n')
        conf_file.write('// See os folder (e.g. src/linux) for conf.h file\n')
        conf_file.write(os_config.read())
        os_config.close()
except Exception as error:
    print(colors.red, f"  ERROR writing file '{ciabatta_header_path}':", sep='')
    print(f"  {error}", colors.reset)
    sys.exit(1)

def run(cmdline):
    cmdline_colored = re.sub(r'"(.+?)"', f'{colors.grey}"\\1"{colors.reset}', cmdline)
    cmdline_colored = re.sub(r'^([a-zA-Z0-9-_]*)', f'{colors.yellow}\\1{colors.reset}', cmdline_colored)
    print('  $', cmdline_colored)
    code = os.system(cmdline)
    if code != 0:
        sys.exit(code)

def assemble(src, out):
    format = 'elf64'
    if target == 'windows':
        format = 'win64'
    cmdline = f'nasm -f "{format}" "{path(src)}" -o "{path(out)}"'
    run(cmdline)

def compile(srcs, out, extra_flags = ''):
    if cc == 'cuik' and out.endswith('.o'):
        out = out[:-2]
    flags = cc_flags_str + ' ' + extra_flags + ' ' + ' '.join(args.cflags)
    inputs = ' '.join(map(quote, map(path, srcs)))
    cmdline = f'{cc} {flags} {inputs} -o {quote(path(out))}'
    run(cmdline)

def archive(srcs, out):
    inputs = ' '.join(map(quote, srcs))
    cmdline = f'llvm-ar -rcs {quote(out)} {inputs}'
    run(cmdline)

# Ciabatta build spec
if not os.path.exists('lib'):
    os.mkdir('lib')
if not os.path.exists('bin'):
    os.mkdir('bin')

cia_lib = f'lib/{lib_file}'
crt_lib = f'lib/{crt_file}'
dl_lib = f'lib/{dl_file}'

dl_flags = [
    '-shared',
    '-nostdlib',
    # '-mno-sse',
    '-ffreestanding',
    '-fno-stack-protector',
    '-Wl,-e,_dlstart',
    '-Wl,--sort-section,alignment',
    '-Wl,--sort-common',
    '-Wl,--gc-sections',
    '-Wl,--hash-style=both',
    '-Wl,--no-undefined',
    '-Wl,--exclude-libs=ALL',
]

if target == 'linux':
    print_step(f'Compiling {dl_lib}\n')
    assemble('src/loader/loader-entry.asm', 'bin/loader-entry.o')
    compile(['bin/loader-entry.o', 'src/loader/loader-self-reloc.c', 'src/loader/loader.c'], dl_lib,
        ' '.join(dl_flags))

print_step(f'Compiling {crt_file}\n')
if target == 'linux':
    compile(['src/linux/crt-ctors.c'], 'bin/crt-ctors.o', '-c')
    archive(['bin/crt-ctors.o'], crt_lib)
elif target == 'windows':
    assemble('src/windows/chkstk.asm', 'bin/chkstk.o')
    compile(['src/windows/crt-entry.c'], 'bin/crt-entry.o', '-c')
    archive(['bin/crt-entry.o', 'bin/chkstk.o'], crt_lib)

print_step(f'Compiling {lib_file}\n')
compile(['src/ciabatta.c'], 'bin/ciabatta.o', '-c')
archive(['bin/ciabatta.o'], cia_lib)

if args.test:
    if target == 'linux':
        compile([args.test, crt_lib, cia_lib], 'a', f'-pie -Wl,-dynamic-linker,{dl_lib} -fno-stack-protector')
    elif target == 'windows':
        compile([args.test, crt_lib, cia_lib], 'a.exe', '-lkernel32.lib')
