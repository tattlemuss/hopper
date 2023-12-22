#include "instruction.h"

namespace hopper56
{

	const char* g_opcode_names[instruction::Opcode::OPCODE_COUNT] =
	{
		"none",
		"ABS",
		"ADC",
	};

	// ----------------------------------------------------------------------------
	const char* g_register_names[Register::REGISTER_COUNT] =
	{
		"?",

		"A",
		"B",
		"X",
		"Y",

		"A0", "A1", "A2",
		"B0", "B1", "B2",
		"X0", "X1",
		"Y0", "Y1",

		"R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7",
		"N0", "N1", "N2", "N3", "N4", "N5", "N6", "N7",
		"M0", "M1", "M2", "M3", "M4", "M5", "M6", "M7",
	};
	// ----------------------------------------------------------------------------
	const char* g_memory_names[Memory::MEM_COUNT] =
	{
		"", "X:", "Y:", "P:"	
	};

#define ARRAY_SIZE(a)		 (sizeof(a) / sizeof(a[0]))
	// ----------------------------------------------------------------------------
	const char* get_opcode_string(instruction::Opcode opcode)
	{
		if (opcode < ARRAY_SIZE(g_opcode_names))
			return g_opcode_names[opcode];
		return "?";
	}

	// ----------------------------------------------------------------------------
	const char* get_register_string(Register reg)
	{
		if (reg < ARRAY_SIZE(g_register_names))
			return g_register_names[reg];
		return "?";
	}

	// ----------------------------------------------------------------------------
	const char* get_memory_string(Memory mem)
	{
		if (mem < ARRAY_SIZE(g_memory_names))
			return g_memory_names[mem];
		return "?";
	}

	// ----------------------------------------------------------------------------
	instruction::instruction() :
		header(0U),
		word_count(0U),
		opcode(Opcode::INVALID)
	{
		reset();
	}

	// ----------------------------------------------------------------------------
	void instruction::reset()
	{
		// This effectively sets the instruction to a "dc.w" statement
		header = 0U;
		word_count = 1U;
		opcode = Opcode::INVALID;
		operands[0].reset();
		operands[1].reset();
		operands[2].reset();

		pmoves[0].reset();
	   	pmoves[1].reset();
	}
}

