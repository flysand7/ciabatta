
import os;
import sys;

abspath = os.path.abspath(sys.argv[0])
dname = os.path.dirname(abspath)
os.chdir(dname)

with open('data.h', 'w') as header:
    header.write('\n');
    header.write('#pragma once\n\n');
    header.write('#include <unicode.h>\n');
    header.write(
'''
uchar_props unicode_data[] = {
''');

    with open('data.txt') as file:
        for line in file:
            row = line.split(';')
            code       = row[0].strip()
            name       = row[1].strip()
            cat_gen    = row[2].strip()
            cat_bidi   = row[4].strip()
            comb_class = row[3].strip()
            dec_map    = row[5].strip()
            dec_value  = row[6].strip()
            dig_value  = row[7].strip()
            num_value  = row[8].strip()
            mirrored   = row[9].strip()
            old_name   = row[10].strip()
            comment    = row[11].strip()
            upper      = row[12].strip()
            lower      = row[13].strip()
            title      = row[14].strip()
            # Process decompositional mapping
            dec_map_n  = 0
            dec_type   = 'CANON'
            if dec_map != '':
                dec_map = dec_map.split(' ')
                if dec_map[0][0] == '<':
                    dec_type = dec_map[0][1:-1].upper()
                    dec_map = dec_map[1:]
                dec_map_n = len(dec_map)
            if dec_map_n != 0:
                dec_map = ', '.join(list(map(lambda x: '0x' + x, dec_map)))
            else:
                dec_map = '0'
            # Make sure lowercase and uppercase mappings are defined
            if lower == '':
                lower = code
            if upper == '':
                upper = code
            if title == '' or title == '\n':
                title = code
            header.write(
            '''
    [0x%s] = {
        .code = 0x%s,
        .name = "%s",
        .cat_gen = UCHAR_%s,
        .cat_bidi = UCHAR_BIDI_%s,
        .comb_class = %s,
        .dec_type = UCHAR_DECOMP_%s,
        .dec_map_n = %s,
        .dec_map = {%s},
        .dec_value = %s,
        .dig_value = %s,
        .num_value = %s,
        .bidi_mirrored = %s,
        .old_name = "%s",
        .comment  = "%s",
        .lower = 0x%s,
        .upper = 0x%s,
        .title = 0x%s,
    },''' % (
                code,
                code,
                name,
                cat_gen,
                cat_bidi,
                comb_class,
                dec_type,
                dec_map_n,
                dec_map,
                dec_value if dec_value != '' else '-1',
                dig_value if dig_value != '' else '-1',
                num_value if num_value != '' else '-1',
                '1' if mirrored == 'Y' else '0',
                old_name,
                comment,
                lower,
                upper,
                title
            ));

    header.write('};\n\n');
    header.close();