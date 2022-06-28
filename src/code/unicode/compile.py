
import os;
import sys;

abspath = os.path.abspath(sys.argv[0])
dname = os.path.dirname(abspath)
os.chdir(dname)

with open('data.h', 'w') as header:
    header.write('\n');
    header.write('#pragma once\n\n');
    header.write('#include<unicode.h>\n');
    header.write(
'''
struct _uni_elm {
    uchar_t code;
    int     cat;
    uchar_t lower;
    uchar_t upper;
} uni_codepoints[] = {
''');

    with open('data.txt') as file:
        for line in file:
            row = line.split(';')
            code  = row[0].strip()
            cat   = row[2].strip()
            lower = row[13].strip()
            upper = row[14].strip()
            if lower == '':
                lower = code
            if upper == '' or upper == '\n':
                upper = code
            header.write('    {' + \
                '0x'     + code  + ', ' + \
                'UCHAR_' + cat   + ', ' + \
                '0x'     + lower + ', ' + \
                '0x'     + upper + '},\n'
            );

    header.write('};\n\n');
    header.close();