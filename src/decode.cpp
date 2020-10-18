#include <assert.h>
#include <cstddef>
#include "decode.h"

#include "instruction.h"
#include "buffer.h"

// ----------------------------------------------------------------------------
//	INSTRUCTION DECODE
// ----------------------------------------------------------------------------
// Set functions to correctly handle operand union data
void set_imm_byte(operand& op, uint8_t val)
{
	op.type = OpType::IMMEDIATE;
	op.imm.size = Size::BYTE;
	op.imm.val0 = val;
}

void set_imm_word(operand& op, uint16_t val)
{
	op.type = OpType::IMMEDIATE;
	op.imm.size = Size::WORD;
	op.imm.val0 = val;
}

void set_imm_long(operand& op, uint32_t val)
{
	op.type = OpType::IMMEDIATE;
	op.imm.size = Size::LONG;
	op.imm.val0 = val;
}

void set_dreg(operand& op, uint8_t reg)
{
	op.type = OpType::D_DIRECT;
	op.d_register.reg = reg;
}

void set_areg(operand& op, uint8_t reg)
{
	op.type = OpType::A_DIRECT;
	op.a_register.reg = reg;
}

void set_predec(operand& op, uint8_t reg)
{
	op.type = OpType::INDIRECT_PREDEC;
	op.indirect_predec.reg = reg;
}

void set_postinc(operand& op, uint8_t reg)
{
	op.type = OpType::INDIRECT_POSTINC;
	op.indirect_postinc.reg = reg;
}

// Effective Address encoding modes.
// Different instructions have different sets of EAs which are allowable to be used
// for each operand. This ID flags which ones are allowed.
enum ea_group
{
	DATA_ALT		= 0,	// Data alterable (no A-reg, no PC-rel, no immedidate)
	DATA			= 1,	// Data, non-alterable (source)
	MEM_ALT			= 2,	// Memory, alterable (no A-reg, no PC-rel, no immedidate)
	MEM				= 3,	// Memory, non-alterable (source)
	CONTROL			= 4,
	CONTROL_MOVEM1	= 5,	// Movem to memory
	CONTROL_MOVEM2	= 6,	// Movem to reg
	ALT				= 7,	// Alterable including A-reg
	ALL				= 8,	// e.g. cmp
	COUNT
};

// Defines which instruction modes are allowed to have which EA modes
bool mode_availability[][ea_group::COUNT] =
{
	// DataAlt	Data	MemAlt	Mem		Ctrl	CMovem	CMovem2	Alt		All
	{	true,	true,	false,	false,	false,	false,	false,	true,	true	}, // D_DIRECT			000 regno
	{	false,	false,	false,	false,	false,	false,	false,	true,	true	}, // A_DIRECT			001 regno
	{	true,	true,	true,	true,	true,	true,	true,	true,	true	}, // INDIRECT			010 regno
	{	true,	true,	true,	true,	false,	false,	true,	true,	true	}, // INDIRECT_POSTINC	011 regno
	{	true,	true,	true,	true,	false,	true,	true,	true,	true	}, // INDIRECT_PREDEC	 100 regno
	{	true,	true,	true,	true,	true,	true,	true,	true,	true	}, // INDIRECT_DISP	   101 regno
	{	true,	true,	true,	true,	true,	true,	true,	true,	true	}, // INDIRECT_INDEX,	 110 regno
	{	true,	true,	true,	true,	true,	true,	true,	true,	true	}, // ABSOLUTE_WORD	   111 000
	{	true,	true,	true,	true,	true,	true,	true,	true,	true	}, // ABSOLUTE_LONG	   111 001  // There is a typo in the doc here
	{	false,  true,	false,	true,	true,	true,	true,	false,	true	}, // PC_DISP			 111 010
	{	false,  true,	false,	true,	true,	true,	true,	false,	true	}, // PC_DISP_INDEX	   111 011
	{	false,  true,	false,	true,	false,	false,	false,	false,	true	}, // IMMEDIATE		   111 100
	{	false,  false,	false,	false,	false,	false,	false,	false,	false	}, // INVALID		   111 100
};

// There is a consistent approach to decoding the mode bits (0-6),
// and when the mode is 7, using the register bits
// Convert these mixed bits to a range [0-12)
uint8_t decode_operand_type(uint8_t mode_bits, uint8_t reg_bits)
{
	if (mode_bits < 7)
		return mode_bits;

	if (reg_bits <= 4)
		return  7 + reg_bits;

	// Invalid!
	return 7 + 5;
}

// ----------------------------------------------------------------------------
// Split 16 bit raw displacement into signed 8-bit offset, register index and size
// e.g n(pc,d0.w) or n(a0,d0.w)
void decode_brief_extension_word(uint16_t word, int8_t& disp, uint8_t& data_reg, bool& is_long)
{
	// The offset is 8-bits
	disp = (int8_t)(word & 0xff);
	data_reg = (word >> 12) & 7;
	is_long = ((word >> 11) & 1);
}

int read_immediate(buffer_reader& buffer, operand& operand, Size size)
{
	uint16_t val16;
	uint32_t val32;
	operand.type = OpType::IMMEDIATE;
	switch (size)
	{
		case Size::BYTE:
			// byte
			if (buffer.read_word(val16))
				return 1;
			operand.imm.size = Size::BYTE;
			operand.imm.val0 = val16 & 0xff;
			break;
		case Size::WORD:
			// word
			if (buffer.read_word(val16))
				return 1;
			operand.imm.size = Size::WORD;
			operand.imm.val0 = val16;
			break;
		case Size::LONG:
			// long
			if (buffer.read_long(val32))
				return 1;
			operand.imm.size = Size::LONG;
			operand.imm.val0 = val32;
			break;
		default:
			return 1;	// error!
	}
	return 0;
}

