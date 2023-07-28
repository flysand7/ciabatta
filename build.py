#!/usr/bin/env python3

import argparse # Parsing command-line args
import platform # Checking current OS
import shutil
import os
import sys
import pyjson5


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
print('==> Performing basic checks')
target = args.target
if target is None:
    host_os = platform.system().lower()
    print(f"  -> Compiling for host OS: '{host_os}'")
    if not os.path.exists(os.path.join('src', host_os)):
        print(f"  -> [ERROR] OS '{host_os}' isn't implemented.")
        sys.exit(1)
    target = host_os

# Add compiler to dependencies
dependencies = [
    'nasm',
    'llvm-ar'
]
dependencies.append(args.compiler)


# Figure out the flags
includes = ['include']
cc = args.compiler
cc_defines = []
cc_flags = ['-nostdlib']
crt_file = 'crt.lib'
lib_file = 'cia.lib'
if args.mode == 'release':
    cc_flags.append('-O2')
    cc_defines.append('NDEBUG')
else: # 'debug'
    cc_flags.append('-g')
    cc_flags.append('-O0')
    cc_defines.append('DEBUG')
if target != 'windows':
    cc_flags.append('-fpic')
cc_defines.append(f'_CIA_OS_{target.upper()}')

# Check dependencies
print('==> Checking dependencies')
for dependency in dependencies:
    if shutil.which(dependency) is None:
        print(f"  -> [ERROR] Missing dependency: '{dependency}'")
        sys.exit(1)
print('  -> Everything OK')

# Generate TinyRT headers for the target platform
print(f"==> Generating TinyRT header for {target}")
tinyrt_config_path = os.path.join('src', target, 'tinyrt.json')
tinyrt_apis = []
try:
    print(f"  -> Reading file '{tinyrt_config_path}'")
    with open(tinyrt_config_path, 'r') as tinyrt_config_file:
        string = tinyrt_config_file.read()
        json = '{' + string + '}'
        tinyrt_config = pyjson5.loads(json)
except Exception as error:
    print(f"  -> [ERROR] reading file '{tinyrt_config_path}'")
    print(f"  *  {error}")
    sys.exit(1)
tinyrt_header_path = os.path.join('src', target, 'tinyrt-iface.h')
try:
    print(f"  -> Writing to file '{tinyrt_header_path}'")
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
    print(f"  -> [ERROR] writing to file '{tinyrt_header_path}'")
    print(f"  *  {error}")
    sys.exit(1)
print('  -> TinyRT header generated')

# Generate ciabatta header for the target platform and configuration
print(f"==> Generating ciabatta.c")
library_config_path = os.path.join('src', 'library.json')
try:
    print(f"  -> Reading file '{library_config_path}'")
    with open(library_config_path, 'r') as library_config_file:
        string = library_config_file.read()
        json = '{' + string + '}'
        library_config = pyjson5.loads(json)
except Exception as error:
    print(f"  -> [ERROR] reading file '{library_config_path}'")
    print(f"  *  {error}")
    sys.exit(1)
ciabatta_header_path = os.path.join('src', 'ciabatta.c')
try:
    print(f"  -> Writing to file '{ciabatta_header_path}'")
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
            print(f"  -> [ERROR] library config doesn't contain configuration for platform {target}")
        for include in platform_config['includes']:
            include_path = platform_config['path']
            ciabatta_header.write(f'#include "{include_path}/{include}"\n')
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
        for api in library_config['apis']:
            api_name = api['name']
            api_path = api['path']
            reqs_satisfied = True
            # Check API dependencies
            for req in api['reqs']:
                if not (req in tinyrt_apis):
                    reqs_satisfied = False
                    break
                if not (req in mod_exports):
                    reqs_satisfied = False
                    break
            if not reqs_satisfied:
                print(f"  -> Not exporting API '{api_name}'")
            else:
                print(f"  * Exporting API '{api_name}'")
                ciabatta_header.write(f'// Module {api_name}\n')
                mod_exports.append(api_name)
                for include in api['includes']:
                    ciabatta_header.write(f'#include "{api_path}/{include}"\n')
except Exception as error:
    print(f"  -> [ERROR] writing file '{ciabatta_header_path}'")
    print(f"  *  {error}")
    sys.exit(1)

def quote(s):
    return '"' + s + '"'
def prefix(prefix):
    return (lambda s: prefix + s)
def prefix_quote(prefix):
    return (lambda s: prefix + '"' + s + '"')

cc_flags_str = ' '.join(
               cc_flags +
               list(map(prefix('-D '), cc_defines)) +
               list(map(prefix_quote('-I '), includes)))

print(f"==> Compiling {lib_file}")
print('  * Compiler flags:', cc_flags_str)

def assemble(src, out):
    format = 'elf64'
    if target == 'windows':
        format = 'win64'
    cmdline = f'nasm -f "{format}" "{src}" -o "{out}"'
    print('  >', cmdline)
    code = os.system(cmdline)
    if code != 0:
        sys.exit(code)

def compile(srcs, out, extra_flags = ''):
    if cc == 'cuik' and out.endswith('.o'):
        out = out[:-2]
    flags = cc_flags_str + ' ' + extra_flags + ' '.join(args.cflags)
    inputs = ' '.join(map(quote, srcs))
    cmdline = f'{cc} {flags} {inputs} -o {quote(out)}'
    print('  >', cmdline)
    code = os.system(cmdline)
    if code != 0:
        sys.exit(code)

def archive(srcs, out):
    inputs = ' '.join(map(quote, srcs))
    cmdline = f'llvm-ar -rcs {quote(out)} {inputs}'
    print('  >', cmdline)
    code = os.system(cmdline)
    if code != 0:
        sys.exit(code)

# Ciabatta build spec
if not os.path.exists('lib'):
    os.mkdir('lib')
if not os.path.exists('bin'):
    os.mkdir('bin')

def p(path):
    l = path.split('/')
    return os.path.join(*l)

cia_lib = p(f'lib/{lib_file}')
crt_lib = p(f'lib/{crt_file}')
ciabatta_c = p('src/ciabatta.c')
ciabatta_o = p('bin/ciabatta.o')

if target == 'linux':
    assemble(p('src/linux/crt-entry.asm'), p('bin/crt-entry.o'))
    compile([p('src/linux/crt-ctors.c')], p('bin/crt-ctors.o'), '-c')
    archive([p('bin/crt-ctors.o'), p('bin/crt-entry.o')], p('lib', crt_file))
elif target == 'windows':
    assemble(p('src/windows/chkstk.asm'), p('bin/chkstk.o'))
    compile([p('src/windows/crt-entry.c')], p('bin/crt-entry.o'), '-c')
    archive([p('bin/crt-entry.o'), p('bin/chkstk.o')], crt_lib)
compile([ciabatta_c], ciabatta_o, '-c')
archive([ciabatta_o], cia_lib)

if args.test:
    if target == 'linux':
        compile([args.test, cia_lib, crt_lib], 'a', '-pie')
    elif target == 'windows':
        compile([args.test, cia_lib, crt_lib], 'a', '-lkernel32.lib')
