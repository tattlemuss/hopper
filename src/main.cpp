#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <vector>
#include <string>

#include "buffer.h"
#include "decode.h"
#include "instruction.h"
#include "symbols.h"

const char* instruction_names[Opcode::COUNT] =
{
	"none",
	"abcd",
	"add",
	"adda",
	"addi",
	"addq",
	"addx",
	"and",
	"andi",
	"asl",
	"asr",
	"bcc",
	"bchg",
	"bclr",
	"bcs",
	"beq",
	"bge",
	"bgt",
	"bhi",
	"ble",
	"bls",
	"blt",
	"bmi",
	"bne",
	"bpl",
	"bra",
	"bset",
	"bsr",
	"btst",
	"bvc",
	"bvs",
	"chk",
	"clr",
	"cmp",
	"cmpi",
	"cmpa",
	"cmpm",
	"dbcc",
	"dbcs",
	"dbeq",
	"dbf",
	"dbge",
	"dbgt",
	"dbhi",
	"dble",
	"dbls",
	"dblt",
	"dbmi",
	"dbne",
	"dbpl",
	"dbra",
	"dbvc",
	"dbvs",
	"divs",
	"divu",
	"eor",
	"eori",
	"exg",
	"ext",
	"illegal",
	"jmp",
	"jsr",
	"lea",
	"link",
	"lsl",
	"lsr",
	"move",
	"movea",
	"movem",
	"movep",
	"moveq",
	"muls",
	"mulu",
	"nbcd",
	"neg",
	"negx",
	"nop",
	"not",
	"or",
	"ori",
	"pea",
	"reset",
	"rol",
	"ror",
	"roxl",
	"roxr",
	"rte",
	"rtr",
	"rts",
	"sbcd",
	"scc",
	"scs",
	"seq",
	"sf",
	"sge",
	"sgt",
	"shi",
	"sle",
	"sls",
	"slt",
	"smi",
	"sne",
	"spl",
	"st",
	"stop",
	"sub",
	"suba",
	"subi",
	"subq",
	"subx",
	"svc",
	"svs",
	"swap",
	"tas",
	"trap",
	"trapv",
	"tst",
	"unlk"
};

// ----------------------------------------------------------------------------
//	INSTRUCTION ANALYSIS
// ----------------------------------------------------------------------------
// Check if an instruction jumps to another known address, and return that address
bool calc_relative_address(const operand& op, uint32_t inst_address, uint32_t& target_address)
{
	if (op.type == PC_DISP)
	{
		// Relative
		int16_t disp = op.pc_disp.disp;

		// The base PC is always +2 from the instruction address, since
		// the 68000 has already fetched the header word by then
		target_address = inst_address + 2 + disp;
		return true;
	}
	else if (op.type == PC_DISP_INDEX)
	{
		// Relative
		int8_t disp = op.pc_disp_index.disp;

		// The base PC is always +2 from the instruction address, since
		// the 68000 has already fetched the header word by then
		target_address = inst_address + 2 + disp;
		return true;
	}
	if (op.type == RELATIVE_BRANCH)
	{
		// Relative
		int16_t disp = op.relative_branch.disp;

		// The base PC is always +2 from the instruction address, since
		// the 68000 has already fetched the header word by then
		target_address = inst_address + 2 + disp;
		return true;
	}
	return false;
}


// ----------------------------------------------------------------------------
//	HIGHER-LEVEL DISASSEMBLY CREATION
// ----------------------------------------------------------------------------
// Storage for an attempt at tokenising the memory
class disassembly
{
public:
	struct line
	{
		uint32_t    address;
		instruction inst;
	};

	std::vector<line>    lines;
};

int decode_buf(buffer_reader& buf, const symbols& symbols, disassembly& disasm)
{
	while (buf.get_remain() >= 2)
	{
		disassembly::line line;
		line.address = buf.get_pos();

		// decode uses a copy of the buffer state
		buffer_reader buf_copy(buf);
		int res = decode(buf_copy, line.inst);

		// Handle failure
		disasm.lines.push_back(line);

		buf.advance(line.inst.byte_count);
	}
	return 0;
}

