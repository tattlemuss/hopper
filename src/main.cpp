#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <vector>
#include <string>
#include <cstring>

#include "buffer.h"
#include "decode.h"
#include "instruction.h"
#include "symbols.h"
#include "print.h"
#include "timing.h"

// ----------------------------------------------------------------------------
// User options for output.
struct print_settings
{
	bool show_address;
	bool show_timings;
};

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

// ----------------------------------------------------------------------------
// Read the buffer in a simple single pass.
int decode_buf(buffer_reader& buf, const decode_settings& dsettings, disassembly& disasm)
{
	while (buf.get_remain() >= 2)
	{
		disassembly::line line;
		line.address = buf.get_pos();

		// decode uses a copy of the buffer state
		buffer_reader buf_copy(buf);

		// We can ignore the return code, since it just says "this instruction is valid"
		// rather than "something catastrophic happened"
		decode(line.inst, buf_copy, dsettings);

		// Handle failure
		disasm.lines.push_back(line);

		buf.advance(line.inst.byte_count);
	}
	return 0;
}

// ----------------------------------------------------------------------------
// Print a set of diassembled lines.
int print(const symbols& symbols, const disassembly& disasm, const print_settings& psettings, FILE* pOutput)
{
	// previous flag for timing pairs
	uint8_t prev_flag = 0;

	for (size_t i = 0; i < disasm.lines.size(); ++i)
	{
		const disassembly::line& line = disasm.lines[i];
		const instruction& inst = line.inst;

		// TODO very naive label check
		symbol sym;
		if (find_symbol(symbols, line.address, sym))
			fprintf(pOutput, "%s:\n", sym.label.c_str());

		if (psettings.show_address)
		{
			fprintf(pOutput, ">> %04x:   $%04x ", line.address, line.inst.header);
		}
		fprintf(pOutput, "\t");
		print(inst, symbols, line.address, pOutput);

		if (psettings.show_timings && inst.opcode != Opcode::NONE)
		{
			timing timing;
			if (calc_timing(inst, timing) != 0)
			{
				fprintf(pOutput, "\t; ?");
			}
			else
			{
				// Adjust timing for pairing.
				// By default, round up to a multiple of four.
				uint16_t time = (timing.min + 3) & 0xfffc;
				const char* comment = "";

				// Exception: previous inst has pair_back and we have pair_front,
				// in which case we subtract 4
				if ((prev_flag & PAIR_BACK) && (timing.flags & PAIR_FRONT))
				{
					time -= 4;
					comment = " (pair)";
				}
				fprintf(pOutput, "\t; %d%s", time, comment);
			}
			prev_flag = timing.flags;
		}
		else
		{
			prev_flag = 0;
		}

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
int process_tos_file(const uint8_t* data_ptr, long size, const decode_settings& dsettings, const print_settings& psettings, FILE* pOutput)
{
	buffer_reader buf(data_ptr, size, 0);
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
	buffer_reader text_buf(buf.get_data(), header.ph_tlen, 0);

	// Skip the text
	buf.advance(header.ph_tlen);
	buffer_reader data_buf(buf.get_data(), header.ph_dlen, 0);

	// Skip the data
	buf.advance(header.ph_dlen);

	// (No BSS in the file, so symbols should be next)
	buffer_reader symbol_buf(buf.get_data(), header.ph_slen, 0);

	symbols exe_symbols;

	fprintf(pOutput, "; Reading symbols...\n");
	int ret = read_symbols(symbol_buf, header, exe_symbols);
	if (ret)
	{
		fprintf(stderr, "Error reading symbol table\n");
		return ret;
	}

	disassembly disasm;
	if (decode_buf(text_buf, dsettings, disasm))
		return 1;

	add_reference_symbols(disasm, exe_symbols);

	print(exe_symbols, disasm, psettings, pOutput);
	return 0;
}

// ----------------------------------------------------------------------------
int process_bin_file(const uint8_t* data_ptr, long size, const decode_settings& dsettings, const print_settings& psettings, FILE* pOutput)
{
	buffer_reader buf(data_ptr, size, 0);
	symbols bin_symbols;

	disassembly disasm;
	if (decode_buf(buf, dsettings, disasm))
		return 1;

	add_reference_symbols(disasm, bin_symbols);

	print(bin_symbols, disasm, psettings, pOutput);
	return 0;
}

// ----------------------------------------------------------------------------
bool get_hex_value(char c, uint8_t& val)
{
	if (c >= '0' && c <= '9')
	{
		val = c - '0';
		return true;
	}
	if (c >= 'A' && c <= 'F')
	{
		val = 10 + c - 'A';
		return true;
	}
	if (c >= 'a' && c <= 'f')
	{
		val = 10 + c - 'a';
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
int process_hex_string(const char* hex_string, const decode_settings& dsettings, const print_settings& psettings, FILE* pOutput)
{
	size_t strsize = strlen(hex_string);
	if ((strsize & 1))
	{
		fprintf(stderr, "Hex string has odd number of characters\n");
		return 1;
	}

	size_t byte_count = strsize / 2;
	uint8_t* data_ptr = (uint8_t*)malloc(byte_count);

	// Parse hex into binary data
	for (size_t i = 0; i < byte_count; ++i)
	{
		uint8_t val1, val2;
		if (!get_hex_value(hex_string[i * 2], val1) ||
			!get_hex_value(hex_string[i * 2 + 1], val2))
		{
			fprintf(stderr, "Not a valid hex string '%s'\n", hex_string);
			return 1;
		}
		data_ptr[i] = (val1 << 4) | val2;
	}

	// Wrap it up and decode
	buffer_reader buf(data_ptr, byte_count, 0);
	disassembly disasm;
	int ret = decode_buf(buf, dsettings, disasm);
	free(data_ptr);

	if (ret)
		return ret;

	// Print it out
	symbols dummy_symbols;
	print(dummy_symbols, disasm, psettings, pOutput);
	return 0;
}

// ----------------------------------------------------------------------------
// Operating mode: .prg file, binary file, or cmdline input
enum DECODE_MODE
{
	MODE_TOS = 0,
	MODE_BIN = 1,
	MODE_HEX = 2
};

// ----------------------------------------------------------------------------
int main(int argc, char** argv)
{
	if (argc < 2)
	{
		fprintf(stderr, "No filename\n");
		return 1;
	}

	DECODE_MODE mode = MODE_TOS;
	print_settings psettings = {};
	psettings.show_address = false;
	psettings.show_timings = false;
	const char* hex_data = NULL;

	decode_settings dsettings = {};
	dsettings.cpu_type = CPU_TYPE_68000;
	// NOTE: this can be replaced if mode is hex
	int last_arg = argc - 1;

	for (int opt = 1; opt < last_arg; ++opt)
	{
		if (strcmp(argv[opt], "--bin") == 0)
		{
			mode = MODE_BIN;
			last_arg = argc - 1;
		}
		else if (strcmp(argv[opt], "--address") == 0)
			psettings.show_address = true;
		else if (strcmp(argv[opt], "--timings") == 0)
			psettings.show_timings = true;
		else if (strcmp(argv[opt], "--m68010") == 0)
			dsettings.cpu_type = CPU_TYPE_68010;
		else if (strcmp(argv[opt], "--m68020") == 0)
			dsettings.cpu_type = CPU_TYPE_68020;
		else if (strcmp(argv[opt], "--m68030") == 0)
			dsettings.cpu_type = CPU_TYPE_68030;
		else if (strcmp(argv[opt], "--hex") == 0)
		{
			++opt;
			if (opt < argc)	// NOTE: not last_arg
			{
				hex_data = argv[opt];
				last_arg = argc;
				mode = MODE_HEX;
				++opt;
			}
			else
			{
				fprintf(stderr, "--hex missing data argument");
			}
		}
		else
		{
			fprintf(stderr, "Unknown switch: '%s'\n", argv[opt]);
			return 1;
		}
	}

	if (mode != MODE_HEX)
	{
		const char* fname = argv[argc - 1];
		FILE* pInfile = fopen(fname, "rb");
		if (!pInfile)
		{
			fprintf(stderr, "Can't read file: %s\n", fname);
			return 1;
		}

		(void) fseek(pInfile, 0, SEEK_END);
		long size = ftell(pInfile);

		rewind(pInfile);
		uint8_t* data_ptr = (uint8_t*) malloc(size);
		long readBytes = fread(data_ptr, 1, size, pInfile);
		fclose(pInfile);
		if (readBytes != size)
		{
			fprintf(stderr, "Failed to read file contents\n");
			return 1;
		}
		int ret = 0;
		if (mode == MODE_TOS)
			ret = process_tos_file(data_ptr, size, dsettings, psettings, stdout);
		else if (mode == MODE_BIN)
			ret = process_bin_file(data_ptr, size, dsettings, psettings, stdout);

		free(data_ptr);
		return ret;
	}
	else if (mode == MODE_HEX)
	{
		return process_hex_string(hex_data, dsettings, psettings, stdout);
	}
}
