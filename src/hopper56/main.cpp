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


#define REGNAME		hopper56::get_register_string

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
		hopper56::instruction inst;
	};

	std::vector<line>    lines;
};

// ----------------------------------------------------------------------------
// Read the buffer in a simple single pass.
int decode_buf(hopper56::buffer_reader& buf, const hopper56::decode_settings& dsettings, disassembly& disasm)
{
	while (buf.get_remain() >= 1)
	{
		disassembly::line line;
		line.address = buf.get_pos();

		// decode uses a copy of the buffer state
		hopper56::buffer_reader buf_copy(buf);

		// We can ignore the return code, since it just says "this instruction is valid"
		// rather than "something catastrophic happened"
		hopper56::decode(line.inst, buf_copy, dsettings);

		// Handle failure
		disasm.lines.push_back(line);

		buf.advance(line.inst.word_count);
	}
	return 0;
}

// Print an operand, for all operand types
void print(const hopper56::operand& operand, FILE* pOutput)
{
	fprintf(pOutput, "%s", hopper56::get_memory_string(operand.memory));
	switch (operand.type) 
	{
		case hopper56::operand::IMM_SHORT:
			fprintf(pOutput, "#%d", operand.imm_short.val);
			break;
		case hopper56::operand::REG:
			fprintf(pOutput, "%s",
					REGNAME(operand.reg.index));
			break;
		case hopper56::operand::POSTDEC_OFFSET:
			fprintf(pOutput, "(%s)-%s",
					REGNAME(operand.postdec_offset.index_1),
					REGNAME(operand.postdec_offset.index_2));
			break;
		case hopper56::operand::POSTINC_OFFSET:
			fprintf(pOutput, "(%s)+%s",
					REGNAME(operand.postinc_offset.index_1),
					REGNAME(operand.postinc_offset.index_2));
			break;
		case hopper56::operand::POSTDEC:
			fprintf(pOutput, "(%s)-",
					REGNAME(operand.postdec.index));
			break;
		case hopper56::operand::POSTINC:
			fprintf(pOutput, "(%s)+",
					REGNAME(operand.postinc.index));
			break;
		case hopper56::operand::NO_UPDATE:
			fprintf(pOutput, "(%s)",
					REGNAME(operand.no_update.index));
			break;
		case hopper56::operand::INDEX_OFFSET:
			fprintf(pOutput, "(%s+%s)",
					REGNAME(operand.index_offset.index_1),
					REGNAME(operand.index_offset.index_2));
			break;
		case hopper56::operand::PREDEC:
			fprintf(pOutput, "-(%s)", REGNAME(operand.predec.index));
			break;
		case hopper56::operand::ABS:
			fprintf(pOutput, "$%X", operand.abs.address);
			break;
		default:
			fprintf(pOutput, "%d?", operand.type);
			break;
		case hopper56::operand::IMM:
			fprintf(pOutput, "#$%X", operand.imm.val);
			break;
	}
}
		
int print(const hopper56::instruction& inst, uint32_t address, FILE* pOutput)
{
	fprintf(pOutput, "%s\t", hopper56::get_opcode_string(inst.opcode));

	for (int i = 0; i < 3; ++i)
	{
		const hopper56::operand& op = inst.operands[i];
		if (op.type == hopper56::operand::NONE)
			break;
	
		if (i > 0)
			fprintf(pOutput, ",");
		
		switch (op.type) 
		{
			case hopper56::operand::REG:
				fprintf(pOutput, "%s", REGNAME(op.reg.index));
				break;
			default:
				fprintf(pOutput, "%d", op.type);
				break;
		}
	}

	for (int i = 0; i < 2; ++i)
	{
		const hopper56::pmove& pmove = inst.pmoves[i];
		if (pmove.operands[0].type == hopper56::operand::NONE)
			continue;	// skip if there is no first operand
	
		fprintf(pOutput, "\t");
		print(pmove.operands[0], pOutput);

		if (pmove.operands[1].type == hopper56::operand::NONE)
			continue;	// next pmove
		fprintf(pOutput, ",");
		print(pmove.operands[1], pOutput);
	}
	return 0;
}

// ----------------------------------------------------------------------------
// Print a set of diassembled lines.
int print(const disassembly& disasm, const output_settings& osettings, FILE* pOutput)
{
	for (size_t i = 0; i < disasm.lines.size(); ++i)
	{
		const disassembly::line& line = disasm.lines[i];
		const hopper56::instruction& inst = line.inst;
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
int process_bin_file(const uint8_t* data_ptr, long size, const hopper56::decode_settings& dsettings, 
		const output_settings& osettings, FILE* pOutput)
{
	hopper56::buffer_reader buf(data_ptr, size, 0);
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

	hopper56::decode_settings dsettings = {};
	//dsettings.cpu_type = hopper56::CPU_TYPE_68000;
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
		fprintf(stderr, "Error: Failed to read file contents\n");
		return 1;
	}
	int ret = 0;
	ret = process_bin_file(data_ptr, size, dsettings, osettings, stdout);

	free(data_ptr);
	return ret;
}
