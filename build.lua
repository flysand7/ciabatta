#!/usr/bin/env lua

local path = require 'path'
local argparse = require 'argparse'
local mf_parser = require 'scripts.manifest-parser'

-- Parse command line arguments
local parser = argparse('build.lua', 'Ciabatta build script')
parser:flag('-c --clean', 'Remove all the binaries before recompiling')
parser:flag('-o --only', 'Do not compile ciabatta')
parser:flag('-r --release', 'Compile the release version (without it will compile everything in debug mode)')
parser:option('-p --platform', 'OS to compile for (linux, windows)')
parser:option('-l --library', 'Type of library to compile (static, shared)')
parser:option('-t --test', 'Compile a C file and link the library against it')
parser:option('--options', 'Additional options to provide to the executable')
local args = parser:parse()
local is_clean = args.clean or false
local is_only = args.only or false
local is_release = args.release or false
local platform = args.platform or 'linux'
local library_type = args.library or 'static'
local test_file = args.test
local compiler_options = args.options or ''

-- Clean the build files if needed
function rmdir(p)
    if not path.exists(p) then
        return
    end
    path.each(path.join(p,"*"), function(P) path.remove(P) end,
        {param = "f",delay = true,recurse = true,reverse = true})
    path.remove(p)
end
if is_clean then
    print('Cleaning files..')
    rmdir('lib')
    rmdir('bin')
    os.remove('a')
    os.remove('a.exe')
end

-- If we only needed to clean the build files, just exit here
if is_only then
    os.exit(0)
end

local assembler = 'nasm'
local compiler = 'clang'
local linker = 'ld'
local includes = {'./include'}
local compiler_defines = {}
local compiler_flags = {'-nostdlib'}
local ciabatta_lib = ''

-- Figure out additional flags
if is_release then
    table.insert(compiler_flags, '-O2')
    table.insert(compiler_defines, 'NDEBUG')
else
    table.insert(compiler_flags, '-g')
    table.insert(compiler_flags, '-O0')
    table.insert(compiler_defines, 'CIA_DEBUG')
end
if library_type == 'static' then
    ciabatta_lib = 'ciabatta.a'
    if platform == 'windows' then
        ciabatta_lib = 'ciabatta.lib'
    end
elseif library_type == 'shared' then
    ciabatta_lib = 'ciabatta.so'
    if platform == 'windows' then
        ciabatta_lib = 'ciabatta.dll'
    end
else
    print('Invalid library type: ' .. library_type)
end

-- Turn flags into table
function map(t, f)
    local t1 = {}
    local t_len = #t
    for i = 1, t_len do
        t1[i] = f(t[i])
    end
    return t1
end
function quote(str)
    return '"'..str..'"'
end
function prefix(prefix)
    return function(str)
        return prefix..str
    end
end
function prefix_quote(prefix)
    return function(str)
        return prefix..'"'..str..'"'
    end
end

-- Generate TinyRT interface file for the platform
print('==> Generating TinyRT interface definitions')
local tinyrt_manifest_path = path.join('src', platform, 'tinyrt.mf')
if not path.exists(tinyrt_manifest_path) then
    print('ERROR: tinyrt manifest wasnt found: '..tinyrt_manifest_path)
end
tinyrt_iface_hdr = io.open(path.join('src', platform, 'tinyrt.iface.h'), 'wb')
tinyrt_iface_hdr:write('\n')
tinyrt_iface_hdr:write('// This file is AUTO-GENERATED\n')
tinyrt_iface_hdr:write('// See tinyrt.mf\n')
tinyrt_iface_hdr:write('\n')
n = 1
tinyrt_apis = {}
for line in io.lines(tinyrt_manifest_path) do
    if line:len() ~= 0 and line:sub(1,1) ~= '#' and line:gsub('%s+', '') ~= '' then
        local line_it = line:gmatch('[_a-zA-Z0-9]+')
        local api_name = line_it()
        local has_impl = line_it()
        if has_impl == '0' or has_impl == '1' then
            local api_define = '#define ' .. (api_name:upper()) .. ' '..has_impl..'\n'
            tinyrt_iface_hdr:write(api_define)
            table.insert(tinyrt_apis, api_name)
        else
            print('SYNTAX ERROR AT LINE '..n..': Expected 1 or 0 for the value')
        end
        
    end
    n = n+1