// ----------------------------------------------------------------------------
int decode_ea(buffer_reader& buffer, operand& operand, ea_group group, uint8_t mode_bits, uint8_t reg_bits, Size size)
{
	// Convert to an operand type
	int ea_type = decode_operand_type(mode_bits, reg_bits);

	// Check EA type is valid for this instruction
	bool allow = mode_availability[ea_type][group];
	if (!allow)
		return 1;

	static const OpType types[] =
	{
		// 0-6 in "mode" bits
		//                                mode
		D_DIRECT,			// Dn		  000 reg. number:Dn
		A_DIRECT,			// An		  001 reg. number:An
		INDIRECT,			// (An)	      010 reg. number:An
		INDIRECT_POSTINC,	// (An) +	  011 reg. number:An
		INDIRECT_PREDEC,	// – (An)	  100 reg. number:An
		INDIRECT_DISP,	    // (d16,An)   101 reg. number:An
		INDIRECT_INDEX,	    // (d8,An,Xn) 110 reg. number:An

		// 7 in "mode" bits, 0-4 in reg bits
		//                                  mode reg
		ABSOLUTE_WORD,      // (xxx).W      111 000
		ABSOLUTE_LONG,      // (xxx).L      111 001
		PC_DISP,		    // (d16,PC)     111 010
		PC_DISP_INDEX,      // (d8,PC,Xn)   111 011
		IMMEDIATE,          // <data>       111 100
		INVALID,
	};
	assert(ea_type <= 7 + 4 + 1);
	operand.type = types[ea_type];

	uint16_t val16;
	uint32_t val32;
	switch (operand.type)
	{
		case OpType::D_DIRECT:
			operand.d_register.reg = reg_bits;
			return 0;

		case OpType::A_DIRECT:
			// Special case: address register direct never allowed with byte
			// operations.
			if (size == Size::BYTE)
				return 1;
			operand.a_register.reg = reg_bits;
			return 0;

		case OpType::INDIRECT:
			operand.indirect.reg = reg_bits;
			return 0;

		case OpType::INDIRECT_POSTINC:
			operand.indirect_postinc.reg = reg_bits;
			return 0;

		case OpType::INDIRECT_PREDEC:
			operand.indirect_predec.reg = reg_bits;
			return 0;

		case OpType::INDIRECT_DISP:
			operand.indirect_disp.reg = reg_bits;
			if (buffer.read_word(val16))
				return 1;
			// 16-bit displacement
			operand.indirect_disp.disp = (int16_t)val16;
			return 0;

		case OpType::INDIRECT_INDEX:
			operand.indirect_index.a_reg = reg_bits;
			if (buffer.read_word(val16))
				return 1;
			decode_brief_extension_word(val16,
				operand.indirect_index.disp,
				operand.indirect_index.d_reg,
				operand.indirect_index.is_long);
			return 0;

		case OpType::ABSOLUTE_WORD:
			if (buffer.read_word(val16))
				return 1;
			operand.absolute_word.wordaddr = val16;
			return 0;

		case OpType::ABSOLUTE_LONG:
			if (buffer.read_long(val32))
				return 1;
			operand.absolute_long.longaddr = val32;
			return 0;

		case OpType::PC_DISP:
			if (buffer.read_word(val16))
				return 1;
			// 16-bit displacement
			operand.pc_disp.disp = (int16_t)val16;
			return 0;

		case OpType::PC_DISP_INDEX:
			if (buffer.read_word(val16))
				return 1;
			decode_brief_extension_word(val16,
				operand.pc_disp_index.disp,
				operand.pc_disp_index.d_reg,
				operand.pc_disp_index.is_long);
			return 0;

		case OpType::IMMEDIATE:
			return read_immediate(buffer, operand, size);

		default:
			break;
	}
	return 1;
}

// ----------------------------------------------------------------------------
// Mapping of 2-bits for size encodings in most instruction types that allow
// all 3 sizes.
Size standard_size_table[] =
{
	Size::BYTE,
	Size::WORD,
	Size::LONG,
	Size::NONE		// Either invalid, or used for another instruction type
};

// ----------------------------------------------------------------------------
Suffix size_to_suffix(Size size)
{
	switch (size)
	{
		case Size::BYTE: return Suffix::BYTE;
		case Size::WORD: return Suffix::WORD;
		case Size::LONG: return Suffix::LONG;
		default: break;
	}
	return Suffix::NONE;
}

// ----------------------------------------------------------------------------
// Integer ALU instruction, #dds,<ea>
int Inst_integer_imm_ea(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	// op0: immediate value
	uint8_t size_bits = (header >> 6) & 3;
	Size sizes[] = { Size::BYTE, Size::WORD, Size::LONG, Size::NONE };
	Size ea_size = sizes[size_bits];
	inst.suffix = size_to_suffix(ea_size);
	if (ea_size == Size::NONE)
		return 1;

	if (read_immediate(buffer, inst.op0, ea_size))
		return 1;

	// op1: EA
	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;
	return decode_ea(buffer, inst.op1, DATA_ALT, mode, reg, ea_size);
}

// ----------------------------------------------------------------------------
// Single EA operand
int Inst_size_ea(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t size = (header >> 6) & 3;
	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;

	Size sizes[] = { Size::BYTE, Size::WORD, Size::LONG, Size::NONE };
	Size ea_size = sizes[size];
	inst.suffix = size_to_suffix(ea_size);
	if (ea_size == Size::NONE)
		return 1;

	return decode_ea(buffer, inst.op0, DATA_ALT, mode, reg, ea_size);
}

// ----------------------------------------------------------------------------
int Inst_lea(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t size = (header >> 6) & 3;
	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;
	uint8_t dest_reg  = (header >> 9) & 7;

	// Target register
	inst.op1.type = OpType::A_DIRECT;
	inst.op1.a_register.reg = dest_reg;

	return decode_ea(buffer, inst.op0, CONTROL, mode, reg, Size::LONG);
}

// ----------------------------------------------------------------------------
int Inst_movem_reg_mem(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;
	Size ea_size = (header >> 6) & 1 ? Size::LONG : Size::WORD;
	inst.suffix = (header >> 6) & 1 ? Suffix::LONG : Suffix::WORD;

	uint16_t reg_mask;
	if (buffer.read_word(reg_mask))
		return 1;

	// src register
	inst.op0.type = OpType::MOVEM_REG;
	inst.op0.movem_reg.reg_mask = reg_mask;
	if (decode_ea(buffer, inst.op1, CONTROL_MOVEM1, mode, reg, ea_size))
		return 1;

	// Special case: if we are -(a0) mode, order of registers is reversed
	if (inst.op1.type == OpType::INDIRECT_PREDEC)
	{
		uint16_t rev_mask = 0;
		for (int i = 0; i < 16; ++i)
		{
			rev_mask <<= 1;
			if (reg_mask & (1 << i))
				rev_mask |= 1;
		}
		inst.op0.movem_reg.reg_mask = rev_mask;
	}
	return 0;
}

// ----------------------------------------------------------------------------
int Inst_movem_mem_reg(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;
	Size ea_size = (header >> 6) & 1 ? Size::LONG : Size::WORD;
	inst.suffix = (header >> 6) & 1 ? Suffix::LONG : Suffix::WORD;
	uint16_t reg_mask;
	if (buffer.read_word(reg_mask))
		return 1;

	// dst register
	inst.op1.type = OpType::MOVEM_REG;
	inst.op1.movem_reg.reg_mask = reg_mask;
	return decode_ea(buffer, inst.op0, CONTROL_MOVEM2, mode, reg, ea_size);
}

