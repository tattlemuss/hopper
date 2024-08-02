
""" Example

    Dn            |  8(2/0)  0(0/0) |          np |               | np          
    (An)          | 12(2/1)  4(1/0) |          np |            nr | np nw	      
    (An)+         | 12(2/1)  4(1/0) |          np |            nr | np nw	      
    -(An)         | 12(2/1)  6(1/0) |          np | n          nr | np nw	      
    (d16,An)      | 12(2/1)  8(2/0) |          np |      np    nr | np nw	      
    (d8,An,Xn)    | 12(2/1) 10(2/0) |          np | n    np    nr | np nw	      
    (xxx).W       | 12(2/1)  8(2/0) |          np |      np    nr | np nw	      
    (xxx).L       | 12(2/1) 12(3/0) |          np |   np np    nr | np nw	      
"""

ea_map = {
    "Dn" : "D_DIRECT",
    "Dm" : "D_DIRECT",
    "Dx" : "D_DIRECT",
    "Dy" : "D_DIRECT",
    "An" : "A_DIRECT",
    "(An)" : "INDIRECT",
    "(An)+" : "INDIRECT_POSTINC",
    "(Ay)+" : "INDIRECT_POSTINC",
    "(Ax)+" : "INDIRECT_POSTINC",
    "-(An)" : "INDIRECT_PREDEC",
    "-(Ay)" : "INDIRECT_PREDEC",
    "-(Ax)" : "INDIRECT_PREDEC",
    "(d16_An)" : "INDIRECT_DISP",
    "(d16_Ay)" : "INDIRECT_DISP",
    "(d8_An_Xn)" : "INDIRECT_INDEX",
    "(xxx).W" : "ABSOLUTE_WORD",
    "(xxx).L" : "ABSOLUTE_LONG",
    "#<data>" : "IMMEDIATE",
    "<label>" : "PC_DISP",
    "pcdisp" : "PC_DISP",
    "regs" : "MOVEM_REG",
    "SR" : "SR",
    "CCR" : "CCR",
    "USP" : "USP",
}

def parse_pairing(bits):
    total = 0
    parts = []
    for p in bits:
        parts += p.split()
    front, back = False, False
    if len(parts) > 0:
        if parts[0] == "n":
            front = True
        if parts[-1] == "n":
            back = True

    return front, back

def parse(lines):
    linenum = 0

    curr_opcodes = None
    curr_suffixes = []
    curr_ops = ["INVALID", "INVALID"]
    curr_ea_slot = -1

    results = []
    while linenum < len(lines):
        line = lines[linenum]
        print(line)
        linenum += 1

        bits = line.split("|")

        if line.startswith(";"):
            # Command
            if line.startswith(";setinst"):
                print(line)
                codes = line.strip().split()
                print(codes)
                curr_suffixes = ["NONE"]
                curr_opcodes = codes[1:]
                curr_ops = ["INVALID", "INVALID"]
                curr_ea_slot = -1
        elif len(bits) >= 3:
            # Read cycles lines
            #inst_type, cycles1, first_op1, data_bus, instr) = bits
            inst_type = bits[0]
            cycles1 = bits[1]
            inst_type = inst_type.strip()
            if inst_type in ea_map:
                # Instruction!
                # Fill out the empty ea slot
                curr_ops[curr_ea_slot] = ea_map[inst_type]
                print("ea slot ", curr_ea_slot, "->", ea_map[inst_type])
            else:
                if len(inst_type) > 0 and inst_type[-1] == ':':
                    inst_type = inst_type[:-1].strip()
                    if inst_type == ".B":
                        curr_suffixes = ["BYTE"]
                    if inst_type == ".B or .S":
                        curr_suffixes = ["SHORT"]
                    elif inst_type == ".B or .W":
                        curr_suffixes = ["BYTE", "WORD"]
                    elif inst_type == ".B, .S or .W":   # DBcc
                        curr_suffixes = ["SHORT", "WORD"]
                    elif inst_type == ".B .S or .W":   # DBcc
                        curr_suffixes = ["SHORT", "WORD"]
                    elif inst_type == ".W":
                        curr_suffixes = ["WORD"]
                    elif inst_type == ".L":
                        curr_suffixes = ["LONG"]
                    else:
                        ops = inst_type.split(",")
                        if len(ops) > 0:
                            # Operand types or suffixes
                            curr_ea_slot = 0
                            for (i,o) in enumerate(ops):
                                if o == "<ea>":
                                    curr_ea_slot = i
                                    print("ea slot: ", i)
                                elif o in ea_map:
                                    curr_ops[i] = ea_map[o]
                                    print(o, "->", ea_map[o], "slot", i)
                                else:
                                    assert(o)
                else:
                    print("**** ERROR **** ", inst_type)

            # If there are cycles, calc them and output
            # TODO parse cycles
            parts = cycles1.split()
            if len(parts) > 0:
                print(parts)
                total = 0
                for p in parts:
                    if p.find("(") == -1:
                        continue
                    front,back = p.split("(")
                    if front.find("+") != -1:
                        fixed,diff = front.split("+")
                        total += int(fixed)
                    else:
                        total += int(front)

                # Now output the cycles for each opcode/suffix
                if total != 0:
                    front, back = parse_pairing(bits[2:])
                    flags = []
                    if front:
                        flags.append("PAIR_FRONT")
                    if back:
                        flags.append("PAIR_BACK")
                    #assert total == split_tot
                    if len(flags) == 0:
                        flags = "0"
                    else:
                        flags = "|".join(flags)

                    for op in curr_opcodes:
                        for suff in curr_suffixes:
                            print("APPEND ",op, suff, curr_ops[0], curr_ops[1], total, flags)
                            results.append((op, suff, curr_ops[0], curr_ops[1], total, flags))
                            # SPECIAL CASE handle d(pc) and d(pc,dn)
                            if (curr_ops[0] == "INDIRECT_DISP"):
                                results.append((op, suff, "PC_DISP", curr_ops[1], total, flags))
                            if (curr_ops[0] == "INDIRECT_INDEX"):
                                results.append((op, suff, "PC_DISP_INDEX", curr_ops[1], total, flags))

    return results

if __name__=='__main__':
    fh = open("Yachtv11.txt", "r")
    l = fh.readlines()
    fh.close()
    results = parse(l)

    fh = open("../hopper68/lib/timing_table.i", "w")
    for r in results:
        fh.write(" TIMING(%5s,%8s,%s,%s,%d,%s),\n" % r)
    fh.close()