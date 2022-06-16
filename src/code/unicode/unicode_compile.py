
import os;
import sys;

abspath = os.path.abspath(sys.argv[0])
dname = os.path.dirname(abspath)
os.chdir(dname)

with open('unicode_data.h', 'w') as header:
    header.write('\n');
    header.write('#pragma once\n\n');
    header.write('#define Cc  0\n');
    header.write('#define Cf  1\n');
    header.write('#define Co  2\n');
    header.write('#define Cs  3\n');
    header.write('#define Ll  4\n');
    header.write('#define Lm  5\n');
    header.write('#define Lo  6\n');
    header.write('#define Lt  7\n');
    header.write('#define Lu  8\n');
    header.write('#define Mc  9\n');
    header.write('#define Me 10\n');
    header.write('#define Mn 11\n');
    header.write('#define Nd 12\n');
    header.write('#define Nl 13\n');
    header.write('#define No 14\n');
    header.write('#define Pc 15\n');
    header.write('#define Pd 16\n');
    header.write('#define Pe 17\n');
    header.write('#define Pf 18\n');
    header.write('#define Pi 19\n');
    header.write('#define Po 20\n');
    header.write('#define Ps 21\n');
    header.write('#define Sc 22\n');
    header.write('#define Sk 23\n');
    header.write('#define Sm 24\n');
    header.write('#define So 25\n');
    header.write('#define Zl 26\n');
    header.write('#define Zp 27\n');
    header.write('#define Zs 28\n');
    header.write(
'''
struct _uni_elm {
    wint_t code;
    wint_t cat;
    wint_t lower;
    wint_t upper;
} uni_codepoints[] = {
''');

    with open('unicode_data.txt') as file:
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
                '0x' + code  + ', ' + \
                cat          + ', ' + \
                '0x' + lower + ', ' + \
                '0x' + upper + '},\n');

    header.write('};\n\n');
    header.close();