// ----------------------------------------------------------------------------
//	INSTRUCTION DISPLAY FORMATTING
// ----------------------------------------------------------------------------
static const char* g_reg_names[] =
{
	"d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7",
	"a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7"
};

// ----------------------------------------------------------------------------
void print(const operand& operand, const symbols& symbols, uint32_t inst_address, FILE* pFile)
{
	switch (operand.type)
	{
		case OpType::D_DIRECT:
			fprintf(pFile, "d%d", operand.d_register.reg);
			return;
		case OpType::A_DIRECT:
			fprintf(pFile, "a%d", operand.a_register.reg);
			return;
		case OpType::INDIRECT:
			fprintf(pFile, "(a%d)", operand.indirect.reg);
			return;
		case OpType::INDIRECT_POSTINC:
			fprintf(pFile, "(a%d)+", operand.indirect_postinc.reg);
			return;
		case OpType::INDIRECT_PREDEC:
			fprintf(pFile, "-(a%d)", operand.indirect_predec.reg);
			return;
		case OpType::INDIRECT_DISP:
			fprintf(pFile, "%d(a%d)", operand.indirect_disp.disp, operand.indirect_disp.reg);
			return;
		case OpType::INDIRECT_INDEX:
			fprintf(pFile, "%d(a%d,d%d.%s)",
					operand.indirect_index.disp,
					operand.indirect_index.a_reg,
					operand.indirect_index.d_reg,
					operand.indirect_index.is_long ? "l" : "w");
			return;
		case OpType::ABSOLUTE_WORD:
			if (operand.absolute_word.wordaddr & 0x8000)
				fprintf(pFile, "$ffff%x.w", operand.absolute_word.wordaddr);
			else
				fprintf(pFile, "$%x.w", operand.absolute_word.wordaddr);
			return;
		case OpType::ABSOLUTE_LONG:
		{
			symbol sym;
			if (find_symbol(symbols, operand.absolute_long.longaddr, sym))
				fprintf(pFile, "%s", sym.label.c_str());
			else
				fprintf(pFile, "$%x.l",
					operand.absolute_long.longaddr);
			return;
		}
		case OpType::PC_DISP:
		{
			symbol sym;
			uint32_t target_address;
			calc_relative_address(operand, inst_address, target_address);
			if (find_symbol(symbols, target_address, sym))
				fprintf(pFile, "%s(pc)", sym.label.c_str());
			else
				fprintf(pFile, "$%x(pc)", target_address);
			return;
		}
		case OpType::PC_DISP_INDEX:
		{
			symbol sym;
			uint32_t target_address;
			calc_relative_address(operand, inst_address, target_address);

			if (find_symbol(symbols, target_address, sym))
			{
				fprintf(pFile, "%s(pc,d%d.%s)",
						sym.label.c_str(),
						operand.pc_disp_index.d_reg,
						operand.pc_disp_index.is_long ? "l" : "w");
			}
			else
			{
				fprintf(pFile, "$%x(pc,d%d.%s)",
					target_address,
					operand.pc_disp_index.d_reg,
					operand.pc_disp_index.is_long ? "l" : "w");

			}
			return;
		}
		case OpType::MOVEM_REG:
		{
			bool first = true;
			for (int i = 0; i < 16; ++i)
				if (operand.movem_reg.reg_mask & (1 << i))
				{
					if (!first)
						fprintf(pFile, "/");
					fprintf(pFile, "%s", g_reg_names[i]);
					first = false;
				}
			return;
		}
		case OpType::RELATIVE_BRANCH:
		{
			symbol sym;
			uint32_t target_address;
			calc_relative_address(operand, inst_address, target_address);
			if (find_symbol(symbols, target_address, sym))
				fprintf(pFile, "%s", sym.label.c_str());
			else
				fprintf(pFile, "$%x", target_address);
			return;
		}

		case OpType::IMMEDIATE:
			fprintf(pFile, "#$%x", operand.imm.val0);
			return;
		case OpType::SR:
			fprintf(pFile, "sr");
			return;
		case OpType::USP:
			fprintf(pFile, "usp");
			return;
		case OpType::CCR:
			fprintf(pFile, "ccr");
			return;
		default:
			fprintf(pFile, "???");
	}
}

