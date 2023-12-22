#include "decode.h"

#include <stdio.h>
#include "buffer.h"
#include "instruction.h"
#define ENTRY(opcode, func)		{ instruction::Opcode::opcode, func }

namespace hopper56
{
	// Converts bit IDs in the parallel moves, to full register IDs
	static Register pmove_registers_1[32] =
	{
		Register::NONE, Register::NONE,	Register::NONE, Register::NONE,
		Register::X0, Register::X1, Register::Y0, Register::Y1,
		Register::A0, Register::B0,
		Register::A2, Register::B2,
		Register::A1, Register::B1,
		Register::A, Register::B,
	 	Register::R0, Register::R1, Register::R2, Register::R3,
		Register::R4, Register::R5, Register::R6, Register::R7,
		Register::N0, Register::N1, Register::N2, Register::N3,
		Register::N4, Register::N5, Register::N6, Register::N7
	};

	static Register pmove_registers_2[4] =
	{
		Register::X0, Register::X1, Register::A, Register::B
	};

	struct op_entry
	{
		instruction::Opcode opcode;
		int (*func)(instruction& inst, uint32_t header, const decode_settings&, buffer_reader& buf);	// decoder
	};

	int dummy_func(instruction& inst, uint32_t header, const decode_settings&, buffer_reader& buf)
	{
		return 0;
	}

	// ========================================================================
	// Setters for operands

	// Create R-register from 0-7 index
	Register make_r(uint32_t r_num) { return (Register)(Register::R0 + r_num); }
	// Create N-register from 0-7 index
	Register make_n(uint32_t n_num) { return (Register)(Register::N0 + n_num); }

	void set_reg(operand& op, Register reg)
	{
		op.type = operand::REG; op.reg.index = reg;
	}
	void set_imm_short(operand& op, int8_t val)
	{
		op.type = operand::IMM_SHORT; op.imm_short.val = val;
	}
	void set_postdec_offset(operand& op, uint32_t reg_num)
	{
		op.type = operand::POSTDEC_OFFSET;
		op.postdec_offset.index_1 = make_r(reg_num);
		op.postdec_offset.index_2 = make_n(reg_num);
	}
	void set_postinc_offset(operand& op, uint32_t reg_num)
	{
		op.type = operand::POSTINC_OFFSET;
		op.postinc_offset.index_1 = make_r(reg_num);
		op.postinc_offset.index_2 = make_n(reg_num);
	}
	void set_postdec(operand& op, uint32_t reg_num)
	{
		op.type = operand::POSTDEC; op.postdec.index = make_r(reg_num);
	}
	void set_postinc(operand& op, uint32_t reg_num)
	{
		op.type = operand::POSTINC; op.postdec.index = make_r(reg_num);
	}
	void set_index_offset(operand& op, uint32_t reg_num)
	{
		op.type = operand::INDEX_OFFSET;
		op.postinc_offset.index_1 = make_r(reg_num);
		op.postinc_offset.index_2 = make_n(reg_num);
	}
	void set_no_update(operand& op, uint32_t reg_num)
	{
		op.type = operand::NO_UPDATE; op.no_update.index = make_r(reg_num);
	}
	void set_predec(operand& op, uint32_t reg_num)
	{
		op.type = operand::PREDEC; op.predec.index = make_r(reg_num);
	}
	void set_abs(operand& op, uint32_t addr)
	{
		op.type = operand::ABS; op.abs.address = addr;
	}
	void set_imm(operand& op, uint32_t val)
	{
		op.type = operand::IMM; op.imm.val = val;
 	}

	void allocate_operands(pmove& pmove, uint32_t w, operand*& opA, operand*& opB)
	{
		// Choose src/dest for "w" modes,
		operand& op0 = pmove.operands[0];
		operand& op1 = pmove.operands[1];
		opA = (w == 0) ? &op1 : &op0;
		opB = (w == 0) ? &op0 : &op1;
	}

