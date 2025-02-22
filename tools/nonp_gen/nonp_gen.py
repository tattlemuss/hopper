from typing import List, Set

OUT_PATH = '../../hopper56/lib'
lines = open("dsp56k.mch").readlines()

class Code:
    def __init__(self):
        pass

    def __repr__(self):
        return "{0:10s} {1} [{2:20s}] {3} ".format(self.opcode,
                                        self.encoding,
                                        self.funcname,
                                        self.notes)

class FieldEntry:
    pass

class FieldMap:
    """ Records the encodings with variable fields,
        and batches them into similar types. """
    def __init__(self):
        self.map = {}
        self.used_tags = set()

    def add_field(self, field, code):
        """ Add a possible new field variant """
        if not field in self.map:
            e = FieldEntry()
            e.tag = field.replace('_', '')
            if e.tag == '':
                e.tag = 'none'
            e.codes = []
            self.map[field] = e
            assert e.tag not in self.used_tags
            self.used_tags.add(e.tag)

        self.map[field].codes.append(code)
        return self.map[field].tag

NUM_SLOTS = 64      # 6 bits
# Set of codes batched by the top 6 bits
op_map = [list() for _ in range(NUM_SLOTS)]
version_map = {}
all_funcnames = set()
all_opcodes = set()
fields_map = FieldMap()


def add_version(opcode):
    if not opcode in version_map:
        version_map[opcode] = 0
    version_map[opcode] += 1
    return version_map[opcode]


def add_code(idx, code):
    if op_map[idx] is None:
        op_map[idx] = []
    op_map[idx].append(code)


def mask_from_string(str):
    """ Convert e.g 000xxxx01010 to a mask and matching value.
    Also generate a string representing which bits have variable fields. """
    def mask_char(x):
        return '1' if x in ['0', '1'] else '0'
    def val_char(x):
        return x if x in ['0', '1'] else '0'

    def field_char(x):
        return '_' if x in ['0', '1'] else x

    m = ''.join([mask_char(x) for x in str])
    v = ''.join([val_char(x) for x in str])
    maskbitcount = m.count('1')
    field = ''.join([field_char(x) for x in str])
    return int(m, 2), int(v, 2), field, maskbitcount

""" Parse the .mch file """
for l in lines:
    if l[0] == ';':
        continue
    l = l.strip()
    blocks = l.split()
    if len(blocks) == 0:
        continue

    if blocks[0] != '-':
        opcode = "O_" + blocks[0].upper()

    all_opcodes.add(opcode)

    type = blocks[3]
    encoding = blocks[4]
    notes = ' '.join(blocks[5:])
    args = blocks[1:3]
    decode_func = blocks[5]
    # Skip the first 4 bits, which reduces the problem down to 16 bits

    # Tweak these entries to include a non-pmove version
    if type != 'NOPARMO':
        continue

    assert encoding[:5] == '%0000'
    encoding = encoding[5:]

    # The Hatari encoding scheme uses 9 bits:
	#	value = (cur_inst >> 11) & (BITMASK(6) << 3);      11111100   so bits 19,18,17,16,15,14
	#	value += (cur_inst >> 5) & BITMASK(3);                   11   so bits 7,6,5
    # This is too complicated, so we just partition with the top 6 bits.
    part1 = encoding[0:6]       # 19 onwards

    code = Code()
    code.opcode = opcode
    code.encoding = encoding    # Store the full thing
    code.notes = notes
    code.part = part1
    code.mask, code.val, code.field, code.maskbitcount = mask_from_string(code.encoding)
    code.args = args
    code.funcname = decode_func
    code.field_tag = fields_map.add_field(code.field, code)

    all_funcnames.add(code.funcname)

    mask, val, _ , _ = mask_from_string(part1)
    for test in range(0, NUM_SLOTS):
        if (test & mask) == val:
            add_code(test, code)
    #print("%s %10s %10s %s %03x %03x" % (encoding, opcode, type, part1, mask, val))