// ----------------------------------------------------------------------------
int Inst_move(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t sizebits = (header >> 12) & 3;
	uint8_t src_reg  = (header >> 0) & 7;
	uint8_t src_mode = (header >> 3) & 7;
	uint8_t dst_reg  = (header >> 9) & 7;
	uint8_t dst_mode = (header >> 6) & 7;

	// NOTE: this is non-standard
	Size sizes[] = { Size::NONE, Size::BYTE, Size::LONG, Size::WORD };
	Size ea_size = sizes[sizebits];
	if (ea_size == Size::NONE)
		return 1;
	inst.suffix = size_to_suffix(ea_size);

	// Src data comes first in the encoding
	int ret = decode_ea(buffer, inst.op0, ea_group::ALL, src_mode, src_reg, ea_size);
	if (ret != 0)
		return ret;
	ret = decode_ea(buffer, inst.op1, ea_group::DATA_ALT, dst_mode, dst_reg, ea_size);
	return ret;
}

// ----------------------------------------------------------------------------
int Inst_movea(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t size = (header >> 12) & 3;
	uint8_t areg = (header >> 9) & 7;
	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;

	Size sizes[] = { Size::NONE, Size::NONE, Size::LONG, Size::WORD };
	Size ea_size = sizes[size];
	if (ea_size == Size::NONE)
		return 1;
	inst.suffix = size_to_suffix(ea_size);

	if (decode_ea(buffer, inst.op0, ALL, mode, reg, ea_size))
		return 1;

	inst.op1.type = OpType::A_DIRECT;
	inst.op1.a_register.reg = areg;
	return 0;
}

// ----------------------------------------------------------------------------
int Inst_moveq(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	inst.suffix = Suffix::LONG;

	set_imm_byte(inst.op0, header & 0xff);
	uint8_t reg = (header >> 9) & 7;
	set_dreg(inst.op1, reg);
	return 0;
}

// ----------------------------------------------------------------------------
int Inst_subq(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t size = (header >> 6) & 3;
	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;

	uint8_t data = (header >> 9) & 7;
	if (data == 0)
		data = 8;
	set_imm_byte(inst.op0, data);

	Size sizes[] = { Size::BYTE, Size::WORD, Size::LONG, Size::NONE };
	Size ea_size = sizes[size];
	if (ea_size == Size::NONE)
		return 1;

	inst.suffix = size_to_suffix(ea_size);
	return decode_ea(buffer, inst.op1, ALT, mode, reg, ea_size);
}

// ----------------------------------------------------------------------------
int Inst_trap(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	set_imm_byte(inst.op0, header & 0xf);
	return 0;
}

// ----------------------------------------------------------------------------
int Inst_move_from_sr(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	inst.suffix = Suffix::WORD;
	inst.op0.type = OpType::SR;

	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;
	return decode_ea(buffer, inst.op1, DATA, mode, reg, Size::WORD);
}

// ----------------------------------------------------------------------------
int Inst_move_to_sr(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	inst.suffix = Suffix::WORD;
	inst.op1.type = OpType::SR;

	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;
	return decode_ea(buffer, inst.op0, DATA, mode, reg, Size::WORD);
}

// ----------------------------------------------------------------------------
// bchg, bset, bclr, btst
int Inst_bchg_imm(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;

	// Read the immediate data
	uint16_t imm;
	if (buffer.read_word(imm))
		return 1;
	if (imm & 0xff00)			   // top bits must be 0
		return 1;

	set_imm_byte(inst.op0, (imm >> 0) & 0xff);
	if (decode_ea(buffer, inst.op1, DATA_ALT, mode, reg, Size::NONE))
		return 1;

	// instruction size is LONG when using DREG.
	inst.suffix = inst.op1.type == OpType::D_DIRECT ? Suffix::LONG : Suffix::BYTE;
	return 0;
}

// ----------------------------------------------------------------------------
// bchg, bset, bclr, btst
int Inst_bchg(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t bitreg = (header >> 9) & 7;
	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;

	set_dreg(inst.op0, bitreg);
	return decode_ea(buffer, inst.op1, DATA_ALT, mode, reg, Size::BYTE);
}

// ----------------------------------------------------------------------------
int Inst_alu_dreg(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t dreg = (header >> 9) & 7;
	uint8_t ea_dst = (header >> 8) & 1;
	uint8_t size   = (header >> 6) & 3;
	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;

	Size sizes[] = { Size::BYTE, Size::WORD, Size::LONG, Size::NONE };
	Size ea_size = sizes[size];
	if (ea_size == Size::NONE)
		return 1;
	inst.suffix = size_to_suffix(ea_size);

	if (ea_dst)
	{
		// Src is d-reg
		inst.op0.type = OpType::D_DIRECT;
		inst.op0.d_register.reg = dreg;
		return decode_ea(buffer, inst.op1, MEM_ALT, mode, reg, ea_size);
	}
	else
	{
		// Dest is d-reg
		inst.op1.type = OpType::D_DIRECT;
		inst.op1.d_register.reg = dreg;
		return decode_ea(buffer, inst.op0, ALL, mode, reg, ea_size);
	}
}

// ----------------------------------------------------------------------------
int Inst_addsuba(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t areg = (header >> 9) & 7;
	uint8_t size = (header >> 8) & 1;
	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;

	Size sizes[] = { Size::WORD, Size::LONG };
	Size ea_size = sizes[size];
	if (ea_size == Size::NONE)
		return 1;
	inst.suffix = size_to_suffix(ea_size);

	inst.op1.type = OpType::A_DIRECT;
	inst.op1.a_register.reg = areg;
	return decode_ea(buffer, inst.op0, ALL, mode, reg, ea_size);
}

// ----------------------------------------------------------------------------
int Inst_shift_reg(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t shift	= (header >> 9) & 7;
	uint8_t size	 = (header >> 6) & 3;
	uint8_t countreg = (header >> 5) & 1;
	uint8_t dreg	 = (header >> 0) & 7;

	Size ea_size = standard_size_table[size];
	if (ea_size == Size::NONE)
		return 1;
	inst.suffix = size_to_suffix(ea_size);
	if (countreg == 0)
	{
		// Shift by immediate
		if (shift == 0)
			shift = 8;

		set_imm_byte(inst.op0, shift);
	}
	else
	{
		// Shift by register
		inst.op0.type = OpType::D_DIRECT;
		inst.op0.d_register.reg = shift;
	}

	inst.op1.type = OpType::D_DIRECT;
	inst.op1.d_register.reg = dreg;
	return 0;
}