	// ========================================================================
	//	OPCODE SHARED FUNCTIONS
	// ========================================================================
	// Handle addressing mode and register for Rn/abs/immediate addressing in parallel moves
	int decode_mmmrrr(operand& op, Memory mem, uint32_t mmm, uint32_t rrr, buffer_reader& buf)
	{
		uint32_t next = 0;
		op.memory = mem;
		switch (mmm)
		{
			case 0:		set_postdec_offset(op, rrr); return 0;
			case 1:		set_postinc_offset(op, rrr); return 0;
			case 2:		set_postdec(op, rrr); return 0;
			case 3:		set_postinc(op, rrr); return 0;
			case 4:		set_no_update(op, rrr); return 0;
			case 5: 	set_index_offset(op, rrr); return 0;
			case 7:		set_predec(op, rrr); return 0;
			case 6:
				// NOTE these modes only available in some modes (i.e. when reading an operand)
				if (rrr == 0x4)
				{
					// Immediate value in next word
					if (buf.read_word(next))
						return 1;
					set_imm(op, next);
					op.memory = Memory::MEM_NONE;
					return 0;
				}
				else if (rrr == 0x0)
				{
					if (buf.read_word(next))
						return 1;
					set_abs(op, next);
					return 0;
				}
				return 1;
			default:
				return 1;
		} // switch
		return 1;
	}

	int decode_pmove(instruction& inst, uint32_t header, const decode_settings&, buffer_reader& buf)
	{
		uint32_t pdata = (header >> 8) & 0xffff;

		// UM, page A-162 onwards

		// Pre-choose operand order for read vs write
		uint32_t w = (pdata >> 7) & 0x1;

		// Pre-choose modes and registers
		uint32_t mmm = (pdata >> 3) & 0x7;
		uint32_t rrr = (pdata >> 0) & 0x7;
		
		// Prep the most commonly-used operand slots
		operand& op0 = inst.pmoves[0].operands[0];
		operand& op1 = inst.pmoves[0].operands[1];
		operand* opA, * opB;

		if (pdata == 0x2000)
			return 0;			// No Parallel Move
		if ((pdata & 0xffe0) == 0x2040)
		{
			// Address Register Update (one operand)
			uint32_t mm = (pdata >> 3) & 3;
			switch (mm)
			{
				case 0: set_postdec_offset(op0, rrr); break;
				case 1: set_postinc_offset(op0, rrr); break;
				case 2: set_postdec(op0, rrr); break;
				case 3: set_postinc(op0, rrr); break;
			}
			return 0;			
		}
		if ((pdata & 0xf040) == 0x1000)
		{
			// X: Memory and Register Move, Class I
			// 0001 ffdf W0MM MRRR format
			// 1111 0000 0100 0000 mask
			// 0001 0000 0000 0000 val
			allocate_operands(inst.pmoves[0], w, opA, opB);
			uint32_t f = (pdata >> 8) & 0x1;		// S2/D2
			uint32_t d = (pdata >> 9) & 0x1;
		   	uint32_t ff = (pdata >> 10) & 0x3;		// S1/D1
			set_reg(*opB, pmove_registers_2[ff]);
			if (decode_mmmrrr(*opA, MEM_X, mmm, rrr, buf))
				return 1;
			// Second pmove is limited
			set_reg(inst.pmoves[1].operands[0], d ? Register::B  : Register::A);
			set_reg(inst.pmoves[1].operands[1], f ? Register::Y1 : Register::Y0);
			return 0;
		}
		if ((pdata & 0xf040) == 0x1040)
		{
			// Y: Memory and Register Move, Class I
			// 0001 deff W1MM MRRR format
			// 1111 0000 0100 0000 mask
			// 0001 0000 0100 0000 val
			allocate_operands(inst.pmoves[1], w, opA, opB);
			uint32_t e = (pdata >> 10) & 0x1;		// S2/D2
			uint32_t d = (pdata >> 9) & 0x1;
		   	uint32_t ff = (pdata >> 8) & 0x3;		// S1/D1
			set_reg(*opB, pmove_registers_2[ff]);
			if (decode_mmmrrr(*opA, MEM_Y, mmm, rrr, buf))
				return 1;
			// Second pmove is limited
			set_reg(inst.pmoves[0].operands[0], d ? Register::B  : Register::A);
			set_reg(inst.pmoves[0].operands[1], e ? Register::X1 : Register::X0);
			return 0;
		}
		if ((pdata & 0xfec0) == 0x0800)
		{
			// X: Memory and Register Move, Class II
			// 0000 100d 00MM MRRR format
			// 1111 1110 1100 0000 mask
			// 0000 1000 0000 0000 val
			uint32_t d = (pdata >> 8) & 0x1;
			Register accum_reg = d ? Register::B : Register::A;
			set_reg(op0, accum_reg);
			if (decode_mmmrrr(op1, MEM_X, mmm, rrr, buf))		// TODO this is the destination so some modes are disallowed
				return 1;
			// Second pmove is always X0
			set_reg(inst.pmoves[1].operands[0], Register::X0);
		   	set_reg(inst.pmoves[1].operands[1], accum_reg);
			return 0;
		}
		if ((pdata & 0xc840) == 0x4000)
		{
			// X: memory from short absolute
			allocate_operands(inst.pmoves[0], w, opA, opB);
			uint32_t ddddd = ((pdata >> 8) & 0x7) | ((pdata >> 9) & 0x18);
			uint32_t ea = (pdata >> 0) & 0x3f;
			opA->memory = MEM_X;
			set_abs(*opA, ea);
			set_reg(*opB, pmove_registers_1[ddddd]);
			return 0;
		}
		if ((pdata & 0xc840) == 0x4040)
		{
			// X: memory move
			// 01dd 0ddd W1MM MRRR format
			// 1100 1000 0100 0000 mask
			// 0100 0000 0100 0000 val
			allocate_operands(inst.pmoves[0], w, opA, opB);
			uint32_t ddddd = ((pdata >> 8) & 0x7) | ((pdata >> 9) & 0x18);
			set_reg(*opB, pmove_registers_1[ddddd]);
			return decode_mmmrrr(*opA, MEM_X, mmm, rrr, buf);
		}
		if ((pdata & 0xc840) == 0x4840)
		{
			// Y: Memory move
			// 01dd 1ddd W1MM MRRR format
			// 1100 1000 0100 0000 mask
			// 0100 1000 0100 0000 val
			allocate_operands(inst.pmoves[1], w, opA, opB);
			uint32_t ddddd = ((pdata >> 8) & 0x7) | ((pdata >> 9) & 0x18);
			set_reg(*opB, pmove_registers_1[ddddd]);
			return decode_mmmrrr(*opA, MEM_Y, mmm, rrr, buf);
		}
		if ((pdata & 0xfc00) == 0x2000)
		{
			// Reg->Reg (special case of immediate short)
			uint32_t imrege = (pdata >> 5) & 0x1f;
			uint32_t imregd = (pdata >> 0) & 0x1f;
			set_reg(op0, pmove_registers_1[imrege]);
			set_reg(op1, pmove_registers_1[imregd]);
			return 0;
		}
		if ((pdata & 0xe000) == 0x2000)
		{	
			// Immediate short data move
			uint32_t imdata = (pdata & 0xff);
			uint32_t imreg = (pdata >> 8) & 0x1f;
			set_imm_short(op0, imdata);
			set_reg(op1, pmove_registers_1[imreg]);
			return 0;
		}

		return 0;
	}

