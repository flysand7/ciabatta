
import os
import sys
import subprocess
import runpy
from pathlib import Path

for arg in sys.argv[1:]:
    if arg == 'test':
        test = os.getenv('test');
        if test == None:
            test = 'assert'
        test_file = 'test/test_' + test + '.c'
        subprocess.run([
            'clang',
            test_file,
            '-Iinc',
            '-g',
            '-luser32',
            '-lkernel32',
            '-lshell32',
            '-lDbghelp',
            '-lciabatta.lib',
        ])
        sys.exit(0)

# Build dependencies rq
if not Path("ryu/ryu.lib").exists():
    os.chdir('ryu')
    runpy.run_path(path_name='bake.py')
    os.chdir('..')

if not Path('unicope/unicope.lib').exists():
    os.chdir('unicope')
    subprocess.run(['bake.cmd'])
    os.chdir('..')


# General compile options

platform = 'win'
do_cuik = False

inc_folders = [
    'inc',
    'unicope/inc',
    'ryu',
]

definitions = [
    '_CRT_SECURE_NO_WARNINGS',
]

# Compiler-specific options

clang_dbg_flags    = ['-g', '-gcodeview']
clang_common_flags = ['-c', '-nodefaultlibs', '-mfma']

cuik_flags         = []

#----------------------------------------------------------------------------#
# Map lists to lists of options
inc_flags = []
def_flags = []
def compile(root, cmap):
    global inc_flags
    global def_flags
    inc_flags = list(map(lambda p: '-I '+ p, inc_folders))
    def_flags = list(map(lambda d: '-D' + d, definitions))
    for path, subdirs, files in os.walk(root):
        for file_name in files:
            file_path = os.path.join(path, file_name)
            short_name, ext = os.path.splitext(file_path)
            if ext in cmap.keys():
                func = cmap[ext]
                func(file_path)

def get_bin_path(file_path):
    rel_path = os.path.normpath(file_path).split(os.path.sep)[1:]
    name, ext = os.path.splitext(os.path.sep.join(rel_path))
    bin_path = os.path.join('bin', name+'.obj')
    os.makedirs(os.path.dirname(bin_path), exist_ok=True)
    return bin_path

def clang_compile(file_name):
    bin_path = get_bin_path(file_name)
    dbg_flags = clang_dbg_flags
    cmn_flags = clang_common_flags
    flags     = dbg_flags + cmn_flags + inc_flags + def_flags
    command   = ' '.join(["clang", file_name, '-o', bin_path] + flags)
    subprocess.run(command.split(' '))
    print(file_name, '=>', bin_path)

def nasm_compile(file_name):
    bin_path = get_bin_path(file_name)
    subprocess.run(['nasm', file_name, '-f', 'win64', '-o', bin_path])
    print(file_name, '=>', bin_path)

#-----------------------------------------------------------------------------#

# Compile the platform-independent part
obj_paths = ['ryu/ryu.lib', 'unicope/unicope.lib']
compile_map = {}
compile_map['.asm'] = nasm_compile

if not do_cuik:
    compile_map['.c']   = clang_compile
    # Add the platform folder to includes and compile platform-dependent part
    inc_folders.append(os.path.join('src', platform))
    compile(os.path.normpath(os.path.join('src', platform)), compile_map)

else:
    src    = 'src/code/*.c'
    os_src = 'src/' + platform
    cuik_flags += '-c -o ciabatta.obj'.split(' ') + [src, os_src]
    subprocess.run(['cuik'] + cuik_flags)
    obj_paths.append('ciabatta.obj')

compile(os.path.normpath('src/code'), compile_map)

# Make an archive of all object files
for dir, _, f in os.walk('bin'):
    if len(f) != 0:
        obj_paths.append(os.path.join(dir, '*.obj'))
subprocess.run(['lib', '/out:ciabatta.lib'] + obj_paths)
print('*.obj => ciabatta.lib')