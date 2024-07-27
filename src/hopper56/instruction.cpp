#include "instruction.h"

namespace hop56
{

	const char* g_opcode_names[instruction::Opcode::OPCODE_COUNT] =
	{
		"DC",
		"ABS",
		"ADC",
		"ADD",
		"ADDL",
		"ADDR",
		"AND",
		"ANDI",
		"ASL",
		"ASL4",
		"ASR",
		"ASR16",
		"ASR4",
		"BCHG",
		"BCLR",
		"BSET",
		"BTSTH",
		"BTSTL",
		"BRA",
		"BRKcc",
		"BSR",
		"BScc",
		"Bcc",
		"CLR",
		"CLR24",
		"CMP",
		"CMPM",
		"DEBUG",
		"DEBUGcc",
		"DEC",
		"DEC24",
		"DIV",
		"DMAC",
		"DO",
		"DOLoop",
		"ENDDO",
		"EOR",
		"EXT",
		"FOREVER",
		"IMAC",
		"IMPY",
		"INC",
		"INC24",
		"JMP",
		"JSR",
		"JScc",
		"Jcc",
		"LEA",
		"LSL",
		"LSR",
		"MAC",
		"MACR",
		"MOVE",
		"MOVEC",
		"MOVEI",
		"MOVEM",
		"MOVEP",
		"MOVES",
		"MPY",
		"MPYR",
		"NEG",
		"NEGC",
		"NOP",
		"NORM",
		"NOT",
		"OR",
		"ORI",
		"REP",
		"REPc",
		"RESET",
		"RND",
		"ROL",
		"ROR",
		"RTI",
		"RTS",
		"SBC",
		"STOP",
		"SUB",
		"SUBL",
		"SUBR",
		"SWAP",
		"SWI",
		"TFR",
		"TFR2",
		"TST",
		"TST2",
		"Tcc",
		"WAIT",
		"ZERO",
	};

	// ----------------------------------------------------------------------------
	const char* g_register_names[Reg::REG_COUNT] =
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

		// Register pairs/parts for MOVEL types
		"A10", "B10", "AB", "BA",

		"MR", "CCR", "OMR",

	};

	// ----------------------------------------------------------------------------
	const char* g_memory_names[Memory::MEM_COUNT] =
	{
		"", "X:", "Y:", "P:", "L:"
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
	const char* get_register_string(Reg reg)
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
		neg_operands = 0;
		operands[0].reset();
		operands[1].reset();
		operands[2].reset();

		pmoves[0].reset();
	   	pmoves[1].reset();
	}
}