	// ========================================================================
	//	OPCODE FUNCTIONS
	// ========================================================================

	int abs_func(instruction& inst, uint32_t header, const decode_settings& settings, buffer_reader& buf)
	{
		uint32_t a_or_b = (header >> 3) & 1;
		inst.operands[0].type = operand::REG;
		inst.operands[0].reg.index = a_or_b ? Register::B : Register::A;
		return decode_pmove(inst, header, settings, buf);
	}
	
	int adc_func(instruction& inst, uint32_t header, const decode_settings& settings, buffer_reader& buf)
	{
		uint32_t x_or_y = (header >> 4) & 1;
		set_reg(inst.operands[0], x_or_y ? Register::Y : Register::X);
		uint32_t a_or_b = (header >> 3) & 1;
		set_reg(inst.operands[1], a_or_b ? Register::B : Register::A);
		return decode_pmove(inst, header, settings, buf);
	}

	// ========================================================================
	//	OPCODE TABLE
	// ========================================================================
	const op_entry entries[256] =
	{
		// 0x
		ENTRY(INVALID, dummy_func),					// 0000 0000
		ENTRY(INVALID, dummy_func),					// 0000 0001
		ENTRY(INVALID, dummy_func),					// 0000 0010
		ENTRY(INVALID, dummy_func),					// 0000 0011
		ENTRY(INVALID, dummy_func),					// 0000 0100
		ENTRY(INVALID, dummy_func),					// 0000 0101
		ENTRY(INVALID, dummy_func),					// 0000 0110
		ENTRY(INVALID, dummy_func),					// 0000 0111
		ENTRY(INVALID, dummy_func),					// 0000 1000
		ENTRY(INVALID, dummy_func),					// 0000 1001
		ENTRY(INVALID, dummy_func),					// 0000 1010
		ENTRY(INVALID, dummy_func),					// 0000 1011
		ENTRY(INVALID, dummy_func),					// 0000 1100
		ENTRY(INVALID, dummy_func),					// 0000 1101
		ENTRY(INVALID, dummy_func),					// 0000 1110
		ENTRY(INVALID, dummy_func),					// 0000 1111

		// 1x
		ENTRY(INVALID, dummy_func),					// 0001 0000
		ENTRY(INVALID, dummy_func),					// 0001 0001
		ENTRY(INVALID, dummy_func),					// 0001 0010
		ENTRY(INVALID, dummy_func),					// 0001 0011
		ENTRY(INVALID, dummy_func),					// 0001 0100
		ENTRY(INVALID, dummy_func),					// 0001 0101
		ENTRY(INVALID, dummy_func),					// 0001 0110
		ENTRY(INVALID, dummy_func),					// 0001 0111
		ENTRY(INVALID, dummy_func),					// 0001 1000
		ENTRY(INVALID, dummy_func),					// 0001 1001
		ENTRY(INVALID, dummy_func),					// 0001 1010
		ENTRY(INVALID, dummy_func),					// 0001 1011
		ENTRY(INVALID, dummy_func),					// 0001 1100
		ENTRY(INVALID, dummy_func),					// 0001 1101
		ENTRY(INVALID, dummy_func),					// 0001 1110
		ENTRY(INVALID, dummy_func),					// 0001 1111

		// 2x
		ENTRY(INVALID, dummy_func),					// 0010 0000
		ENTRY(ADC,     adc_func),					// 0010 0001
		ENTRY(INVALID, dummy_func),					// 0010 0010
		ENTRY(INVALID, dummy_func),					// 0010 0011
		ENTRY(INVALID, dummy_func),					// 0010 0100
		ENTRY(INVALID, dummy_func),					// 0010 0101
		ENTRY(ABS,     abs_func),					// 0010 0110
		ENTRY(INVALID, dummy_func),					// 0010 0111
		ENTRY(INVALID, dummy_func),					// 0010 1000
		ENTRY(ADC,     adc_func),					// 0010 1001
		ENTRY(INVALID, dummy_func),					// 0010 1010
		ENTRY(INVALID, dummy_func),					// 0010 1011
		ENTRY(INVALID, dummy_func),					// 0010 1100
		ENTRY(INVALID, dummy_func),					// 0010 1101
		ENTRY(ABS,     abs_func),					// 0010 1110
		ENTRY(INVALID, dummy_func),					// 0010 1111

		// 3x
		ENTRY(INVALID, dummy_func),					// 0011 0000
		ENTRY(ADC,     adc_func),					// 0011 0001
		ENTRY(INVALID, dummy_func),					// 0011 0010
		ENTRY(INVALID, dummy_func),					// 0011 0011
		ENTRY(INVALID, dummy_func),					// 0011 0100
		ENTRY(INVALID, dummy_func),					// 0011 0101
		ENTRY(INVALID, dummy_func),					// 0011 0110
		ENTRY(INVALID, dummy_func),					// 0011 0111
		ENTRY(INVALID, dummy_func),					// 0011 1000
		ENTRY(ADC,     adc_func),					// 0011 1001
		ENTRY(INVALID, dummy_func),					// 0011 1010
		ENTRY(INVALID, dummy_func),					// 0011 1011
		ENTRY(INVALID, dummy_func),					// 0011 1100
		ENTRY(INVALID, dummy_func),					// 0011 1101
		ENTRY(INVALID, dummy_func),					// 0011 1110
		ENTRY(INVALID, dummy_func),					// 0011 1111

		// 4x
		ENTRY(INVALID, dummy_func),					// 0100 0000
		ENTRY(INVALID, dummy_func),					// 0100 0001
		ENTRY(INVALID, dummy_func),					// 0100 0010
		ENTRY(INVALID, dummy_func),					// 0100 0011
		ENTRY(INVALID, dummy_func),					// 0100 0100
		ENTRY(INVALID, dummy_func),					// 0100 0101
		ENTRY(INVALID, dummy_func),					// 0100 0110
		ENTRY(INVALID, dummy_func),					// 0100 0111
		ENTRY(INVALID, dummy_func),					// 0100 1000
		ENTRY(INVALID, dummy_func),					// 0100 1001
		ENTRY(INVALID, dummy_func),					// 0100 1010
		ENTRY(INVALID, dummy_func),					// 0100 1011
		ENTRY(INVALID, dummy_func),					// 0100 1100
		ENTRY(INVALID, dummy_func),					// 0100 1101
		ENTRY(INVALID, dummy_func),					// 0100 1110
		ENTRY(INVALID, dummy_func),					// 0100 1111

		// 5x
		ENTRY(INVALID, dummy_func),					// 0101 0000
		ENTRY(INVALID, dummy_func),					// 0101 0001
		ENTRY(INVALID, dummy_func),					// 0101 0010
		ENTRY(INVALID, dummy_func),					// 0101 0011
		ENTRY(INVALID, dummy_func),					// 0101 0100
		ENTRY(INVALID, dummy_func),					// 0101 0101
		ENTRY(INVALID, dummy_func),					// 0101 0110
		ENTRY(INVALID, dummy_func),					// 0101 0111
		ENTRY(INVALID, dummy_func),					// 0101 1000
		ENTRY(INVALID, dummy_func),					// 0101 1001
		ENTRY(INVALID, dummy_func),					// 0101 1010
		ENTRY(INVALID, dummy_func),					// 0101 1011
		ENTRY(INVALID, dummy_func),					// 0101 1100
		ENTRY(INVALID, dummy_func),					// 0101 1101
		ENTRY(INVALID, dummy_func),					// 0101 1110
		ENTRY(INVALID, dummy_func),					// 0101 1111

		// 6x
		ENTRY(INVALID, dummy_func),					// 0110 0000
		ENTRY(INVALID, dummy_func),					// 0110 0001
		ENTRY(INVALID, dummy_func),					// 0110 0010
		ENTRY(INVALID, dummy_func),					// 0110 0011
		ENTRY(INVALID, dummy_func),					// 0110 0100
		ENTRY(INVALID, dummy_func),					// 0110 0101
		ENTRY(INVALID, dummy_func),					// 0110 0110
		ENTRY(INVALID, dummy_func),					// 0110 0111
		ENTRY(INVALID, dummy_func),					// 0110 1000
		ENTRY(INVALID, dummy_func),					// 0110 1001
		ENTRY(INVALID, dummy_func),					// 0110 1010
		ENTRY(INVALID, dummy_func),					// 0110 1011
		ENTRY(INVALID, dummy_func),					// 0110 1100
		ENTRY(INVALID, dummy_func),					// 0110 1101
		ENTRY(INVALID, dummy_func),					// 0110 1110
		ENTRY(INVALID, dummy_func),					// 0110 1111

		// 7x
		ENTRY(INVALID, dummy_func),					// 0111 0000
		ENTRY(INVALID, dummy_func),					// 0111 0001
		ENTRY(INVALID, dummy_func),					// 0111 0010
		ENTRY(INVALID, dummy_func),					// 0111 0011
		ENTRY(INVALID, dummy_func),					// 0111 0100
		ENTRY(INVALID, dummy_func),					// 0111 0101
		ENTRY(INVALID, dummy_func),					// 0111 0110
		ENTRY(INVALID, dummy_func),					// 0111 0111
		ENTRY(INVALID, dummy_func),					// 0111 1000
		ENTRY(INVALID, dummy_func),					// 0111 1001
		ENTRY(INVALID, dummy_func),					// 0111 1010
		ENTRY(INVALID, dummy_func),					// 0111 1011
		ENTRY(INVALID, dummy_func),					// 0111 1100
		ENTRY(INVALID, dummy_func),					// 0111 1101
		ENTRY(INVALID, dummy_func),					// 0111 1110
		ENTRY(INVALID, dummy_func),					// 0111 1111

		// 8x
		ENTRY(INVALID, dummy_func),					// 1000 0000
		ENTRY(INVALID, dummy_func),					// 1000 0001
		ENTRY(INVALID, dummy_func),					// 1000 0010
		ENTRY(INVALID, dummy_func),					// 1000 0011
		ENTRY(INVALID, dummy_func),					// 1000 0100
		ENTRY(INVALID, dummy_func),					// 1000 0101
		ENTRY(INVALID, dummy_func),					// 1000 0110
		ENTRY(INVALID, dummy_func),					// 1000 0111
		ENTRY(INVALID, dummy_func),					// 1000 1000
		ENTRY(INVALID, dummy_func),					// 1000 1001
		ENTRY(INVALID, dummy_func),					// 1000 1010
		ENTRY(INVALID, dummy_func),					// 1000 1011
		ENTRY(INVALID, dummy_func),					// 1000 1100
		ENTRY(INVALID, dummy_func),					// 1000 1101
		ENTRY(INVALID, dummy_func),					// 1000 1110
		ENTRY(INVALID, dummy_func),					// 1000 1111

		// 9x
		ENTRY(INVALID, dummy_func),					// 1001 0000
		ENTRY(INVALID, dummy_func),					// 1001 0001
		ENTRY(INVALID, dummy_func),					// 1001 0010
		ENTRY(INVALID, dummy_func),					// 1001 0011
		ENTRY(INVALID, dummy_func),					// 1001 0100
		ENTRY(INVALID, dummy_func),					// 1001 0101
		ENTRY(INVALID, dummy_func),					// 1001 0110
		ENTRY(INVALID, dummy_func),					// 1001 0111
		ENTRY(INVALID, dummy_func),					// 1001 1000
		ENTRY(INVALID, dummy_func),					// 1001 1001
		ENTRY(INVALID, dummy_func),					// 1001 1010
		ENTRY(INVALID, dummy_func),					// 1001 1011
		ENTRY(INVALID, dummy_func),					// 1001 1100
		ENTRY(INVALID, dummy_func),					// 1001 1101
		ENTRY(INVALID, dummy_func),					// 1001 1110
		ENTRY(INVALID, dummy_func),					// 1001 1111

		// Ax
		ENTRY(INVALID, dummy_func),					// 1010 0000
		ENTRY(INVALID, dummy_func),					// 1010 0001
		ENTRY(INVALID, dummy_func),					// 1010 0010
		ENTRY(INVALID, dummy_func),					// 1010 0011
		ENTRY(INVALID, dummy_func),					// 1010 0100
		ENTRY(INVALID, dummy_func),					// 1010 0101
		ENTRY(INVALID, dummy_func),					// 1010 0110
		ENTRY(INVALID, dummy_func),					// 1010 0111
		ENTRY(INVALID, dummy_func),					// 1010 1000
		ENTRY(INVALID, dummy_func),					// 1010 1001
		ENTRY(INVALID, dummy_func),					// 1010 1010
		ENTRY(INVALID, dummy_func),					// 1010 1011
		ENTRY(INVALID, dummy_func),					// 1010 1100
		ENTRY(INVALID, dummy_func),					// 1010 1101
		ENTRY(INVALID, dummy_func),					// 1010 1110
		ENTRY(INVALID, dummy_func),					// 1010 1111

		// Bx
		ENTRY(INVALID, dummy_func),					// 1011 0000
		ENTRY(INVALID, dummy_func),					// 1011 0001
		ENTRY(INVALID, dummy_func),					// 1011 0010
		ENTRY(INVALID, dummy_func),					// 1011 0011
		ENTRY(INVALID, dummy_func),					// 1011 0100
		ENTRY(INVALID, dummy_func),					// 1011 0101
		ENTRY(INVALID, dummy_func),					// 1011 0110
		ENTRY(INVALID, dummy_func),					// 1011 0111
		ENTRY(INVALID, dummy_func),					// 1011 1000
		ENTRY(INVALID, dummy_func),					// 1011 1001
		ENTRY(INVALID, dummy_func),					// 1011 1010
		ENTRY(INVALID, dummy_func),					// 1011 1011
		ENTRY(INVALID, dummy_func),					// 1011 1100
		ENTRY(INVALID, dummy_func),					// 1011 1101
		ENTRY(INVALID, dummy_func),					// 1011 1110
		ENTRY(INVALID, dummy_func),					// 1011 1111

		// Cx
		ENTRY(INVALID, dummy_func),					// 1100 0000
		ENTRY(INVALID, dummy_func),					// 1100 0001
		ENTRY(INVALID, dummy_func),					// 1100 0010
		ENTRY(INVALID, dummy_func),					// 1100 0011
		ENTRY(INVALID, dummy_func),					// 1100 0100
		ENTRY(INVALID, dummy_func),					// 1100 0101
		ENTRY(INVALID, dummy_func),					// 1100 0110
		ENTRY(INVALID, dummy_func),					// 1100 0111
		ENTRY(INVALID, dummy_func),					// 1100 1000
		ENTRY(INVALID, dummy_func),					// 1100 1001
		ENTRY(INVALID, dummy_func),					// 1100 1010
		ENTRY(INVALID, dummy_func),					// 1100 1011
		ENTRY(INVALID, dummy_func),					// 1100 1100
		ENTRY(INVALID, dummy_func),					// 1100 1101
		ENTRY(INVALID, dummy_func),					// 1100 1110
		ENTRY(INVALID, dummy_func),					// 1100 1111

		// Dx
		ENTRY(INVALID, dummy_func),					// 1101 0000
		ENTRY(INVALID, dummy_func),					// 1101 0001
		ENTRY(INVALID, dummy_func),					// 1101 0010
		ENTRY(INVALID, dummy_func),					// 1101 0011
		ENTRY(INVALID, dummy_func),					// 1101 0100
		ENTRY(INVALID, dummy_func),					// 1101 0101
		ENTRY(INVALID, dummy_func),					// 1101 0110
		ENTRY(INVALID, dummy_func),					// 1101 0111
		ENTRY(INVALID, dummy_func),					// 1101 1000
		ENTRY(INVALID, dummy_func),					// 1101 1001
		ENTRY(INVALID, dummy_func),					// 1101 1010
		ENTRY(INVALID, dummy_func),					// 1101 1011
		ENTRY(INVALID, dummy_func),					// 1101 1100
		ENTRY(INVALID, dummy_func),					// 1101 1101
		ENTRY(INVALID, dummy_func),					// 1101 1110
		ENTRY(INVALID, dummy_func),					// 1101 1111

		// Ex
		ENTRY(INVALID, dummy_func),					// 1110 0000
		ENTRY(INVALID, dummy_func),					// 1110 0001
		ENTRY(INVALID, dummy_func),					// 1110 0010
		ENTRY(INVALID, dummy_func),					// 1110 0011
		ENTRY(INVALID, dummy_func),					// 1110 0100
		ENTRY(INVALID, dummy_func),					// 1110 0101
		ENTRY(INVALID, dummy_func),					// 1110 0110
		ENTRY(INVALID, dummy_func),					// 1110 0111
		ENTRY(INVALID, dummy_func),					// 1110 1000
		ENTRY(INVALID, dummy_func),					// 1110 1001
		ENTRY(INVALID, dummy_func),					// 1110 1010
		ENTRY(INVALID, dummy_func),					// 1110 1011
		ENTRY(INVALID, dummy_func),					// 1110 1100
		ENTRY(INVALID, dummy_func),					// 1110 1101
		ENTRY(INVALID, dummy_func),					// 1110 1110
		ENTRY(INVALID, dummy_func),					// 1110 1111

		// Fx
		ENTRY(INVALID, dummy_func),					// 1111 0000
		ENTRY(INVALID, dummy_func),					// 1111 0001
		ENTRY(INVALID, dummy_func),					// 1111 0010
		ENTRY(INVALID, dummy_func),					// 1111 0011
		ENTRY(INVALID, dummy_func),					// 1111 0100
		ENTRY(INVALID, dummy_func),					// 1111 0101
		ENTRY(INVALID, dummy_func),					// 1111 0110
		ENTRY(INVALID, dummy_func),					// 1111 0111
		ENTRY(INVALID, dummy_func),					// 1111 1000
		ENTRY(INVALID, dummy_func),					// 1111 1001
		ENTRY(INVALID, dummy_func),					// 1111 1010
		ENTRY(INVALID, dummy_func),					// 1111 1011
		ENTRY(INVALID, dummy_func),					// 1111 1100
		ENTRY(INVALID, dummy_func),					// 1111 1101
		ENTRY(INVALID, dummy_func),					// 1111 1110
		ENTRY(INVALID, dummy_func),					// 1111 1111
	};


	int decode(instruction& inst, buffer_reader& buf, const decode_settings& settings)
	{
		inst.reset();
		buffer_reader buf_copy = buf;

		if (buf.read_word(inst.header))
			return 1;

		// Main instruction type is stored in the lower 8 bits
		uint8_t base_type = inst.header & 0xff;
		const op_entry& entry = entries[base_type];

		inst.opcode = entry.opcode;
		inst.word_count = 1;		// default values
		int ret = entry.func(inst, inst.header, settings, buf);
		if (ret == 0)
		{
			inst.word_count = buf.get_pos() - buf_copy.get_pos();
			return ret;
		}
		return ret;
	}
}

