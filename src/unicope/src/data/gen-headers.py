
import os;
import sys;

abspath = os.path.abspath(sys.argv[0])
dname = os.path.dirname(abspath)
os.chdir(dname)

with open('unicode.h', 'w') as header:
    header.write('\n');
    header.write('#pragma once\n\n');
    header.write('#include <unicope.h>\n');
    header.write(
'''

#pragma pack(push, 1)

typedef struct char_cat_props_t char_cat_props_t;
struct char_cat_props_t {
    uint16_t       comb_class;
    uint8_t        cat_gen;
    uint8_t        cat_bidi;
};

typedef struct char32_props char32_props;
struct char32_props {
    double         num_value;
    char32_t const dec_map[8];
    uint8_t        dec_type;
    char32_t       code;
    char32_t       lower;
    char32_t       upper;
    char32_t       title;
    uint8_t        dec_map_n;
};

typedef struct char16_props char16_props;
struct char16_props {
    double         num_value;
    char32_t const dec_map[8];
    uint8_t        dec_type;
    char16_t       code;
    char16_t       lower;
    char16_t       upper;
    char16_t       title;
    uint8_t        dec_map_n;
};\n
''');


    header.write('static char const name_buff[] = {\n');
    header.write('    "\\0"\n');
    offsets = [0]
    offset = 1
    prev_idx = -1
    with open('UnicodeData.txt') as file:
        for line in file:
            row = line.split(';')
            code       = row[0].strip()
            name       = row[1].strip()
            header.write('    "%s\\0"\n' % (name))
            offset += len(name) + 1
            idx = int(code, 16)
            while idx - prev_idx != 1:
                offsets.append(0)
                prev_idx += 1
            prev_idx = idx
            if code == 'F0000':
                break
    header.write('};\n\n');

    header.write('static char const *char_names[] = {\n');
    for o in offsets:
        header.write('    name_buff+%s,\n' % (o))
    header.write('};\n\n');

    header.write('static uint8_t cat_block_indices[] = {\n');

    block_size = 2**7
    block_id = 0
    block_ent_id = 0
    cur_block = []
    blocks = []
    
    def blkcmp(b1, b2):
        for b in range(block_size):
            if b1[b] != b2[b]:
                return False
        return True


    with open('UnicodeData.txt') as file:
        for line in file:
            row = line.split(';')
            code       = int(row[0].strip(), 16)
            cat_gen    = row[2].strip()
            cat_bidi   = row[4].strip()
            comb_class = row[3].strip()
            mirrored   = row[9].strip()
            cur_block.append((comb_class, cat_gen, cat_bidi, mirrored))
            block_ent_id += 1
            if block_ent_id == block_size:
                existing_found = False
                cur_block_id = block_id
                for i,b in enumerate(blocks):
                    if blkcmp(b, cur_block):
                        existing_found = True
                        cur_block_id = i
                if not existing_found:
                    blocks.append(cur_block)
                    block_id += 1
                header.write('    %s,\n' % (cur_block_id))
                block_ent_id = 0
                cur_block = []
    header.write('};\n\n')

    header.write('static char_cat_props_t char_cat_props[][%s] = {\n' % (block_size));
    for b in blocks:
        header.write('    {\n')
        for d in b:
            cat = 'UCHAR_'+d[1];
            if d[3] == 'Y':
                cat += '|0x40'
            header.write('        {%s,%s,UCHAR_BIDI_%s},\n' % (d[0], cat, d[2]))
        header.write('    },\n')
    header.write('};\n\n');

    tab32started = False
    prev_idx = -1
    header.write('static char16_props unicode_data16[] = {\n')
    with open('UnicodeData.txt') as file:
        for line in file:
            row = line.split(';')
            code       = row[0].strip()
            if int(code, 16) > 0xffff and not tab32started:
                header.write('\n};\n\nstatic char32_props unicode_data32[] = {\n')
                tab32started=True
                prev_idx = -1
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
            # Special cases
            if code == 'FDFA':
                dec_map = '0'
            idx = int(code,16)
            if tab32started:
                idx -= 0x10000
            contiguous = idx - prev_idx == 1
            prev_idx = idx
            if cat_gen == 'Co':
                continue;
            header.write(
            '''
    %s{
        .code = 0x%s,
        .dec_map_n = %s,
        .dec_map = {%s},
        .dec_type = UCHAR_DECOMP_%s,
        .num_value = %s,
        .lower = 0x%s,
        .upper = 0x%s,
        .title = 0x%s,
    },''' % (
                '[' + hex(idx) + '] =' if not contiguous else '',
                code,
                dec_map_n,
                dec_map,
                dec_type,
                num_value if num_value != '' else dec_value if dec_value != '' else dig_value if dig_value != '' else '0',
                lower,
                upper,
                title
            ));

    header.write('\n};\n#pragma pack(pop)\n');
    header.close();