// ----------------------------------------------------------------------------
void print(const instruction& inst, const symbols& symbols, uint32_t inst_address, FILE* pFile)
{
	if (inst.opcode == Opcode::NONE)
	{
		fprintf(pFile, "dc.w $%x", inst.header);
		return;
	}
	fprintf(pFile, "%s", instruction_names[inst.opcode]);

	switch (inst.suffix)
	{
		case Suffix::BYTE:
			fprintf(pFile, ".b"); break;
		case Suffix::WORD:
			fprintf(pFile, ".w"); break;
		case Suffix::LONG:
			fprintf(pFile, ".l"); break;
		case Suffix::SHORT:
			fprintf(pFile, ".s"); break;
		default:
			break;
	}

	if (inst.op0.type == OpType::INVALID)
		return;
	fprintf(pFile, " ");
	print(inst.op0, symbols, inst_address, pFile);

	if (inst.op1.type == OpType::INVALID)
		return;
	fprintf(pFile, ",");
	print(inst.op1, symbols, inst_address, pFile);
}

// ----------------------------------------------------------------------------
int print(const symbols& symbols, const disassembly& disasm, FILE* pOutput)
{
	for (size_t i = 0; i < disasm.lines.size(); ++i)
	{
		const disassembly::line& line = disasm.lines[i];

		// TODO very naive label check
		symbol sym;
		if (find_symbol(symbols, line.address, sym))
			fprintf(pOutput, "%s:\n", sym.label.c_str());

		//fprintf(pOutput, ">> %04x:   $%04x ", line.address, line.inst.header);
		fprintf(pOutput, "\t");
		print(line.inst, symbols, line.address, pOutput);

		fprintf(pOutput, "\n");
	}
	return 0;
}

// ----------------------------------------------------------------------------
// Find addresses referenced by disasm instructions and add them to the
// symbol table
void add_reference_symbols(const disassembly& disasm, symbols& symbols)
{
	uint32_t label_id = 0;
	for (size_t i = 0; i < disasm.lines.size(); ++i)
	{
		const disassembly::line& line = disasm.lines[i];
		uint32_t target_address;

		if (calc_relative_address(line.inst.op0, line.address, target_address))
		{
			symbol sym;
			if (!find_symbol(symbols, target_address, sym))
			{
				sym.address = target_address;
				sym.section = symbol::section_type::TEXT;
				sym.label = std::string("L") + std::to_string(label_id);
				add_symbol(symbols, sym);
				++label_id;
			}
		}
		if (calc_relative_address(line.inst.op1, line.address, target_address))
		{
			symbol sym;
			if (!find_symbol(symbols, target_address, sym))
			{
				sym.address = target_address;
				sym.section = symbol::section_type::TEXT;
				sym.label = std::string("L") + std::to_string(label_id);
				add_symbol(symbols, sym);
				++label_id;
			}
		}
	}


}

// ----------------------------------------------------------------------------
//	TOS EXECUTABLE READING
// ----------------------------------------------------------------------------
struct tos_header
{
   //  See http://toshyp.atari.org/en/005005.html for TOS header details
	uint16_t  ph_branch;	  /* Branch to start of the program  */
							  /* (must be 0x601a!)			   */

	uint32_t  ph_tlen;		  /* Length of the TEXT segment	  */
	uint32_t  ph_dlen;		  /* Length of the DATA segment	  */
	uint32_t  ph_blen;		  /* Length of the BSS segment	   */
	uint32_t  ph_slen;		  /* Length of the symbol table	  */
	uint32_t  ph_res1;		  /* Reserved, should be 0;		  */
							  /* Required by PureC			   */
	uint32_t  ph_prgflags;	  /* Program flags				   */
	uint16_t  ph_absflag;	  /* 0 = Relocation info present	 */
};

// ----------------------------------------------------------------------------
//	DRI SYMBOL READING
// ----------------------------------------------------------------------------
static const int DRI_TABLE_SIZE = 14;
static const uint16_t DRI_EXT_SYMBOL_FLAG = 0x0048;
static const uint16_t DRI_SECT_TEXT = 0x0200;
static const uint16_t DRI_SECT_DATA = 0x0400;
static const uint16_t DRI_SECT_BSS  = 0x0100;

