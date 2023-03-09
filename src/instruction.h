#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <cstdint>

// ----------------------------------------------------------------------------
//	INSTRUCTION STORAGE
// ----------------------------------------------------------------------------
enum Opcode
{
	NONE,
	ABCD,
	ADD,
	ADDA,
	ADDI,
	ADDQ,
	ADDX,
	AND,
	ANDI,
	ASL,
	ASR,
	BCC,
	BCHG,
	BCLR,
	BCS,
	BEQ,
	BGE,
	BGT,
	BHI,
	BKPT,
	BLE,
	BLS,
	BLT,
	BMI,
	BNE,
	BPL,
	BRA,
	BSET,
	BSR,
	BTST,
	BVC,
	BVS,
	CHK,
	CLR,
	CMP,
	CMPI,
	CMPA,
	CMPM,
	DBCC,
	DBCS,
	DBEQ,
	DBF,		// aka DBRA
	DBGE,
	DBGT,
	DBHI,
	DBLE,
	DBLS,
	DBLT,
	DBMI,
	DBNE,
	DBPL,
	DBVC,
	DBVS,
	DIVS,
	DIVU,
	EOR,
	EORI,
	EXG,
	EXT,
	ILLEGAL,
	JMP,
	JSR,
	LEA,
	LINK,
	LSL,
	LSR,
	MOVE,
	MOVEA,
	MOVEM,
	MOVEP,
	MOVEQ,
	MULS,
	MULU,
	NBCD,
	NEG,
	NEGX,
	NOP,
	NOT,
	OR,
	ORI,
	PEA,
	RESET,
	ROL,
	ROR,
	ROXL,
	ROXR,
	RTD,
	RTE,
	RTR,
	RTS,
	SBCD,
	SCC,
	SCS,
	SEQ,
	SF,
	SGE,
	SGT,
	SHI,
	SLE,
	SLS,
	SLT,
	SMI,
	SNE,
	SPL,
	ST,
	STOP,
	SUB,
	SUBA,
	SUBI,
	SUBQ,
	SUBX,
	SVC,
	SVS,
	SWAP,
	TAS,
	TRAP,
	TRAPV,
	TST,
	UNLK,

	COUNT	// Used for array sizes
};

enum class Size
{
	BYTE,
	WORD,
	LONG,
	NONE
};

// Operand types
enum OpType
{
	INVALID = 0,
	D_DIRECT,				// Dn		 000 reg. number:Dn
	A_DIRECT,				// An		 001 reg. number:An
	INDIRECT,				// (An)	   010 reg. number:An
	INDIRECT_POSTINC,		// (An) +	 011 reg. number:An
	INDIRECT_PREDEC,		// – (An)	 100 reg. number:An
	INDIRECT_DISP,			// (d16,An)   101 reg. number:An
	INDIRECT_INDEX,			// (d8,An,Xn) 110 reg. number:An
	ABSOLUTE_WORD,			// (xxx).W	111 000
	ABSOLUTE_LONG,			// (xxx).L	111 001
	PC_DISP,				// (d16,PC)   111 010
	PC_DISP_INDEX,			// (d8,PC,Xn) 111 011
	IMMEDIATE,				// <data>	 111 100
	MOVEM_REG,				// mask of registers
	RELATIVE_BRANCH,		// +/- 32K branch, doesn't display (pc)
	INDIRECT_PREINDEXED,	// (68020+)
	INDIRECT_POSTINDEXED,	// (68020+)
	MEMORY_INDIRECT,		// (68020+)
	NO_MEMORY_INDIRECT,		// (68020+)

	// Specific registers
	SR,
	USP,
	CCR,
};

// ----------------------------------------------------------------------------
enum IndexRegister
{
	INDEX_REG_D0,
	INDEX_REG_D1,
	INDEX_REG_D2,
	INDEX_REG_D3,
	INDEX_REG_D4,
	INDEX_REG_D5,
	INDEX_REG_D6,
	INDEX_REG_D7,
	INDEX_REG_A0,
	INDEX_REG_A1,
	INDEX_REG_A2,
	INDEX_REG_A3,
	INDEX_REG_A4,
	INDEX_REG_A5,
	INDEX_REG_A6,
	INDEX_REG_A7,
	INDEX_REG_PC,
	INDEX_REG_NONE
};

// ----------------------------------------------------------------------------
// Fields relating to address- or PC-index indirect (simple indexing)
// e.g. the "d2.w*4" in an opcode
struct index_indirect
{
	bool is_long;
	uint8_t scale_shift;		// 0 - scale 1, 1 = scale*2, 2 = scale*4, 3=scale*8
	IndexRegister index_reg;	// e.g. a0, d0, pc, none
};

// ----------------------------------------------------------------------------
struct indirect_index_full
{
	uint8_t					used[4];			// Denotes whether the below items are to be displayed
	int32_t					base_displacement;	// 0
	IndexRegister			base_register;		// 1
	index_indirect			index;				// 2
	int32_t					outer_displacement;	// 3
};

// ----------------------------------------------------------------------------
struct operand
{
	operand() :
		type(OpType::INVALID)
	{}

	// The overall type of operand
	OpType				type;

	// Data fields, dependent on OpType above.
	// This is a rather ugly union, but the simplest way to store the data
	// consistently without a lot of awkward subclassing.
	union
	{
		struct
		{
			Size		size;		// byte, word or long
			uint32_t	val0;
			bool		is_signed;
		} imm;

		struct
		{
			uint8_t	reg;			// e.g. D3
		} d_register;

		struct
		{
			uint8_t reg;
		} a_register;

		struct
		{
			uint8_t reg;
		} indirect;

		struct
		{
			uint8_t reg;
		} indirect_postinc;

		struct
		{
			uint8_t reg;
		} indirect_predec;

		struct
		{
			int16_t disp;
			uint8_t reg;						// Address register only
		} indirect_disp;

		struct
		{
			int8_t disp;						// displacement [-128...127)
			uint8_t a_reg;						// base register
			index_indirect indirect_info;
		} indirect_index;

		struct
		{
			uint32_t wordaddr;
		} absolute_word;

		struct
		{
			uint32_t longaddr;
		} absolute_long;

		struct
		{
			// Base register is implicitly "PC"
			int32_t inst_disp;					// offset from the base instruction address. Can be $7ffe+6 bytes max.
		} pc_disp;

		struct
		{
			// Base register is implicitly "PC"
			int32_t inst_disp;					// offset from the base instruction address. Can be $7ffe+6 bytes max.
			index_indirect indirect_info;
		} pc_disp_index;

		struct
		{
			uint16_t reg_mask;
		} movem_reg;

		struct
		{
			int32_t inst_disp;		// offset from the base instruction address. Can be $7ffe+2 bytes max.
		} relative_branch;

		indirect_index_full		indirect_index_68020;
	};
};

enum class Suffix
{
	BYTE,
	WORD,
	LONG,
	SHORT,
	NONE
};

// ----------------------------------------------------------------------------
// A decoded instruction split into its constituent parts.
struct instruction
{
	instruction() :
		header(0U),
		byte_count(0U),
		opcode(Opcode::NONE),
		suffix(Suffix::BYTE)
	{
	}

	uint32_t	address;
	uint16_t	header;			// first 16-bit word of data
	uint16_t	byte_count;
	Opcode		opcode;
	Suffix		suffix;
	operand		op0;
	operand		op1;
};

#endif