def create_decoder(codes : List[Code], idx, fh):
    masks = set()
    pairs = {}
    for code in codes:
        masks.add(code.mask)
        pair = (code.mask, code.val)
        if pair in pairs:
            print("WARNING: mask/val pair: %s Existing was %s Mask: %x Val: %x" %
                  (code.opcode, pairs[pair].opcode, code.mask, code.val))
        else:
            pairs[pair] = code

    fh.write("\n// Decode with top bits = %{0:b}\n".format(idx))
    fh.write("static int decode_idx_%02x(nonp_context& ctx)\n{\n" % idx)
    for code in codes:
        fh.write("  if ((ctx.header & 0x%05x) == 0x%05x)\n" % (code.mask, code.val))
        fh.write("     return decode_nonp_%s(ctx, Opcode::%s);\n" % (code.field_tag, code.opcode))
        #print(code.args)
    if len(codes) == 0:
        fh.write("  (void)ctx;\n") # prevent warnings

    fh.write("  return 1;\n")
    fh.write("}\n")

def write_protos(field_map):
    """ Write out prototype functions and all opcodes """
    protos_fh = open("protos.cpp", "w")

    for f in fields_map.map:
        info = fields_map.map[f]
        tag = info.tag
        protos_fh.write("\t// Decoder for field type '%s'\n" % f)
        for code in info.codes:      # set of codes using this pattern
            protos_fh.write("\t// Used in {0:s}  {1:10s} '{2:s}'\n".format(code.encoding, code.opcode, code.notes))
        protos_fh.write("\tstatic int decode_nonp_%s(nonp_context& ctx, Opcode opcode)\n" % tag)
        protos_fh.write("\t{\n")
        protos_fh.write("\t\t//printf(\"%s\\n\");\n" % f)
        protos_fh.write("\t\tctx.inst.opcode = opcode;\n")
        protos_fh.write("\t\treturn 0;\n")
        protos_fh.write("\t}\n\n")
    protos_fh.close()

opcodes = list(all_opcodes)
opcodes.sort()
opcodes.insert(0, "INVALID")
opcodes.append("OPCODE_COUNT")

def write_opcode_enum(opcodes):
    opcodes_fh = open(OUT_PATH + "/opcode.h", "w")
    opcodes_fh.write("""#ifndef HOPPER_56_OPCODE_H
#define HOPPER_56_OPCODE_H
#include <cstdint>

namespace hop56
{
""")

    # Enum
    opcodes_fh.write("\tenum Opcode\n\t{\n")
    rows = ',\n'.join(["\t\t%s" % idx for idx in opcodes])
    opcodes_fh.write(rows)
    opcodes_fh.write("\n\t};\n")

    opcodes_fh.write("""
} // namespace

#endif // HOPPER_56_OPCODE_H
""")
    opcodes_fh.close()

def write_opcode_strings(opcodes):
    # Names (strip off the "O_" at the start)
    def strip_op(x):
        return x[2:] if x.find("O_") == 0 else x
    # Strings
    opcodes_fh = open(OUT_PATH + "/opcode_strings.i", "w")
    opcodes_fh.write("namespace hop56\n")
    opcodes_fh.write("{\n")
    opcodes_fh.write("\tconst char* g_opcode_names[Opcode::OPCODE_COUNT] =\n")
    opcodes_fh.write("\t{\n")
    rows = ',\n'.join(["\t\t\"%s\"" % strip_op(idx).lower() for idx in opcodes[:-1]])
    opcodes_fh.write(rows + "\n")
    opcodes_fh.write("\t};\n")
    opcodes_fh.write("} // namespace\n")
    opcodes_fh.close()

def write_tables(op_map):
    tables_fh = open(OUT_PATH + "/nonp_tables.i", "w")
    tables_fh.write("// DO NOT EDIT\n")
    tables_fh.write("// Generated by nonp_gen.py\n\n")
    tables_fh.write("// Typedef for a single non-pmove decoder function\n")
    tables_fh.write("typedef int (*dsp_decoder)(nonp_context& ctx);\n")

    for idx in range(0, NUM_SLOTS):
        codes = list(op_map[idx])
        codes.sort(key = lambda x : x.maskbitcount, reverse=True)
        create_decoder(codes, idx, tables_fh)

    # Output the top-bit table
    rows = ',\n'.join(["\tdecode_idx_%02x" % idx for idx in range(NUM_SLOTS)])

    tables_fh.write("// Maps top 6 bits of the instruction header to decoder table functions.\n")
    tables_fh.write("static const dsp_decoder g_nonp_tables[%d] =\n" % NUM_SLOTS)
    tables_fh.write("{\n")
    tables_fh.write("%s\n" % rows)
    tables_fh.write("};\n\n")
    tables_fh.close()

write_opcode_enum(opcodes)
write_opcode_strings(opcodes)
write_tables(op_map)
write_protos(fields_map)
print("Complete. Output %d functions." % (len(all_funcnames)))