int read_symbols(buffer_reader& buf, const tos_header& header, symbols& symbols)
{
	// Calculate text, data and bss addresses
	uint32_t text_address = 0;
	uint32_t data_address = text_address + header.ph_tlen;
	uint32_t bss_address  = data_address + header.ph_dlen;

	while (buf.get_remain() >= DRI_TABLE_SIZE)
	{
		uint8_t name[8 + 14 + 1];
		uint16_t symbol_id;
		uint32_t symbol_address;

		// Name (8 bytes) -> ID (2 bytes) -> address (4 bytes)
		memset(name, 0, sizeof(name));
		if (buf.read(name, 8))
			return 1;
		if (buf.read_word(symbol_id))
			return 1;
		if (buf.read_long(symbol_address))
			return 1;

		// Looks like either bit denotes an extended symbol??
		if ((symbol_id & DRI_EXT_SYMBOL_FLAG) != 0)
		{
			// 14 bytes: symbol name extended
			if (buf.read(name + 8, 14))
				return 1;
		}

		symbol sym;
		sym.label = std::string((const char*)name);
		sym.address = symbol_address;
		sym.section = symbol::section_type::UNKNOWN;

		// Parse the flags to work out which section, then resolve an address
		switch (symbol_id & 0xf00)
		{
			case DRI_SECT_TEXT: sym.section = symbol::section_type::TEXT; sym.address += text_address; break;
			case DRI_SECT_DATA: sym.section = symbol::section_type::DATA; sym.address += data_address; break;
			case DRI_SECT_BSS:  sym.section = symbol::section_type::BSS;  sym.address += bss_address;  break;
			default:
				break;
		}
		if (sym.section != symbol::section_type::UNKNOWN)
			add_symbol(symbols, sym);
	}

	return 0;
}

// ----------------------------------------------------------------------------
int process_tos_file(const uint8_t* pData, long size, FILE* pOutput)
{
	buffer_reader buf(pData, size);
	tos_header header = {};

	if (buf.read_word(header.ph_branch))
		return 1;
	if (buf.read_long(header.ph_tlen))
		return 1;
	if (buf.read_long(header.ph_dlen))
		return 1;
	if (buf.read_long(header.ph_blen))
		return 1;
	if (buf.read_long(header.ph_slen))
		return 1;
	if (buf.read_long(header.ph_res1))
		return 1;
	if (buf.read_long(header.ph_prgflags))
		return 1;
	if (buf.read_word(header.ph_absflag))
		return 1;

	if (header.ph_branch != 0x601a)
		return 1;

	// Next section is text
	fprintf(pOutput, "; Reading text section\n");
	buffer_reader text_buf(buf.get_data(), header.ph_tlen);

	// Skip the text
	buf.advance(header.ph_tlen);
	buffer_reader data_buf(buf.get_data(), header.ph_dlen);

	// Skip the data
	buf.advance(header.ph_dlen);

	// (No BSS in the file, so symbols should be next)
	buffer_reader symbol_buf(buf.get_data(), header.ph_slen);

	symbols exe_symbols;

	fprintf(pOutput, "; Reading symbols...\n");
	int ret = read_symbols(symbol_buf, header, exe_symbols);

	disassembly disasm;
	if (decode_buf(text_buf, exe_symbols, disasm))
		return 1;

	add_reference_symbols(disasm, exe_symbols);

	print(exe_symbols, disasm, pOutput);
	return 0;
}

// ----------------------------------------------------------------------------
int main(int argc, char** argv)
{
	if (argc < 2)
	{
		fprintf(stderr, "No filename\n");
		return 1;
	}
	FILE* pInfile = fopen(argv[1], "rb");
	if (!pInfile)
	{
		fprintf(stderr, "Can't read file\n");
		return 1;
	}

	int r = fseek(pInfile, 0, SEEK_END);
	long size = ftell(pInfile);

	rewind(pInfile);

	uint8_t* pData = (uint8_t*) malloc(size);
	int readBytes = fread(pData, 1, size, pInfile);
	fclose(pInfile);

	return process_tos_file(pData, size, stdout);
}