// ----------------------------------------------------------------------------
int Inst_stop(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint16_t val;
	if (buffer.read_word(val))
		return 1;
	set_imm_word(inst.op0, val);
	return 0;
}

// ----------------------------------------------------------------------------
// Instructions without any operands
int Inst_simple(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	return 0;
}

// ----------------------------------------------------------------------------
int Inst_swap(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t reg = (header >> 0) & 7;
	set_dreg(inst.op0, reg);
	return 0;
}

// ----------------------------------------------------------------------------
int Inst_link_w(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t reg = (header >> 0) & 7;
	inst.suffix = Suffix::WORD;
	set_areg(inst.op0, reg);

	uint16_t disp;
	if (buffer.read_word(disp))
		return 1;

	// NO CHECK unsigned
	set_imm_word(inst.op1, disp);
	return 0;
}

// ----------------------------------------------------------------------------
int Inst_unlk(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t reg = (header >> 0) & 7;
	set_areg(inst.op0, reg);
	return 0;
}

// ----------------------------------------------------------------------------
int Inst_move_from_usp(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t reg = (header >> 0) & 7;
	inst.op0.type = OpType::USP;
	set_areg(inst.op1, reg);
	return 0;
}

// ----------------------------------------------------------------------------
int Inst_move_to_usp(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t reg = (header >> 0) & 7;
	set_areg(inst.op0, reg);
	inst.op1.type = OpType::USP;
	return 0;
}

// ----------------------------------------------------------------------------
int Inst_move_to_ccr(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;
	inst.op1.type = OpType::CCR;
	return decode_ea(buffer, inst.op0, DATA, mode, reg, Size::WORD);
}

// ----------------------------------------------------------------------------
int Inst_nbcd(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	inst.suffix = Suffix::BYTE;
	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;
	return decode_ea(buffer, inst.op0, DATA_ALT, mode, reg, Size::BYTE);
}

// ----------------------------------------------------------------------------
int Inst_pea(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;
	return decode_ea(buffer, inst.op0, CONTROL, mode, reg, Size::LONG);
}

// ----------------------------------------------------------------------------
// NOTE: identical to nbcd
int Inst_tas(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;
	return decode_ea(buffer, inst.op0, DATA_ALT, mode, reg, Size::BYTE);
}

// ----------------------------------------------------------------------------
// jmp/jsr
int Inst_jump(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;
	return decode_ea(buffer, inst.op0, CONTROL, mode, reg, Size::NONE);
}

// ----------------------------------------------------------------------------
int Inst_asl_asr_mem(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t size = (header >> 6) & 3;
	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;
	// "An operand in memory can be shifted one bit only, and the operand size is restricted to a word."
	inst.suffix = Suffix::WORD;
	return decode_ea(buffer, inst.op0, MEM_ALT, mode, reg, Size::WORD);
}

// ----------------------------------------------------------------------------
int Inst_branch(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	int8_t disp8 = (int8_t)(header & 0xff);

	inst.op0.type = RELATIVE_BRANCH;
	if (disp8 == 0)
	{
		// 16-bit offset
		uint16_t disp16;
		if (buffer.read_word(disp16))
			return 1;
		inst.op0.relative_branch.disp = (int16_t)disp16;
		inst.suffix = Suffix::WORD;
	}
	else
	{
		inst.op0.relative_branch.disp = disp8;
		inst.suffix = Suffix::SHORT;
	}
	return 0;
}

// ----------------------------------------------------------------------------
int Inst_ext(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	// NOTE: this needs to be changed if handling ext byte->long
	uint8_t mode = (header >> 6) & 1;
	inst.suffix = (mode == 0) ? Suffix::WORD : Suffix::LONG;
	uint8_t reg = (header >> 0) & 7;
	set_dreg(inst.op0, reg);
	return 0;
}

// ----------------------------------------------------------------------------
int Inst_movep_mem_reg(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t mode = (header >> 6) & 1;
	inst.suffix = (mode == 0) ? Suffix::WORD : Suffix::LONG;

	uint8_t dreg = (header >> 9) & 7;
	uint8_t areg = (header >> 0) & 7;
	uint16_t val16;
	if (buffer.read_word(val16))
		return 1;

	inst.op0.type = OpType::INDIRECT_DISP;
	inst.op0.indirect_disp.reg = areg;
	inst.op0.indirect_disp.disp = (int16_t)val16;
	set_dreg(inst.op1, dreg);

	return 0;
}

// ----------------------------------------------------------------------------
int Inst_movep_reg_mem(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t mode = (header >> 6) & 1;
	inst.suffix = (mode == 0) ? Suffix::WORD : Suffix::LONG;

	uint8_t dreg = (header >> 9) & 7;
	uint8_t areg = (header >> 0) & 7;
	uint16_t val16;
	if (buffer.read_word(val16))
		return 1;

	set_dreg(inst.op0, dreg);
	inst.op1.type = OpType::INDIRECT_DISP;
	inst.op1.indirect_disp.reg = areg;
	inst.op1.indirect_disp.disp = (int16_t)val16;
	return 0;
}

// ----------------------------------------------------------------------------
int Inst_dbcc(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t dreg  = (header >> 0) & 7;
	uint16_t disp16;
	if (buffer.read_word(disp16))
		return 1;

	set_dreg(inst.op0, dreg);
	inst.op1.type = RELATIVE_BRANCH;
	inst.op1.relative_branch.disp = (int16_t)disp16;
	return 0;
}

// ----------------------------------------------------------------------------
int Inst_scc(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;
	return decode_ea(buffer, inst.op0, DATA_ALT, mode, reg, Size::BYTE);
}

// ----------------------------------------------------------------------------
int Inst_sbcd_reg(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t regx = (header >> 0) & 7;
	uint8_t regy = (header >> 9) & 7;
	set_dreg(inst.op0, regx);
	set_dreg(inst.op1, regy);
	return 0;
}

// ----------------------------------------------------------------------------
int Inst_sbcd_predec(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t regx = (header >> 0) & 7;
	uint8_t regy = (header >> 9) & 7;
	set_predec(inst.op0, regx);
	set_predec(inst.op1, regy);
	return 0;
}

// ----------------------------------------------------------------------------
int Inst_subx_reg(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t size = (header >> 6) & 3;
	Size ea_size = standard_size_table[size];
	if (ea_size == Size::NONE)
		return 1;
	inst.suffix = size_to_suffix(ea_size);
	uint8_t regx = (header >> 0) & 7;
	uint8_t regy = (header >> 9) & 7;
	set_dreg(inst.op0, regx);
	set_dreg(inst.op1, regy);
	return 0;
}

