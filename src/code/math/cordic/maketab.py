
import math;

import os;
import sys;

abspath = os.path.abspath(sys.argv[0])
dname = os.path.dirname(abspath)
os.chdir(dname)

data=[
    ('cordic_dataf.c', 'float', 'f',       32),
    ('cordic_data.c',  'double', '',       64),
    ('cordic_datal.c', 'long double', 'l', 64),
]

for f in data:
    fname = f[0]
    ftype = f[1]
    s = f[2]
    N = f[3]

    f = open(fname, 'w')

    tK = 1
    for i in range(1, N):
        tK *= math.cos(math.atan(2**(-i)));
    f.write(ftype + ' tK' + s + ' = ' + float.hex(tK) + ';\n')
    f.write('\n')

    hK = 1
    for i in range(1, N):
        hK *= math.cos(math.atanh(2**(-i)));
    f.write(ftype + ' hK' + s + ' = ' + float.hex(hK) + ';\n')
    f.write('\n')

    f.write(ftype + ' ttable' + s + '[] = {\n');
    for i in range(1, N):
        v = math.atan(2**(-i));
        f.write('    ' + float.hex(v) + ',\n')
    f.write('};\n')
    f.write('\n')

    f.write(ftype + ' htable' + s + '[] = {\n');
    for i in range(1, N):
        v = math.atanh(2**(-i));
        f.write('    ' + float.hex(v) + ',\n')
    f.write('};\n')
    f.write('\n')

    f.write(ftype + ' ptable' + s + '[] = {\n');
    for i in range(1, N):
        v = 2**(-i);
        f.write('    ' + float.hex(v) + ',\n')
    f.write('};\n')
    f.write('\n')

    f.close();
