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

#define REGNAME		hop56::get_register_string

// ----------------------------------------------------------------------------
// User options for output.
struct output_settings
{
	bool show_address;			// print address for each line before opcode
	bool show_timings;			// print (guessed) timings for each line (valid for 68000 only)
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
int print(const disassembly& disasm, const output_settings& osettings, FILE* pOutput)
{
	for (size_t i = 0; i < disasm.lines.size(); ++i)
	{
		const disassembly::line& line = disasm.lines[i];
		const hop56::instruction& inst = line.inst;
		if (osettings.show_address)
		{
			fprintf(pOutput, ">> %04x:   $%06x ", line.address, line.inst.header);
		}
		fprintf(pOutput, "\t");
		print(inst, line.address, pOutput);
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
		printf("\n>>> %06x\t", line.inst.header);

		// Handle failure
		disasm.lines.push_back(line);

		// DEBUG
		print(line.inst, line.address, stdout);
		printf("\n");

		buf.advance(line.inst.word_count);
	}
	return 0;
}

// ----------------------------------------------------------------------------
int process_bin_file(const uint8_t* data_ptr, long size, const hop56::decode_settings& dsettings,
		const output_settings& osettings, FILE* pOutput)
{
	hop56::buffer_reader buf(data_ptr, size, 0);
	//symbols bin_symbols;

	disassembly disasm;
	if (decode_buf(buf, dsettings, disasm))
		return 1;

	//add_reference_symbols(disasm, osettings, bin_symbols);

	print(disasm, osettings, pOutput);
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
	osettings.show_timings = false;
	osettings.label_prefix = "L";
	osettings.label_start_id = 0;

	hop56::decode_settings dsettings = {};
	//dsettings.cpu_type = hop56::CPU_TYPE_68000;
	const int last_arg = argc - 1;							// last arg is reserved for filename or hex data.

	for (int opt = 1; opt < last_arg; ++opt)
	{
		if (strcmp(argv[opt], "--address") == 0)
			osettings.show_address = true;
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
