#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <vector>
#include <string>

#include "lib/buffer.h"
#include "lib/decode.h"
#include "lib/instruction.h"
#include "print.h"
#include "symbols.h"

#define REGNAME		hop56::get_register_string

// ----------------------------------------------------------------------------
// User options for output.
struct output_settings
{
	bool show_address;			// print address for each line before opcode
	bool show_header;			// print hex header for each line before opcode
	bool abs_addressing;		// absolute addresses (don't create labels)
	std::string label_prefix;	// prefix for all auto-labels, normally "L"
	uint32_t label_start_id;	// starting number of label prefix, normally 0
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
		uint32_t address;
		hop56::instruction inst;
	};

	std::vector<line>    lines;
};

// ----------------------------------------------------------------------------
// Print a set of diassembled lines.
int print(const disassembly& disasm, const output_settings& osettings, const symbols& symbols, FILE* pOutput)
{
	for (size_t i = 0; i < disasm.lines.size(); ++i)
	{
		const disassembly::line& line = disasm.lines[i];
		const hop56::instruction& inst = line.inst;

		symbol sym;
		if (find_symbol(symbols, hop56::Memory::MEM_P, line.address, sym))
			fprintf(pOutput, "%s:\n", sym.label.c_str());

		if (osettings.show_address)
			fprintf(pOutput, "P:$%04x:  ", line.address);
		if (osettings.show_header)
			fprintf(pOutput, "[%06x] ", line.inst.header);

		fprintf(pOutput, "\t");
		print(inst, symbols, line.address, pOutput);
		fprintf(pOutput, "\n");
	}
	return 0;
}

// ----------------------------------------------------------------------------
// Read the buffer in a simple single pass.
int decode_buf(hop56::buffer_reader& buf, const hop56::decode_settings& dsettings, disassembly& disasm)
{
	while (buf.get_remain() >= 1)
	{
		disassembly::line line;
		line.address = buf.get_pos();

		// decode uses a copy of the buffer state
		hop56::buffer_reader buf_copy(buf);

		// We can ignore the return code, since it just says "this instruction is valid"
		// rather than "something catastrophic happened"
		hop56::decode(line.inst, buf_copy, dsettings);

		// Handle failure
		disasm.lines.push_back(line);

		buf.advance(line.inst.word_count);
	}
	return 0;
}

// Check if an operand jumps to another known address, and return that address
bool get_address(const hop56::operand& op, uint32_t inst_address, symbol::addr_t& target_address)
{
	if (op.type == hop56::operand::ABS)
	{
		// Ignore X: and Y: refs
		if (op.memory == hop56::MEM_P || op.memory == hop56::MEM_NONE)
		{
			// Force to P: memory
			target_address.mem = hop56::MEM_P;
			target_address.addr = op.abs.address;
			return true;
		}
	}
	return false;
}

// ----------------------------------------------------------------------------
// Find addresses referenced by disasm instructions and add them to the
// symbol table
void add_reference_symbols(const disassembly& disasm, const output_settings& settings, symbols& symbols)
{
	uint32_t label_id = settings.label_start_id;
	symbol::addr_t target_address;
	symbol sym;
	for (size_t i = 0; i < disasm.lines.size(); ++i)
	{
		const disassembly::line& line = disasm.lines[i];
		const hop56::instruction& inst = line.inst;
		for (size_t o = 0; o < 3; ++o)
		{
			const hop56::operand& op = inst.operands[o];
			if (get_address(op, line.address, target_address))
			{
				if (!find_symbol(symbols, hop56::Memory::MEM_P, target_address.addr, sym))
				{
					sym.label = settings.label_prefix + std::to_string(label_id);
					add_symbol(symbols, target_address.mem, target_address.addr, sym);
					++label_id;
				}
			}
		}

		// Now check pmoves
		for (size_t pm = 0; pm < 2; ++pm)
		{
			for (size_t o = 0; o < 3; ++o)
			{
				const hop56::operand& op = inst.pmoves[pm].operands[o];
				if (get_address(op, line.address, target_address))
				{
					if (!find_symbol(symbols, target_address.mem, target_address.addr, sym))
					{
						sym.label = settings.label_prefix + std::to_string(label_id);
						add_symbol(symbols, target_address.mem, target_address.addr, sym);
						++label_id;
					}
				}
			}
		}
	}
}

// ----------------------------------------------------------------------------
int process_bin_file(const uint8_t* data_ptr, long size, const hop56::decode_settings& dsettings,
		const output_settings& osettings, FILE* pOutput)
{
	hop56::buffer_reader buf(data_ptr, size, 0);
	symbols bin_symbols;

	disassembly disasm;
	if (decode_buf(buf, dsettings, disasm))
		return 1;

	if (!osettings.abs_addressing)
		add_reference_symbols(disasm, osettings, bin_symbols);

	print(disasm, osettings, bin_symbols, pOutput);
	return 0;
}

// ----------------------------------------------------------------------------
int main(int argc, char** argv)
{
	if (argc < 2)
	{
		fprintf(stderr, "Error: No filename or hexstring\n");
		//usage();
		return 1;
	}

	output_settings osettings = {};
	osettings.show_address = false;
	osettings.show_header = false;
	osettings.abs_addressing = false;
	osettings.label_prefix = "L";
	osettings.label_start_id = 0;

	hop56::decode_settings dsettings = {};
	const int last_arg = argc - 1;							// last arg is reserved for filename or hex data.

	for (int opt = 1; opt < last_arg; ++opt)
	{
		if (strcmp(argv[opt], "--address") == 0)
			osettings.show_address = true;
		if (strcmp(argv[opt], "--header") == 0)
			osettings.show_header = true;
		if (strcmp(argv[opt], "--abs") == 0)
			osettings.abs_addressing = true;
	}

	const char* fname = argv[argc - 1];
	FILE* pInfile = fopen(fname, "rb");
	if (!pInfile)
	{
		fprintf(stderr, "Error: Can't open file: %s\n", fname);
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
		free(data_ptr);
		fprintf(stderr, "Error: Failed to read file contents\n");
		return 1;
	}
	int ret = 0;
	ret = process_bin_file(data_ptr, size, dsettings, osettings, stdout);

	free(data_ptr);
	return ret;
}
