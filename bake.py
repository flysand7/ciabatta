
import os
import subprocess

# General compile options

platform = 'win'
do_cuik = False

inc_folders = [
    'inc',
    os.path.join('unicope', 'inc'),
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

if not do_cuik:
    # Compile the platform-independent part
    compile_map = {}
    compile_map['.c']   = clang_compile
    compile_map['.asm'] = nasm_compile
    compile(os.path.normpath('src/code'), compile_map)

    # Add the platform folder to includes and compile platform-dependent part
    inc_folders.append(os.path.join('src', platform))
    compile(os.path.normpath(os.path.join('src', platform)), compile_map)

    # Make an archive of all object files
    obj_paths = []
    for dir, _, f in os.walk('bin'):
        if len(f) != 0:
            obj_paths.append(os.path.join(dir, '*.obj'))
    subprocess.run(['llvm-ar', 'rc', 'ciabatta.lib'] + obj_paths)
else:
    src = os.path.join('src', 'code', '*.c')
    os_src = os.path.join('src', platform)
    cmd = cuik_flags + '-c -o ciabatta.obj'.split(' ') + [src, os_src]
    print(cmd)
    subprocess.run(['cuik'] + cmd)
    subprocess.run('lib', '/out:ciabatta.lib', 'ciabatta.obj')