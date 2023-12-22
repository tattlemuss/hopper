#ifndef HOPPER_56_INSTRUCTION_H
#define HOPPER_56_INSTRUCTION_H
#include <cstdint>

namespace hopper56
{
	enum Register
	{
		NONE,
		
		A,
		B,
		X,
		Y,

		A0, A1, A2,
		B0, B1, B2,
		X0, X1,
		Y0, Y1,

		R0, R1, R2, R3, R4, R5, R6, R7,
		N0, N1, N2, N3, N4, N5, N6, N7,
		M0, M1, M2, M3, M4, M5, M6, M7,
		REGISTER_COUNT
	};

	enum Memory
	{
		MEM_NONE = 0,
		MEM_X = 1,
		MEM_Y = 2,
		MEM_P = 3,
		MEM_COUNT
	};

	// Describes operands in all 3 columns
	struct operand
	{
		enum Type
		{
			NONE = 0,
			IMM_SHORT = 1,			// #xx
			REG = 2,				// any register
			POSTDEC_OFFSET = 3,		// (Rn)-Nn
			POSTINC_OFFSET = 4,		// (Rn)+Nn
			POSTDEC = 5,			// (Rn)+
			POSTINC = 6,			// (Rn)-
			INDEX_OFFSET = 7,		// (Rn+Nn)
			NO_UPDATE = 8,			// (Rn)
			PREDEC = 9,				// -(Rn)
			ABS = 10,				// absolute address
			IMM = 11,				// full-word immediate
		};
		Memory memory;				// X/Y/P/None
		Type type;

		// Data fields, dependent on Type above.
		union
		{
			struct
			{
				int8_t val;
			} imm_short;

			struct
			{
				uint32_t val;
			} imm;

			struct
			{
				Register index;			// any
			} reg, postdec, postinc, no_update, predec;

			struct
			{
				Register index_1;		// Rn
				Register index_2;		// Nn
			} postdec_offset, postinc_offset, index_offset;

			struct
			{
				uint32_t address;
			} abs;
		};

		void reset() 		{ memory = MEM_NONE; type = NONE; }
	};


	// parallel moves (columns 2 and 3)
	struct pmove
	{
		operand operands[2];
		void reset() 			{ operands[0].reset(); operands[1].reset(); }
	};

	struct instruction
	{
		enum Opcode
		{
			INVALID = 0,
			ABS,
			ADC,

			OPCODE_COUNT
		};
	
		instruction();
		void reset();

		uint32_t address;
		uint32_t header;
		int word_count;			// size in 56-bit words

		Opcode opcode;
		operand operands[3];
		pmove pmoves[2];
	};

	// ----------------------------------------------------------------------------
	// Helper functions to convert parts of the instruction to strings.
	extern const char* get_opcode_string(instruction::Opcode opcode);
	extern const char* get_register_string(Register reg);
	extern const char* get_memory_string(Memory mem);
}

#endif // HOPPER_56_INSTRUCTION_H