// ----------------------------------------------------------------------------
int Inst_subx_predec(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t size = (header >> 6) & 3;
	Size ea_size = standard_size_table[size];
	if (ea_size == Size::NONE)
		return 1;
	inst.suffix = size_to_suffix(ea_size);

	uint8_t regx = (header >> 0) & 7;
	uint8_t regy = (header >> 9) & 7;
	set_predec(inst.op0, regx);
	set_predec(inst.op1, regy);
	return 0;
}

// ----------------------------------------------------------------------------
int Inst_cmpm(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t size = (header >> 6) & 3;
	Size ea_size = standard_size_table[size];
	if (ea_size == Size::NONE)
		return 1;
	inst.suffix = size_to_suffix(ea_size);

	uint8_t regx = (header >> 0) & 7;
	uint8_t regy = (header >> 9) & 7;
	set_postinc(inst.op0, regx);
	set_postinc(inst.op1, regy);
	return 0;
}

// ----------------------------------------------------------------------------
int Inst_cmpa(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;
	uint8_t size = (header >> 8) & 1;
	uint8_t areg = (header >> 9) & 7;

	Size ea_size = size ? Size::LONG : Size::WORD;
	if (ea_size == Size::NONE)
		return 1;
	inst.suffix = size_to_suffix(ea_size);

	if (decode_ea(buffer, inst.op0, ALL, mode, reg, ea_size))
		return 1;
	set_areg(inst.op1, areg);
	return 0;
}

// ----------------------------------------------------------------------------
int Inst_cmp(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;
	uint8_t size = (header >> 6) & 3;
	uint8_t dreg = (header >> 9) & 7;
	Size ea_size = standard_size_table[size];
	if (ea_size == Size::NONE)
		return 1;
	inst.suffix = size_to_suffix(ea_size);

	if (decode_ea(buffer, inst.op0, ALL, mode, reg, ea_size))
		return 1;
	set_dreg(inst.op1, dreg);
	return 0;
}

// ----------------------------------------------------------------------------
int Inst_eor(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;
	uint8_t size = (header >> 6) & 3;
	uint8_t dreg = (header >> 9) & 7;
	Size ea_size = standard_size_table[size];
	if (ea_size == Size::NONE)
		return 1;
	inst.suffix = size_to_suffix(ea_size);

	// src is d-reg
	set_dreg(inst.op0, dreg);

	// dest is EA
	if (decode_ea(buffer, inst.op1, DATA_ALT, mode, reg, ea_size))
		return 1;
	return 0;
}

// ----------------------------------------------------------------------------
int Inst_muldiv(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;
	uint8_t dreg = (header >> 9) & 7;
	Size ea_size = Size::WORD;
	if (ea_size == Size::NONE)
		return 1;
	inst.suffix = Suffix::WORD;

	// src is EA
	if (decode_ea(buffer, inst.op0, DATA, mode, reg, ea_size))
		return 1;

	// dst is d-reg
	set_dreg(inst.op1, dreg);
	return 0;
}

// ----------------------------------------------------------------------------
int Inst_chk(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t dreg = (header >> 9) & 7;
	uint8_t size = (header >> 7) & 1;
	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;
	Size ea_size = size ? Size::WORD : Size::LONG;
	inst.suffix = size_to_suffix(ea_size);

	set_dreg(inst.op1, dreg);
	return decode_ea(buffer, inst.op0, DATA, mode, reg, ea_size);
}

// ----------------------------------------------------------------------------
int Inst_exg_dd(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t regx = (header >> 0) & 7;
	uint8_t regy = (header >> 9) & 7;
	set_dreg(inst.op0, regx);
	set_dreg(inst.op1, regy);
	return 0;
}

// ----------------------------------------------------------------------------
int Inst_exg_aa(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t regx = (header >> 0) & 7;
	uint8_t regy = (header >> 9) & 7;
	set_areg(inst.op0, regx);
	set_areg(inst.op1, regy);
	return 0;
}

// ----------------------------------------------------------------------------
int Inst_exg_da(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t regx = (header >> 0) & 7;
	uint8_t regy = (header >> 9) & 7;
	set_dreg(inst.op0, regx);
	set_areg(inst.op1, regy);
	return 0;
}

// ----------------------------------------------------------------------------
int Inst_imm_ccr(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint16_t val16;
	if (buffer.read_word(val16))
		return 1;
	if ((val16 & 0xff00) != 0)
		return 1;

	set_imm_byte(inst.op0, val16 & 0xff);
	inst.op1.type = OpType::CCR;
	return 0;
}

// ----------------------------------------------------------------------------
int Inst_imm_sr(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint16_t val16;
	if (buffer.read_word(val16))
		return 1;

	set_imm_word(inst.op0, val16);
	inst.op1.type = OpType::SR;
	return 0;
}

// ----------------------------------------------------------------------------
typedef int (*pfnDecoderFunc)(buffer_reader& buffer, instruction& inst, uint32_t header);

// ===========================================================
//   MATCHER TABLE
// ===========================================================
//
//	This is the main dispatch table. It matches partial bitstreams to
//	call through to a handler class.
//
//	Entries go in the order Most specific -> Least Specific
//	i.e. they should go in terms of decreasing "CT" i.e. bitcount

// TODO switch tag to an instruction type
struct matcher_entry
{
	uint32_t		mask0;
	uint32_t		val0;
	const char*		tag;
	pfnDecoderFunc	func;
};

#define MATCH_ENTRY1_IMPL(shift, bitcount, val, tag, func)   \
	{ (((1U<<(bitcount))-1U)<<(shift)), (val<<(shift)), tag, func }

#define MATCH_ENTRY2_IMPL(shift, bitcount, val, shift2, bitcount2, val2, tag, func)   \
	{ (((1U<<(bitcount))-1U)<<(shift)) | (((1U<<(bitcount2))-1U)<<(shift2)), \
		(val<<(shift)) | (val2<<(shift2)), \
	   tag, func }

#define MATCH_ENTRY3_IMPL(shift, bitcount, val, shift2, bitcount2, val2, shift3, bitcount3, val3, tag, func)   \
	{ (((1U<<(bitcount))-1U)<<(shift)) | (((1U<<(bitcount2))-1U)<<(shift2))  | (((1U<<(bitcount3))-1U)<<(shift3)), \
		(val<<(shift)) | (val2<<(shift2)) | (val3<<(shift3)), \
	   tag, func }