end
io.close(tinyrt_iface_hdr)
-- Parse manifest and generate ciabatta.c
local function has_value(tab, val)
    for index, value in ipairs(tab) do
        if value == val then
            return true
        end
    end

    return false
end
print('==> Generating ciabatta.c')
cia_h = io.open(path.join('src', 'ciabatta.c'), 'wb')
mf = parse_mf(path.join('src', 'library.mf'))
cia_h:write('\n')
cia_h:write('// THIS FILE IS AUTO-GENERATED. SEE library.mf FOR DETAILS\n')
cia_h:write('\n// global includes\n')
for index,include in ipairs(mf.includes) do
    cia_h:write('#include <'..include..'>\n')
end
for index,decl_platform in ipairs(mf.platforms) do
    if decl_platform.name == platform then
        cia_h:write(('\n// platform %s\n'):format(decl_platform.name))
        for index,include in ipairs(decl_platform.includes) do
            cia_h:write('#include "'..include..'"\n')
        end
    end
end
for index, api in ipairs(mf.apis) do
    supported = true
    for index, dep in ipairs(api.deps) do
        if not has_value(tinyrt_apis, dep) then
            supported = false
        end
    end
    if supported then
        cia_h:write(('\n// module %s\n'):format(api.name))
        print('  - Exporting module: ' .. api.name)
        for index, include in ipairs(api.includes) do
            cia_h:write('#include "'..include..'"\n')
        end
    end
end
io.close(cia_h)


-- Figure out compiler flags
local cflags = table.concat(compiler_flags, ' ')..' '..
               table.concat(map(compiler_defines, prefix('-D ')), ' ')..' '..
               table.concat(map(includes, prefix_quote('-I ')), ' ')..' '

print('==> Compiling ciabatta')
print('Flags: ' .. cflags)

-- Functions for compiling, linking and assembling individual files
function assemble(src, out)
    local format = 'elf64'
    if platform == 'windows' then
        format = 'win64'
    end
    local cmdline = 'nasm -f '..format..' "'..src..'" -o "'..out..'"'
    print('> '..cmdline)
    os.execute(cmdline)
end

function compile(srcs, out, additional_flags)
    local flags = (additional_flags or '')..' '..cflags
    local inputs = table.concat(map(srcs, quote), ' ')
    local cmdline = 'clang '..flags..' '..inputs..' -o '..quote(out)..''
    print('> '..cmdline)
    os.execute(cmdline)
end

function archive(srcs, out)
    os.remove(out)
    local inputs = table.concat(map(srcs, quote), ' ')
    local cmdline = 'llvm-ar -rcs '..quote(out)..' '..inputs
    print('> '..cmdline)
    os.execute(cmdline)
end

-- Build ciabatta
path.mkdir('lib')
path.mkdir('bin')

assemble('src/linux/crt-entry.asm', 'bin/crt-entry.o')
compile({'src/linux/crt-ctors.c'}, 'bin/crt-ctors.o', '-fpic -c')
compile({'src/ciabatta.c'}, 'bin/ciabatta.o', '-fpic -c')

if library_type == 'static' then
    archive({'bin/ciabatta.o', 'bin/crt-ctors.o', 'bin/crt-entry.o'}, 'lib/'..ciabatta_lib)
elseif library_type == 'shared' then
    print('SHARED OBJECTS NOT SUPPORTED YET')
    os.exit(1)
end

if test_file then
    compile({test_file, 'lib/'..ciabatta_lib}, 'a', '-pie')
end
