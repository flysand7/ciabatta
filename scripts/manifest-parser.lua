#!/usr/bin/env lua

local path = require 'path'

levels = {}
parser_obj = {}
parser_cur_ind = 0
parser_ind_lv = 0

-- Tokens
Token = {}
function Token.new(kind, value)
    return {kind = kind, value = value}
end
function Token.keyword(kw)  return Token.new('keyword', kw) end
function Token.string(str)  return Token.new('string', str) end
function Token.lparen()     return Token.new('(', nil) end
function Token.rparen()     return Token.new(')', nil) end
function Token.lbrace()     return Token.new('{', nil) end
function Token.rbrace()     return Token.new('}', nil) end
function Token.colon()      return Token.new(':', nil) end
function Token.comma()      return Token.new(',', nil) end
function Token.eof()        return Token.new('eof', nil) end

Parser = {
    file = nil,
    filename = nil,
    row = 0,
    col = 0,
    ind_spaces = 0,
    ind_levels = {0},
    c = nil, -- Last Read Character
    t = nil, -- Last Read Token
}
-- Character functions
function Parser.char_next(parser, c)
    char = parser.file:read(1)
    parser.col = parser.col + 1
    if char == '\n' then
        parser.row = parser.row + 1
        parser.col = 0
    end
    parser.c = char
end
function Parser.char(parser)
    return parser.c
end
function Parser.char_is(parser, pattern)
    match = parser:char():match(pattern)
    if match then
        return true
    else
        return false
    end
end
function Parser.char_match(parser, pattern)
    if parser:char_is(pattern) then
        parser:char_next()
        return true
    end
    return false
end
-- Token functions
function Parser.token(parser)
    return parser.t
end
function Parser.token_next(parser)
    -- Skip over the whitespace
    while true do
        if parser.c == nil then
            parser.t = Token.eof()
            return
        elseif parser:char_match('#') then
            while not parser:char_match('\n') do
                parser:char_next()
            end
        elseif parser:char_is('%s') then
            parser:char_next()
        else
            break
        end
    end
    -- print(1+parser.row, 1+parser.col, parser:char())
    -- Keyword/identifier
    if parser:char_is('[%a-_]') then
        ident = ''
        while parser:char_is('[%a%d-_]') do
            ident = ident .. parser:char()
            parser:char_next()
        end
        parser.t = Token.keyword(ident)
        return
    -- String
    elseif parser:char_match('"') then
        string = ''
        while not parser:char_match('"') do
            string = string .. parser:char()
            parser:char_next()
        end
        parser.t = Token.string(string)
        return
    -- Single-char tokens
    elseif parser:char_match('%(') then
        parser.t = Token.lparen()
    elseif parser:char_match('%)') then
        parser.t = Token.rparen()
    elseif parser:char_match('{') then
        parser.t = Token.lbrace()
    elseif parser:char_match('}') then
        parser.t = Token.rbrace()
    elseif parser:char_match(':') then
        parser.t = Token.colon()
    elseif parser:char_match(',') then
        parser.t = Token.comma()
    end
end
function Parser.token_is(parser, kind)
    return parser:token().kind == kind
end
function Parser.token_kw_is(parser, name)
    return parser:token().kind == 'keyword' and parser:token().value == name
end
function Parser.token_match(parser, kind)
    if parser:token().kind == kind then
        parser:token_next()
        return true
    end
    return false
end
function Parser.token_kw_match(parser, name)
    if parser:token().kind == 'keyword' and parser:token().value == name then
        parser:token_next()
        return true
    end
    return false
end

function Parser.new(filename)
    file = io.open(filename, 'rb')
    parser = Parser
    parser.file = file
    parser.filename = filename
    parser:char_next()
    parser:token_next()
    return parser;
end

function parse_mf(mf_path)
    parser = Parser.new(mf_path)
    includes = {}
    platforms = {}
    apis = {}
    while parser:token().kind ~= 'eof' do
        if parser:token().kind ~= 'keyword' then
            print(('%s(%d, %d): Expected keyword'):format(parser.filename, 1+parser.row, 1+parser.col))
        end
        -- TODO: add error handling
        if parser:token_kw_match('include') then
            parser:token_match('{')
            while not parser:token_match('}') do
                inc = parser:token().value
                table.insert(includes, inc)
                parser:token_next()
                parser:token_match(',')
            end
        elseif parser:token_kw_match 'platform' then
            platform_name = parser:token().value
            parser:token_next()
            parser:token_match('(')
            platform_path = parser:token().value
            if platform_path:sub(1,1) == '/' then
                platform_path = platform_path:sub(2, -1)
            end
            parser:token_next()
            parser:token_match(')')
            parser:token_match('{')
            platform_includes = {}
            while not parser:token_match('}') do
                if parser:token_kw_match('tinyrt') then
                    table.insert(platform_includes, path.join(platform_path, 'tinyrt.iface.h'))
                    table.insert(platform_includes, 'tinyrt.h')
                    parser:token_match('{')
                    while not parser:token_match('}') do
                        inc = parser:token().value
                        table.insert(platform_includes, path.join(platform_path, inc))
                        parser:token_next()
                    end
                else
                    inc = parser:token().value
                    table.insert(platform_includes, path.join(platform_path, inc))
                    parser:token_next()
                end
                parser:token_match(',')
            end
            table.insert(platforms, {
                name = platform_name,
                includes = platform_includes
            })
        elseif parser:token_kw_match 'api' then
            api_name = parser:token().value
            parser:token_next()
            parser:token_match('(')
            api_path = parser:token().value
            if api_path:sub(1,1) == '/' then
                api_path = api_path:sub(2, -1)
            end
            parser:token_next()
            parser:token_match(')')
            parser:token_match('{')
            api_includes = {}
            rt_deps = {}
            while not parser:token_match('}') do
                if parser:token_kw_match('tinyrt') then
                    parser:token_match('{')
                    while not parser:token_match('}') do
                        dep = parser:token().value
                        table.insert(rt_deps, dep)
                        parser:token_next()
                    end
                else
                    inc = parser:token().value
                    table.insert(api_includes, path.join(api_path, inc))
                    parser:token_next()
                end
                parser:token_match(',')
            end
            table.insert(apis, {
                name = api_name,
                deps = rt_deps,
                includes = api_includes
            })
        else
            print(('%s(%d, %d): Unknown directive: %s'):format(parser.filename, 1+parser.row, 1+parser.col, parser:token().value))
        end
    end
    io.close(parser.file)
    return {
        includes = includes,
        platforms = platforms,
        apis = apis,
    }
end

function print_r(arr, indentLevel)
    local str = ""
    local indentStr = ""
    if(indentLevel == nil) then
        print(print_r(arr, 0))
        return
    end
    for i = 0, indentLevel do
        indentStr = indentStr.."  "
    end
    for index,value in pairs(arr) do
        if type(value) == "table" then
            str = str..indentStr..index..": \n"..print_r(value, (indentLevel + 1))
        else 
            str = str..indentStr..index..": "..value.."\n"
        end
    end
    return str
end

-- mf = parse_mf('src/library.mf')
-- print(print_r(mf, 0))