matcher_entry g_matcher_table[] =
{
	//		          SH CT							Tag				  Decoder
	MATCH_ENTRY1_IMPL(0,16,0b0000101001111100,		"eori",		Inst_imm_sr ), // supervisor
	MATCH_ENTRY1_IMPL(0,16,0b0000001000111100,		"andi",		Inst_imm_ccr ),
	MATCH_ENTRY1_IMPL(0,16,0b0000101000111100,		"eori",		Inst_imm_ccr ),
	MATCH_ENTRY1_IMPL(0,16,0b0000000000111100,		"ori",		Inst_imm_ccr ),
	MATCH_ENTRY1_IMPL(0,16,0b0000000001111100,		"ori",		Inst_imm_sr ), // supervisor
	MATCH_ENTRY1_IMPL(0,16,0b0000001001111100,		"andi",		Inst_imm_sr ), // supervisor

	MATCH_ENTRY1_IMPL(0,16,0b0100101011111100,		"illegal",	Inst_simple ),
	MATCH_ENTRY1_IMPL(0,16,0b0100111001110000,		"reset",	Inst_simple ), // supervisor
	MATCH_ENTRY1_IMPL(0,16,0b0100111001110001,		"nop",		Inst_simple ),
	MATCH_ENTRY1_IMPL(0,16,0b0100111001110011,		"rte",		Inst_simple ), // supervisor
	MATCH_ENTRY1_IMPL(0,16,0b0100111001110101,		"rts",		Inst_simple ),
	MATCH_ENTRY1_IMPL(0,16,0b0100111001110110,		"trapv",	Inst_simple ),
	MATCH_ENTRY1_IMPL(0,16,0b0100111001110111,		"rtr",		Inst_simple ),
	MATCH_ENTRY1_IMPL(0,16,0b0100111001110010,		"stop",		Inst_stop ),

	MATCH_ENTRY1_IMPL(3,13,0b0100100001000,			"swap",		Inst_swap ),
	MATCH_ENTRY1_IMPL(3,13,0b0100111001010,			"link",		Inst_link_w ),
	//([ ( 3,13, 0b0100100000001)			  ,		"link.l",	Inst_link_l ),  # not on 68000
	MATCH_ENTRY1_IMPL(3,13,0b0100111001011,			"unlk",		Inst_unlk ),
	MATCH_ENTRY1_IMPL(3,13,0b0100111001100,			"move",		Inst_move_to_usp ),
	MATCH_ENTRY1_IMPL(3,13,0b0100111001101,			"move",		Inst_move_from_usp ),
	MATCH_ENTRY1_IMPL(3,13,0b0100100010000,			"ext",		Inst_ext ),
	MATCH_ENTRY1_IMPL(3,13,0b0100100011000,			"ext",		Inst_ext ),
	//([ ( 3,13, 0b0100100011000)			  ,		"extb.l",	Inst_ext ),	 # not on 68000

	MATCH_ENTRY1_IMPL(4,12,0b010011100100,			"trap",		Inst_trap ),

	MATCH_ENTRY1_IMPL(6,10,0b0100000011,			"move",		Inst_move_from_sr ),   // supervisor
	MATCH_ENTRY1_IMPL(6,10,0b0100011011,			"move",		Inst_move_to_sr ),   // supervisor
	//([ ( 6,10, 0b0100001011)				 ,		"move from ccr",	 Inst ),		  # not on 68000
	MATCH_ENTRY1_IMPL(6,10,0b0100010011,			"move",		Inst_move_to_ccr ),
	MATCH_ENTRY1_IMPL(6,10,0b0100100000,			"nbcd",		Inst_nbcd ),
	MATCH_ENTRY1_IMPL(6,10,0b0100100001,			"pea",		Inst_pea ),
	MATCH_ENTRY1_IMPL(6,10,0b0100101011,			"tas",		Inst_tas ),
	MATCH_ENTRY1_IMPL(6,10,0b0100111010,			"jsr",		Inst_jump ),
	MATCH_ENTRY1_IMPL(6,10,0b0100111011,			"jmp",		Inst_jump ),
	MATCH_ENTRY1_IMPL(6,10,0b1110000011,			"asr",		Inst_asl_asr_mem ),
	MATCH_ENTRY1_IMPL(6,10,0b1110000111,			"asl",		Inst_asl_asr_mem ),

	MATCH_ENTRY2_IMPL(12,4,0b0000, 3,6,0b100001,	"movep",	Inst_movep_mem_reg ),
	MATCH_ENTRY2_IMPL(12,4,0b0000, 3,6,0b101001,	"movep",	Inst_movep_mem_reg ),
	MATCH_ENTRY2_IMPL(12,4,0b0000, 3,6,0b110001,	"movep",	Inst_movep_reg_mem ),
	MATCH_ENTRY2_IMPL(12,4,0b0000, 3,6,0b111001,	"movep",	Inst_movep_reg_mem ),
	MATCH_ENTRY1_IMPL(6,10,0b0000100001,			"bchg",		Inst_bchg_imm ),
	MATCH_ENTRY1_IMPL(6,10,0b0000100010,			"bclr",		Inst_bchg_imm ),
	MATCH_ENTRY1_IMPL(6,10,0b0000100011,			"bset",		Inst_bchg_imm ),
	MATCH_ENTRY1_IMPL(6,10,0b0000100000,			"btst",		Inst_bchg_imm ),
	MATCH_ENTRY1_IMPL(7,9,0b010010001,				"movem",	Inst_movem_reg_mem ), // Register to memory.
	MATCH_ENTRY1_IMPL(7,9,0b010011001,				"movem",	Inst_movem_mem_reg ), // Memory to register.
	MATCH_ENTRY1_IMPL(8,8,0b00000000,				"ori",		Inst_integer_imm_ea ),
	MATCH_ENTRY1_IMPL(8,8,0b00000010,				"andi",		Inst_integer_imm_ea ),
	MATCH_ENTRY1_IMPL(8,8,0b00000100,				"subi",		Inst_integer_imm_ea ),
	MATCH_ENTRY1_IMPL(8,8,0b00000110,				"addi",		Inst_integer_imm_ea ),
	MATCH_ENTRY1_IMPL(8,8,0b00001010,				"eori",		Inst_integer_imm_ea ),
	MATCH_ENTRY1_IMPL(8,8,0b00001100,				"cmpi",		Inst_integer_imm_ea ),
	MATCH_ENTRY1_IMPL(8,8,0b01000000,				"negx",		Inst_size_ea ),
	MATCH_ENTRY1_IMPL(8,8,0b01000010,				"clr",		Inst_size_ea ),
	MATCH_ENTRY1_IMPL(8,8,0b01000100,				"neg",		Inst_size_ea ),
	MATCH_ENTRY1_IMPL(8,8,0b01000110,				"not",		Inst_size_ea ),
	MATCH_ENTRY1_IMPL(8,8,0b01001010,				"tst",		Inst_size_ea ),
	MATCH_ENTRY1_IMPL(8,8,0b01100000,				"bra",		Inst_branch ),
	MATCH_ENTRY1_IMPL(8,8,0b01100001,				"bsr",		Inst_branch ),
	MATCH_ENTRY1_IMPL(8,8,0b01100010,				"bhi",		Inst_branch ),
	MATCH_ENTRY1_IMPL(8,8,0b01100011,				"bls",		Inst_branch ),
	MATCH_ENTRY1_IMPL(8,8,0b01100100,				"bcc",		Inst_branch ),
	MATCH_ENTRY1_IMPL(8,8,0b01100101,				"bcs",		Inst_branch ),
	MATCH_ENTRY1_IMPL(8,8,0b01100110,				"bne",		Inst_branch ),
	MATCH_ENTRY1_IMPL(8,8,0b01100111,				"beq",		Inst_branch ),
	MATCH_ENTRY1_IMPL(8,8,0b01101000,				"bvc",		Inst_branch ),
	MATCH_ENTRY1_IMPL(8,8,0b01101001,				"bvs",		Inst_branch ),
	MATCH_ENTRY1_IMPL(8,8,0b01101010,				"bpl",		Inst_branch ),
	MATCH_ENTRY1_IMPL(8,8,0b01101011,				"bmi",		Inst_branch ),
	MATCH_ENTRY1_IMPL(8,8,0b01101100,				"bge",		Inst_branch ),
	MATCH_ENTRY1_IMPL(8,8,0b01101101,				"blt",		Inst_branch ),
	MATCH_ENTRY1_IMPL(8,8,0b01101110,				"bgt",		Inst_branch ),
	MATCH_ENTRY1_IMPL(8,8,0b01101111,				"ble",		Inst_branch ),

	MATCH_ENTRY2_IMPL(12,4,0b0000, 6,3,0b101,		"bchg",		Inst_bchg ),
	MATCH_ENTRY2_IMPL(12,4,0b0000, 6,3,0b110,		"bclr",		Inst_bchg ),
	MATCH_ENTRY2_IMPL(12,4,0b0000, 6,3,0b111,		"bset",		Inst_bchg ),
	MATCH_ENTRY2_IMPL(12,4,0b0000, 6,3,0b100,		"btst",		Inst_bchg ),

	//Table 3-19. Conditional Tests
	// These sneakily take the "001" in the bottom 3 bits to override the EA parts of Scc
	MATCH_ENTRY1_IMPL(3,13,0b0101000011001,			"dbra",		Inst_dbcc ),
	MATCH_ENTRY1_IMPL(3,13,0b0101000111001,			"dbf",		Inst_dbcc ),
	MATCH_ENTRY1_IMPL(3,13,0b0101001011001,			"dbhi",		Inst_dbcc ),
	MATCH_ENTRY1_IMPL(3,13,0b0101001111001,			"dbls",		Inst_dbcc ),
	MATCH_ENTRY1_IMPL(3,13,0b0101010011001,			"dbcc",		Inst_dbcc ),
	MATCH_ENTRY1_IMPL(3,13,0b0101010111001,			"dbcs",		Inst_dbcc ),
	MATCH_ENTRY1_IMPL(3,13,0b0101011011001,			"dbne",		Inst_dbcc ),
	MATCH_ENTRY1_IMPL(3,13,0b0101011111001,			"dbeq",		Inst_dbcc ),
	MATCH_ENTRY1_IMPL(3,13,0b0101100011001,			"dbvc",		Inst_dbcc ),
	MATCH_ENTRY1_IMPL(3,13,0b0101100111001,			"dbvs",		Inst_dbcc ),
	MATCH_ENTRY1_IMPL(3,13,0b0101101011001,			"dbpl",		Inst_dbcc ),
	MATCH_ENTRY1_IMPL(3,13,0b0101101111001,			"dbmi",		Inst_dbcc ),
	MATCH_ENTRY1_IMPL(3,13,0b0101110011001,			"dbge",		Inst_dbcc ),
	MATCH_ENTRY1_IMPL(3,13,0b0101110111001,			"dblt",		Inst_dbcc ),
	MATCH_ENTRY1_IMPL(3,13,0b0101111011001,			"dbgt",		Inst_dbcc ),
	MATCH_ENTRY1_IMPL(3,13,0b0101111111001,			"dble",		Inst_dbcc ),

	MATCH_ENTRY1_IMPL(6,10,0b0101000011,			"st",		Inst_scc ),
	MATCH_ENTRY1_IMPL(6,10,0b0101000111,			"sf",		Inst_scc ),
	MATCH_ENTRY1_IMPL(6,10,0b0101001011,			"shi",		Inst_scc ),
	MATCH_ENTRY1_IMPL(6,10,0b0101001111,			"sls",		Inst_scc ),
	MATCH_ENTRY1_IMPL(6,10,0b0101010011,			"scc",		Inst_scc ),
	MATCH_ENTRY1_IMPL(6,10,0b0101010111,			"scs",		Inst_scc ),
	MATCH_ENTRY1_IMPL(6,10,0b0101011011,			"sne",		Inst_scc ),
	MATCH_ENTRY1_IMPL(6,10,0b0101011111,			"seq",		Inst_scc ),
	MATCH_ENTRY1_IMPL(6,10,0b0101100011,			"svc",		Inst_scc ),
	MATCH_ENTRY1_IMPL(6,10,0b0101100111,			"svs",		Inst_scc ),
	MATCH_ENTRY1_IMPL(6,10,0b0101101011,			"spl",		Inst_scc ),
	MATCH_ENTRY1_IMPL(6,10,0b0101101111,			"smi",		Inst_scc ),
	MATCH_ENTRY1_IMPL(6,10,0b0101110011,			"sge",		Inst_scc ),
	MATCH_ENTRY1_IMPL(6,10,0b0101110111,			"slt",		Inst_scc ),
	MATCH_ENTRY1_IMPL(6,10,0b0101111011,			"sgt",		Inst_scc ),
	MATCH_ENTRY1_IMPL(6,10,0b0101111111,			"sle",		Inst_scc ),

	MATCH_ENTRY2_IMPL(12,4,0b0101, 8,1,0b1,			"subq",		Inst_subq ),
	MATCH_ENTRY2_IMPL(12,4,0b0101, 8,1,0b0,			"addq",		Inst_subq ),
	MATCH_ENTRY2_IMPL(12,4,0b0111, 8,1,0b0,			"moveq",	Inst_moveq ),

	MATCH_ENTRY2_IMPL(12,4,0b1000, 3,6,0b100000,	"sbcd",		Inst_sbcd_reg ),
	MATCH_ENTRY2_IMPL(12,4,0b1000, 3,6,0b100001,	"sbcd",		Inst_sbcd_predec ),
	MATCH_ENTRY2_IMPL(12,4,0b1100, 3,6,0b100000,	"abcd",		Inst_sbcd_reg ),
	MATCH_ENTRY2_IMPL(12,4,0b1100, 3,6,0b100001,	"abcd",		Inst_sbcd_predec ),

	MATCH_ENTRY3_IMPL(12,4,0b1001, 8,1,1, 3,3,0,	"subx",		Inst_subx_reg ),
	MATCH_ENTRY3_IMPL(12,4,0b1001, 8,1,1, 3,3,1,	"subx",		Inst_subx_predec ),
	MATCH_ENTRY3_IMPL(12,4,0b1101, 8,1,1, 3,3,0,	"addx",		Inst_subx_reg ),
	MATCH_ENTRY3_IMPL(12,4,0b1101, 8,1,1, 3,3,1,	"addx",		Inst_subx_predec ),

	MATCH_ENTRY3_IMPL(12,4,0b1011, 8,1,1, 3,3,1,	"cmpm",		Inst_cmpm ),
	MATCH_ENTRY2_IMPL(12,4,0b1011, 6,2,3,			"cmpa",		Inst_cmpa ),

	// Nasty case where eor and cmp mirror one another
	MATCH_ENTRY2_IMPL(12,4,0b1011, 6,3,0b100,		"eor",		Inst_eor ),
	MATCH_ENTRY2_IMPL(12,4,0b1011, 6,3,0b101,		"eor",		Inst_eor ),
	MATCH_ENTRY2_IMPL(12,4,0b1011, 6,3,0b110,		"eor",		Inst_eor ),
	MATCH_ENTRY2_IMPL(12,4,0b1100, 6,3,0b011,		"mulu",		Inst_muldiv ),
	MATCH_ENTRY2_IMPL(12,4,0b1100, 6,3,0b111,		"muls",		Inst_muldiv ),

	MATCH_ENTRY3_IMPL(12,4,0b1110, 3,2,0, 8,1,1,	"asl",		Inst_shift_reg ),
	MATCH_ENTRY3_IMPL(12,4,0b1110, 3,2,0, 8,1,0,	"asr",		Inst_shift_reg ),
	MATCH_ENTRY3_IMPL(12,4,0b1110, 3,2,1, 8,1,1,	"lsl",		Inst_shift_reg ),
	MATCH_ENTRY3_IMPL(12,4,0b1110, 3,2,1, 8,1,0,	"lsr",		Inst_shift_reg ),
	MATCH_ENTRY3_IMPL(12,4,0b1110, 3,2,2, 8,1,1,	"roxl",		Inst_shift_reg ),
	MATCH_ENTRY3_IMPL(12,4,0b1110, 3,2,2, 8,1,0,	"roxr",		Inst_shift_reg ),
	MATCH_ENTRY3_IMPL(12,4,0b1110, 3,2,3, 8,1,1,	"rol",		Inst_shift_reg ),
	MATCH_ENTRY3_IMPL(12,4,0b1110, 3,2,3, 8,1,0,	"ror",		Inst_shift_reg ),

	MATCH_ENTRY2_IMPL(12,4,0b0100, 6,3,0b111,		"lea",		Inst_lea ),
	MATCH_ENTRY2_IMPL(12,4,0b1000, 6,3,0b011,		"divu",		Inst_muldiv ),
	MATCH_ENTRY2_IMPL(12,4,0b1000, 6,3,0b111,		"divs",		Inst_muldiv ),

	// NOTE: specific case to override CHK since it doesn't support addressing mode 001
	MATCH_ENTRY2_IMPL(12,4,0b1100, 3,6,0b101000,	"exg",		Inst_exg_dd ),
	MATCH_ENTRY2_IMPL(12,4,0b1100, 3,6,0b101001,	"exg",		Inst_exg_aa ),
	MATCH_ENTRY2_IMPL(12,4,0b1100, 3,6,0b110001,	"exg",		Inst_exg_da ),
	MATCH_ENTRY2_IMPL(12,4,0b0100, 6,3,0b110,		"chk",		Inst_chk ),
	MATCH_ENTRY2_IMPL(12,4,0b0100, 6,3,0b100,		"chk",		Inst_chk ),	// not 68000

	MATCH_ENTRY2_IMPL(12,4,0b1001, 6,2,0b11,		"suba",		Inst_addsuba ),
	MATCH_ENTRY2_IMPL(12,4,0b1101, 6,2,0b11,		"adda",		Inst_addsuba ),

	// Fallback generics
	MATCH_ENTRY1_IMPL(12,4,0b1011,					"cmp",		Inst_cmp ),

	// Following where src/dest is d-register
	MATCH_ENTRY1_IMPL(12,4,0b1000,					"or",		Inst_alu_dreg ),
	MATCH_ENTRY1_IMPL(12,4,0b1001,					"sub",		Inst_alu_dreg ),
	MATCH_ENTRY1_IMPL(12,4,0b1101,					"add",		Inst_alu_dreg ),
	MATCH_ENTRY1_IMPL(12,4,0b1100,					"and",		Inst_alu_dreg ),

	MATCH_ENTRY2_IMPL(13,3,0b001, 6,3,0b001,		"movea",	Inst_movea ),
	MATCH_ENTRY1_IMPL(14,2,0b00,					"move",		Inst_move ),

	{ 0 }			 // end sentinel
};

// ----------------------------------------------------------------------------
// decode a single instruction if possible
int decode(buffer_reader& buffer, instruction& inst)
{
	inst.byte_count = 2;	// assume error
	inst.tag = NULL;
	inst.suffix = Suffix::NONE;

	// Check remaining size
	bool has32 = false;
	uint16_t header0 = 0;
	uint16_t header1 = 0;
	uint32_t start_pos = buffer.get_pos();

	if (buffer.get_remain() >= 2)
	{
		buffer.read_word(header0);
		inst.header = header0;
	}

	// Make a temp copy of the reader to pass to the decoder, after the first word
	buffer_reader reader_tmp = buffer;
	for (const matcher_entry* pEntry = g_matcher_table;
		pEntry->mask0 != 0;
		++pEntry)
	{
		// Choose 16 or 32 bits for the check
		uint32_t header = header0;
		if ((header & pEntry->mask0) != pEntry->val0)
			continue;

		// Do specialised decoding
		inst.tag = pEntry->tag;
		int res = 0;
		if (pEntry->func)
			res = pEntry->func(reader_tmp, inst, header);
		inst.byte_count = reader_tmp.get_pos() - start_pos;
		return res;
	}

	// no match found
	return 1;
}
