#include "decode.h"

#include <stdio.h>
#include "buffer.h"
#include "instruction.h"
#define ENTRY(opcode, func)		{ instruction::Opcode::opcode, func }

namespace hop56
{
	// Converts bit IDs in the parallel moves, to full register IDs
	static Reg pmove_registers_1[32] =
	{
		Reg::NONE, Reg::NONE,	Reg::NONE, Reg::NONE,
		Reg::X0, Reg::X1, Reg::Y0, Reg::Y1,
		Reg::A0, Reg::B0,
		Reg::A2, Reg::B2,
		Reg::A1, Reg::B1,
		Reg::A, Reg::B,
	 	Reg::R0, Reg::R1, Reg::R2, Reg::R3,
		Reg::R4, Reg::R5, Reg::R6, Reg::R7,
		Reg::N0, Reg::N1, Reg::N2, Reg::N3,
		Reg::N4, Reg::N5, Reg::N6, Reg::N7
	};
	static Reg pmove_registers_bank_x[4] =
	{
		Reg::X0, Reg::X1, Reg::A, Reg::B
	};
	static Reg pmove_registers_bank_y[4] =
	{
		Reg::Y0, Reg::Y1, Reg::A, Reg::B
	};
	static Reg pmove_registers_movel[8] =
	{
		Reg::A10, Reg::B10,
		Reg::X, Reg::Y, Reg::A, Reg::B,
		Reg::AB, Reg::BA,
	};

	struct op_entry
	{
		instruction::Opcode opcode;
		int (*func)(instruction& inst, uint32_t header, const decode_settings&, buffer_reader& buf);	// decoder
	};

	// ========================================================================
	// Setters for operands

	// Create R-register from 0-7 index
	Reg make_r(uint32_t r_num) { return (Reg)(Reg::R0 + r_num); }
	// Create N-register from 0-7 index
	Reg make_n(uint32_t n_num) { return (Reg)(Reg::N0 + n_num); }

	static void set_reg(operand& op, Reg reg)
	{
		op.type = operand::REG; op.reg.index = reg;
	}
	static void set_imm_short(operand& op, int8_t val)
	{
		op.type = operand::IMM_SHORT; op.imm_short.val = val;
	}
	static void set_postdec_offset(operand& op, uint32_t reg_num)
	{
		op.type = operand::POSTDEC_OFFSET;
		op.postdec_offset.index_1 = make_r(reg_num);
		op.postdec_offset.index_2 = make_n(reg_num);
	}
	static void set_postinc_offset(operand& op, uint32_t reg_num)
	{
		op.type = operand::POSTINC_OFFSET;
		op.postinc_offset.index_1 = make_r(reg_num);
		op.postinc_offset.index_2 = make_n(reg_num);
	}
	static void set_postdec(operand& op, uint32_t reg_num)
	{
		op.type = operand::POSTDEC; op.postdec.index = make_r(reg_num);
	}
	static void set_postinc(operand& op, uint32_t reg_num)
	{
		op.type = operand::POSTINC; op.postdec.index = make_r(reg_num);
	}
	static void set_index_offset(operand& op, uint32_t reg_num)
	{
		op.type = operand::INDEX_OFFSET;
		op.postinc_offset.index_1 = make_r(reg_num);
		op.postinc_offset.index_2 = make_n(reg_num);
	}
	static void set_no_update(operand& op, uint32_t reg_num)
	{
		op.type = operand::NO_UPDATE; op.no_update.index = make_r(reg_num);
	}
	static void set_predec(operand& op, uint32_t reg_num)
	{
		op.type = operand::PREDEC; op.predec.index = make_r(reg_num);
	}
	static void set_abs(operand& op, uint32_t addr)
	{
		op.type = operand::ABS; op.abs.address = addr;
	}
	static void set_imm(operand& op, uint32_t val)
	{
		op.type = operand::IMM; op.imm.val = val;
 	}

	static void allocate_operands(pmove& pmove, uint32_t w, operand*& opA, operand*& opB)
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

	// Speicifies the set of allowable addressing modes in parallel moves.
	enum EA_MODE { EA_MODE_ALL, EA_MODE_WRITE, EA_MODE_SHORT, EA_MODE_MOVEL, EA_MODE_COUNT };

	// Handle "mmm_rrr" addressing mode and register for Rn/abs/immediate addressing in parallel moves
	static int decode_mmmrrr(operand& op, Memory mem, uint32_t mmm, uint32_t rrr, EA_MODE mode, buffer_reader& buf)
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
				if ((rrr == 0x4) && (mode == EA_MODE_ALL))
				{
					// Immediate value in next word -- can't write
					if (buf.read_word(next))
						return 1;
					set_imm(op, next);
					op.memory = Memory::MEM_NONE;
					return 0;
				}
				else if ((rrr == 0x0) &&
							((mode == EA_MODE_ALL) ||
							 (mode == EA_MODE_WRITE) ||
							 (mode == EA_MODE_MOVEL)))
				{
					// Absolute address write
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

	// Decode short-format "mm rrr" fields used in XY parallel data moves.
	static int decode_mmrrr(operand& op, Memory mem, uint32_t mm, uint32_t rrr, buffer_reader& buf)
	{
		op.memory = mem;
		switch (mm)
		{
			case 0:		set_index_offset(op, rrr); return 0;
			case 1:		set_postinc_offset(op, rrr); return 0;
			case 2:		set_postdec(op, rrr); return 0;
			case 3:		set_postinc(op, rrr); return 0;
		}
		return 1;
	}

	// Decode all parallel moves in an instruction.
	static int decode_pmove(instruction& inst, uint32_t header, const decode_settings&, buffer_reader& buf)
	{
		uint32_t pdata = (header >> 8) & 0xffff;

		// UM, page A-162 onwards

		// Pre-choose operand order for read vs write
		uint32_t w = (pdata >> 7) & 0x1;

		// Pre-choose modes and registers
		uint32_t mmm = (pdata >> 3) & 0x7;
		uint32_t rrr = (pdata >> 0) & 0x7;

		// Prep the most commonly-used operand slots/arguments
		operand& op0 = inst.pmoves[0].operands[0];
		operand& op1 = inst.pmoves[0].operands[1];
		operand* opA, * opB;
		EA_MODE default_mode = w ? EA_MODE_ALL : EA_MODE_WRITE;	// w==0 means "reading from reg, and writing to memory"

		if (pdata == 0x2000)
			return 0;			// No Parallel Move
		if ((pdata & 0xffe0) == 0x2040)
		{
			// Address Reg Update (one operand)
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
		if ((pdata & 0xf440) == 0x4040)
		{
			// L: Memory move
			// 0100 L0LL W1MM MRRR format
			// 1111 0100 0100 0000 mask
			// 0100 0000 0100 0000 val
			allocate_operands(inst.pmoves[1], w, opA, opB);
			uint32_t LLL = ((pdata >> 8) & 0x3) | ((pdata >> 9) & 0x4);
			set_reg(*opB, pmove_registers_movel[LLL]);
			return decode_mmmrrr(*opA, MEM_L, mmm, rrr, EA_MODE_MOVEL, buf);
		}
		if ((pdata & 0xf040) == 0x1000)
		{
			// X: Memory and Reg Move, Class I
			// 0001 ffdf W0MM MRRR format
			// 1111 0000 0100 0000 mask
			// 0001 0000 0000 0000 val
			allocate_operands(inst.pmoves[0], w, opA, opB);
			uint32_t f = (pdata >> 8) & 0x1;		// S2/D2
			uint32_t d = (pdata >> 9) & 0x1;
		   	uint32_t ff = (pdata >> 10) & 0x3;		// S1/D1
			set_reg(*opB, pmove_registers_bank_x[ff]);
			if (decode_mmmrrr(*opA, MEM_X, mmm, rrr, default_mode, buf))	// TODO check since UM suggests SHORT modes only
				return 1;
			// Second pmove is limited
			set_reg(inst.pmoves[1].operands[0], d ? Reg::B  : Reg::A);
			set_reg(inst.pmoves[1].operands[1], f ? Reg::Y1 : Reg::Y0);
			return 0;
		}
		if ((pdata & 0xf040) == 0x1040)
		{
			// Y: Reg and Memory Move, Class I
			// 0001 deff W1MM MRRR format
			// 1111 0000 0100 0000 mask
			// 0001 0000 0100 0000 val
			allocate_operands(inst.pmoves[1], w, opA, opB);
		   	uint32_t ff = (pdata >> 8) & 0x3;		// S1/D1
			uint32_t e = (pdata >> 10) & 0x1;		// S2/D2
			uint32_t d = (pdata >> 11) & 0x1;
			set_reg(*opB, pmove_registers_bank_y[ff]);
			if (decode_mmmrrr(*opA, MEM_Y, mmm, rrr,  default_mode, buf))	// TODO check since UM suggests SHORT modes only
				return 1;
			// Second pmove is limited
			set_reg(inst.pmoves[0].operands[0], d ? Reg::B  : Reg::A);
			set_reg(inst.pmoves[0].operands[1], e ? Reg::X1 : Reg::X0);
			return 0;
		}
		if ((pdata & 0xfec0) == 0x0800)
		{
			// X: Memory and Reg Move, Class II
			// 0000 100d 00MM MRRR format
			// 1111 1110 1100 0000 mask
			// 0000 1000 0000 0000 val
			uint32_t d = (pdata >> 8) & 0x1;
			Reg accum_reg = d ? Reg::B : Reg::A;
			set_reg(op0, accum_reg);
			if (decode_mmmrrr(op1, MEM_X, mmm, rrr, EA_MODE_SHORT, buf))		// this is limited to short modes
				return 1;
			// Second pmove is always X0
			set_reg(inst.pmoves[1].operands[0], Reg::X0);
		   	set_reg(inst.pmoves[1].operands[1], accum_reg);
			printf("X Class II\n");
			return 0;
		}
		if ((pdata & 0xfec0) == 0x0880)
		{
			// Y: Reg and Memory Move, Class II
			// 0000 100d 10MM MRRR format
			// 1111 1110 1100 0000 mask
			// 0000 1000 1000 0000 val
			uint32_t d = (pdata >> 8) & 0x1;
			Reg accum_reg = d ? Reg::B : Reg::A;
			set_reg(inst.pmoves[1].operands[0], accum_reg);
			if (decode_mmmrrr(inst.pmoves[1].operands[1], MEM_Y, mmm, rrr, EA_MODE_SHORT, buf))		// this is limited to short modes
				return 1;
			// Second pmove is always Y0
			set_reg(inst.pmoves[0].operands[0], Reg::Y0);
		   	set_reg(inst.pmoves[0].operands[1], accum_reg);
			printf("Y Class II\n");
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
			return decode_mmmrrr(*opA, MEM_X, mmm, rrr, default_mode, buf);
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
			return decode_mmmrrr(*opA, MEM_Y, mmm, rrr, default_mode, buf);
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
		if ((pdata & 0xc8000) == 0x8000)
		{
			// XY Memory Data move
			// 1wmm eeff WrrM MRRR format
			// 1000 0000 0000 0000 mask
			// 1000 0000 0000 0000 val
			// X memory:
			uint32_t MM = (pdata >> 3) & 0x3;
			uint32_t ee = (pdata >> 10) & 0x3;
			allocate_operands(inst.pmoves[0], w, opA, opB);
			set_reg(*opB, pmove_registers_bank_x[ee]);
			if (decode_mmrrr(*opA, MEM_X, MM, rrr, buf))
				return 1;
			// Y memory:
			uint32_t mm = (pdata >> 12) & 0x3;
			uint32_t rr = (pdata >> 5) & 0x3;
			uint32_t ff = (pdata >> 8) & 0x3;
			rrr &= 4;					// isolate memory bank base (R0 or R4)
			rrr ^= 4;					// switch memory bank base
			rrr |= rr;
			w = (pdata >> 14) & 0x1;
			allocate_operands(inst.pmoves[1], w, opA, opB);
			set_reg(*opB, pmove_registers_bank_y[ff]);
			if (decode_mmrrr(*opA, MEM_Y, mm, rrr, buf))
				return 1;
		}
		return 0;
	}

	// ========================================================================
	//	PARALLEL MOVE OPCODE FUNCTIONS
	// ========================================================================
	static int dummy(instruction& inst, uint32_t header, const decode_settings&, buffer_reader& buf)
	{
		return 1;
	}

	static int alu_d(instruction& inst, uint32_t header, const decode_settings& settings, buffer_reader& buf)
	{
		uint32_t a_or_b = (header >> 3) & 1;
		inst.operands[0].type = operand::REG;
		inst.operands[0].reg.index = a_or_b ? Reg::B : Reg::A;
		return decode_pmove(inst, header, settings, buf);
	}

	static int adc(instruction& inst, uint32_t header, const decode_settings& settings, buffer_reader& buf)
	{
		uint32_t x_or_y = (header >> 4) & 1;
		set_reg(inst.operands[0], x_or_y ? Reg::Y : Reg::X);
		uint32_t a_or_b = (header >> 3) & 1;
		set_reg(inst.operands[1], a_or_b ? Reg::B : Reg::A);
		return decode_pmove(inst, header, settings, buf);
	}

	static int alu_JJJd(instruction& inst, uint32_t header, const decode_settings& settings, buffer_reader& buf)
	{
		uint32_t JJJd = (header >> 3) & 0xf;
		uint32_t d = (header >> 4) & 0x1;
		static Reg sources[16] =
		{
			Reg::NONE, Reg::NONE, Reg::B,  Reg::A,  Reg::X,  Reg::X,  Reg::Y,  Reg::Y,
			Reg::X0,   Reg::X0,   Reg::Y0, Reg::Y0, Reg::X1, Reg::X1, Reg::Y1, Reg::Y1
		};
		set_reg(inst.operands[0], sources[JJJd]);
		set_reg(inst.operands[1], d ? Reg::B : Reg::A);
		if (sources[JJJd] == Reg::NONE)
		   return 1;
		return decode_pmove(inst, header, settings, buf);
	}

	static int alu_JJd(instruction& inst, uint32_t header, const decode_settings& settings, buffer_reader& buf)
	{
		uint32_t JJ = (header >> 4) & 0x3;
		uint32_t d = (header >> 3) & 0x1;
		static Reg sources[4] = 		{ Reg::X0,   Reg::Y0, Reg::X1, Reg::Y1 };
		set_reg(inst.operands[0], sources[JJ]);
		set_reg(inst.operands[1], d ? Reg::B : Reg::A);
		return decode_pmove(inst, header, settings, buf);
	}

	static int addl(instruction& inst, uint32_t header, const decode_settings& settings, buffer_reader& buf)
	{
		uint32_t d = (header >> 3) & 0x1;
		set_reg(inst.operands[0], d ? Reg::A : Reg::B);
		set_reg(inst.operands[1], d ? Reg::B : Reg::A);
		return decode_pmove(inst, header, settings, buf);
	}

	// ========================================================================
	//	OPCODE TABLE FOR PARALLEL MOVE INSTRUCTIONS
	// ========================================================================
	static const op_entry pm_entries[256] =
	{
		// 0x
		ENTRY(ADD,     alu_JJJd),				// 0000 0000
		ENTRY(INVALID, dummy),					// 0000 0001
		ENTRY(ADDR,    addl),					// 0000 0010
		ENTRY(INVALID, dummy),					// 0000 0011
		ENTRY(INVALID, dummy),					// 0000 0100
		ENTRY(INVALID, dummy),					// 0000 0101
		ENTRY(INVALID, dummy),					// 0000 0110
		ENTRY(INVALID, dummy),					// 0000 0111
		ENTRY(ADD,     alu_JJJd),				// 0000 1000
		ENTRY(INVALID, dummy),					// 0000 1001
		ENTRY(ADDR,    addl),					// 0000 1010
		ENTRY(INVALID, dummy),					// 0000 1011
		ENTRY(INVALID, dummy),					// 0000 1100
		ENTRY(INVALID, dummy),					// 0000 1101
		ENTRY(INVALID, dummy),					// 0000 1110
		ENTRY(INVALID, dummy),					// 0000 1111

		// 1x
		ENTRY(ADD,     alu_JJJd),				// 0001 0000
		ENTRY(INVALID, dummy),					// 0001 0001
		ENTRY(ADDL,    addl),					// 0001 0010
		ENTRY(INVALID, dummy),					// 0001 0011
		ENTRY(INVALID, dummy),					// 0001 0100
		ENTRY(INVALID, dummy),					// 0001 0101
		ENTRY(INVALID, dummy),					// 0001 0110
		ENTRY(INVALID, dummy),					// 0001 0111
		ENTRY(ADD,     alu_JJJd),				// 0001 1000
		ENTRY(INVALID, dummy),					// 0001 1001
		ENTRY(ADDL,    addl),					// 0001 1010
		ENTRY(INVALID, dummy),					// 0001 1011
		ENTRY(INVALID, dummy),					// 0001 1100
		ENTRY(INVALID, dummy),					// 0001 1101
		ENTRY(INVALID, dummy),					// 0001 1110
		ENTRY(INVALID, dummy),					// 0001 1111

		// 2x
		ENTRY(ADD,     alu_JJJd),				// 0010 0000
		ENTRY(ADC,     adc),					// 0010 0001
		ENTRY(ASR,     alu_d),					// 0010 0010
		ENTRY(INVALID, dummy),					// 0010 0011
		ENTRY(INVALID, dummy),					// 0010 0100
		ENTRY(INVALID, dummy),					// 0010 0101
		ENTRY(ABS,     alu_d),					// 0010 0110
		ENTRY(INVALID, dummy),					// 0010 0111
		ENTRY(ADD,     alu_JJJd),				// 0010 1000
		ENTRY(ADC,     adc),					// 0010 1001
		ENTRY(ASR,     alu_d),					// 0010 1010
		ENTRY(INVALID, dummy),					// 0010 1011
		ENTRY(INVALID, dummy),					// 0010 1100
		ENTRY(INVALID, dummy),					// 0010 1101
		ENTRY(ABS,     alu_d),					// 0010 1110
		ENTRY(INVALID, dummy),					// 0010 1111

		// 3x
		ENTRY(ADD,     alu_JJJd),				// 0011 0000
		ENTRY(ADC,     adc),					// 0011 0001
		ENTRY(ASL,     alu_d),					// 0011 0010
		ENTRY(INVALID, dummy),					// 0011 0011
		ENTRY(INVALID, dummy),					// 0011 0100
		ENTRY(INVALID, dummy),					// 0011 0101
		ENTRY(INVALID, dummy),					// 0011 0110
		ENTRY(INVALID, dummy),					// 0011 0111
		ENTRY(ADD,     alu_JJJd),				// 0011 1000
		ENTRY(ADC,     adc),					// 0011 1001
		ENTRY(ASL,     alu_d),					// 0011 1010
		ENTRY(INVALID, dummy),					// 0011 1011
		ENTRY(INVALID, dummy),					// 0011 1100
		ENTRY(INVALID, dummy),					// 0011 1101
		ENTRY(INVALID, dummy),					// 0011 1110
		ENTRY(INVALID, dummy),					// 0011 1111

		// 4x
		ENTRY(ADD,     alu_JJJd),				// 0100 0000
		ENTRY(INVALID, dummy),					// 0100 0001
		ENTRY(INVALID, dummy),					// 0100 0010
		ENTRY(INVALID, dummy),					// 0100 0011
		ENTRY(INVALID, dummy),					// 0100 0100
		ENTRY(INVALID, dummy),					// 0100 0101
		ENTRY(AND,     alu_JJd),				// 0100 0110
		ENTRY(INVALID, dummy),					// 0100 0111
		ENTRY(ADD,     alu_JJJd),				// 0100 1000
		ENTRY(INVALID, dummy),					// 0100 1001
		ENTRY(INVALID, dummy),					// 0100 1010
		ENTRY(INVALID, dummy),					// 0100 1011
		ENTRY(INVALID, dummy),					// 0100 1100
		ENTRY(INVALID, dummy),					// 0100 1101
		ENTRY(AND,     alu_JJd),				// 0100 1110
		ENTRY(INVALID, dummy),					// 0100 1111

		// 5x
		ENTRY(ADD,     alu_JJJd),				// 0101 0000
		ENTRY(INVALID, dummy),					// 0101 0001
		ENTRY(INVALID, dummy),					// 0101 0010
		ENTRY(INVALID, dummy),					// 0101 0011
		ENTRY(INVALID, dummy),					// 0101 0100
		ENTRY(INVALID, dummy),					// 0101 0101
		ENTRY(AND,     alu_JJd),				// 0101 0110
		ENTRY(INVALID, dummy),					// 0101 0111
		ENTRY(ADD,     alu_JJJd),				// 0101 1000
		ENTRY(INVALID, dummy),					// 0101 1001
		ENTRY(INVALID, dummy),					// 0101 1010
		ENTRY(INVALID, dummy),					// 0101 1011
		ENTRY(INVALID, dummy),					// 0101 1100
		ENTRY(INVALID, dummy),					// 0101 1101
		ENTRY(AND,     alu_JJd),				// 0101 1110
		ENTRY(INVALID, dummy),					// 0101 1111

		// 6x
		ENTRY(ADD,     alu_JJJd),				// 0110 0000
		ENTRY(INVALID, dummy),					// 0110 0001
		ENTRY(INVALID, dummy),					// 0110 0010
		ENTRY(INVALID, dummy),					// 0110 0011
		ENTRY(INVALID, dummy),					// 0110 0100
		ENTRY(INVALID, dummy),					// 0110 0101
		ENTRY(AND,     alu_JJd),				// 0110 0110
		ENTRY(INVALID, dummy),					// 0110 0111
		ENTRY(ADD,     alu_JJJd),				// 0110 1000
		ENTRY(INVALID, dummy),					// 0110 1001
		ENTRY(INVALID, dummy),					// 0110 1010
		ENTRY(INVALID, dummy),					// 0110 1011
		ENTRY(INVALID, dummy),					// 0110 1100
		ENTRY(INVALID, dummy),					// 0110 1101
		ENTRY(AND,     alu_JJd),				// 0110 1110
		ENTRY(INVALID, dummy),					// 0110 1111

		// 7x
		ENTRY(ADD,     alu_JJJd),				// 0111 0000
		ENTRY(INVALID, dummy),					// 0111 0001
		ENTRY(INVALID, dummy),					// 0111 0010
		ENTRY(INVALID, dummy),					// 0111 0011
		ENTRY(INVALID, dummy),					// 0111 0100
		ENTRY(INVALID, dummy),					// 0111 0101
		ENTRY(AND,     alu_JJd),				// 0111 0110
		ENTRY(INVALID, dummy),					// 0111 0111
		ENTRY(ADD,     alu_JJJd),				// 0111 1000
		ENTRY(INVALID, dummy),					// 0111 1001
		ENTRY(INVALID, dummy),					// 0111 1010
		ENTRY(INVALID, dummy),					// 0111 1011
		ENTRY(INVALID, dummy),					// 0111 1100
		ENTRY(INVALID, dummy),					// 0111 1101
		ENTRY(AND,     alu_JJd),				// 0111 1110
		ENTRY(INVALID, dummy),					// 0111 1111

		// 8x
		ENTRY(INVALID, dummy),					// 1000 0000
		ENTRY(INVALID, dummy),					// 1000 0001
		ENTRY(INVALID, dummy),					// 1000 0010
		ENTRY(INVALID, dummy),					// 1000 0011
		ENTRY(INVALID, dummy),					// 1000 0100
		ENTRY(INVALID, dummy),					// 1000 0101
		ENTRY(INVALID, dummy),					// 1000 0110
		ENTRY(INVALID, dummy),					// 1000 0111
		ENTRY(INVALID, dummy),					// 1000 1000
		ENTRY(INVALID, dummy),					// 1000 1001
		ENTRY(INVALID, dummy),					// 1000 1010
		ENTRY(INVALID, dummy),					// 1000 1011
		ENTRY(INVALID, dummy),					// 1000 1100
		ENTRY(INVALID, dummy),					// 1000 1101
		ENTRY(INVALID, dummy),					// 1000 1110
		ENTRY(INVALID, dummy),					// 1000 1111

		// 9x
		ENTRY(INVALID, dummy),					// 1001 0000
		ENTRY(INVALID, dummy),					// 1001 0001
		ENTRY(INVALID, dummy),					// 1001 0010
		ENTRY(INVALID, dummy),					// 1001 0011
		ENTRY(INVALID, dummy),					// 1001 0100
		ENTRY(INVALID, dummy),					// 1001 0101
		ENTRY(INVALID, dummy),					// 1001 0110
		ENTRY(INVALID, dummy),					// 1001 0111
		ENTRY(INVALID, dummy),					// 1001 1000
		ENTRY(INVALID, dummy),					// 1001 1001
		ENTRY(INVALID, dummy),					// 1001 1010
		ENTRY(INVALID, dummy),					// 1001 1011
		ENTRY(INVALID, dummy),					// 1001 1100
		ENTRY(INVALID, dummy),					// 1001 1101
		ENTRY(INVALID, dummy),					// 1001 1110
		ENTRY(INVALID, dummy),					// 1001 1111

		// Ax
		ENTRY(INVALID, dummy),					// 1010 0000
		ENTRY(INVALID, dummy),					// 1010 0001
		ENTRY(INVALID, dummy),					// 1010 0010
		ENTRY(INVALID, dummy),					// 1010 0011
		ENTRY(INVALID, dummy),					// 1010 0100
		ENTRY(INVALID, dummy),					// 1010 0101
		ENTRY(INVALID, dummy),					// 1010 0110
		ENTRY(INVALID, dummy),					// 1010 0111
		ENTRY(INVALID, dummy),					// 1010 1000
		ENTRY(INVALID, dummy),					// 1010 1001
		ENTRY(INVALID, dummy),					// 1010 1010
		ENTRY(INVALID, dummy),					// 1010 1011
		ENTRY(INVALID, dummy),					// 1010 1100
		ENTRY(INVALID, dummy),					// 1010 1101
		ENTRY(INVALID, dummy),					// 1010 1110
		ENTRY(INVALID, dummy),					// 1010 1111

		// Bx
		ENTRY(INVALID, dummy),					// 1011 0000
		ENTRY(INVALID, dummy),					// 1011 0001
		ENTRY(INVALID, dummy),					// 1011 0010
		ENTRY(INVALID, dummy),					// 1011 0011
		ENTRY(INVALID, dummy),					// 1011 0100
		ENTRY(INVALID, dummy),					// 1011 0101
		ENTRY(INVALID, dummy),					// 1011 0110
		ENTRY(INVALID, dummy),					// 1011 0111
		ENTRY(INVALID, dummy),					// 1011 1000
		ENTRY(INVALID, dummy),					// 1011 1001
		ENTRY(INVALID, dummy),					// 1011 1010
		ENTRY(INVALID, dummy),					// 1011 1011
		ENTRY(INVALID, dummy),					// 1011 1100
		ENTRY(INVALID, dummy),					// 1011 1101
		ENTRY(INVALID, dummy),					// 1011 1110
		ENTRY(INVALID, dummy),					// 1011 1111

		// Cx
		ENTRY(INVALID, dummy),					// 1100 0000
		ENTRY(INVALID, dummy),					// 1100 0001
		ENTRY(INVALID, dummy),					// 1100 0010
		ENTRY(INVALID, dummy),					// 1100 0011
		ENTRY(INVALID, dummy),					// 1100 0100
		ENTRY(INVALID, dummy),					// 1100 0101
		ENTRY(INVALID, dummy),					// 1100 0110
		ENTRY(INVALID, dummy),					// 1100 0111
		ENTRY(INVALID, dummy),					// 1100 1000
		ENTRY(INVALID, dummy),					// 1100 1001
		ENTRY(INVALID, dummy),					// 1100 1010
		ENTRY(INVALID, dummy),					// 1100 1011
		ENTRY(INVALID, dummy),					// 1100 1100
		ENTRY(INVALID, dummy),					// 1100 1101
		ENTRY(INVALID, dummy),					// 1100 1110
		ENTRY(INVALID, dummy),					// 1100 1111

		// Dx
		ENTRY(INVALID, dummy),					// 1101 0000
		ENTRY(INVALID, dummy),					// 1101 0001
		ENTRY(INVALID, dummy),					// 1101 0010
		ENTRY(INVALID, dummy),					// 1101 0011
		ENTRY(INVALID, dummy),					// 1101 0100
		ENTRY(INVALID, dummy),					// 1101 0101
		ENTRY(INVALID, dummy),					// 1101 0110
		ENTRY(INVALID, dummy),					// 1101 0111
		ENTRY(INVALID, dummy),					// 1101 1000
		ENTRY(INVALID, dummy),					// 1101 1001
		ENTRY(INVALID, dummy),					// 1101 1010
		ENTRY(INVALID, dummy),					// 1101 1011
		ENTRY(INVALID, dummy),					// 1101 1100
		ENTRY(INVALID, dummy),					// 1101 1101
		ENTRY(INVALID, dummy),					// 1101 1110
		ENTRY(INVALID, dummy),					// 1101 1111

		// Ex
		ENTRY(INVALID, dummy),					// 1110 0000
		ENTRY(INVALID, dummy),					// 1110 0001
		ENTRY(INVALID, dummy),					// 1110 0010
		ENTRY(INVALID, dummy),					// 1110 0011
		ENTRY(INVALID, dummy),					// 1110 0100
		ENTRY(INVALID, dummy),					// 1110 0101
		ENTRY(INVALID, dummy),					// 1110 0110
		ENTRY(INVALID, dummy),					// 1110 0111
		ENTRY(INVALID, dummy),					// 1110 1000
		ENTRY(INVALID, dummy),					// 1110 1001
		ENTRY(INVALID, dummy),					// 1110 1010
		ENTRY(INVALID, dummy),					// 1110 1011
		ENTRY(INVALID, dummy),					// 1110 1100
		ENTRY(INVALID, dummy),					// 1110 1101
		ENTRY(INVALID, dummy),					// 1110 1110
		ENTRY(INVALID, dummy),					// 1110 1111

		// Fx
		ENTRY(INVALID, dummy),					// 1111 0000
		ENTRY(INVALID, dummy),					// 1111 0001
		ENTRY(INVALID, dummy),					// 1111 0010
		ENTRY(INVALID, dummy),					// 1111 0011
		ENTRY(INVALID, dummy),					// 1111 0100
		ENTRY(INVALID, dummy),					// 1111 0101
		ENTRY(INVALID, dummy),					// 1111 0110
		ENTRY(INVALID, dummy),					// 1111 0111
		ENTRY(INVALID, dummy),					// 1111 1000
		ENTRY(INVALID, dummy),					// 1111 1001
		ENTRY(INVALID, dummy),					// 1111 1010
		ENTRY(INVALID, dummy),					// 1111 1011
		ENTRY(INVALID, dummy),					// 1111 1100
		ENTRY(INVALID, dummy),					// 1111 1101
		ENTRY(INVALID, dummy),					// 1111 1110
		ENTRY(INVALID, dummy),					// 1111 1111
	};

	int decode_pm(instruction& inst, uint32_t header, const decode_settings& settings, buffer_reader& buf)
	{
		// Main instruction type is stored in the lower 8 bits
		uint8_t base_type = inst.header & 0xff;
		const op_entry& entry = pm_entries[base_type];
		inst.opcode = entry.opcode;
		inst.word_count = 1;		// default values
		int ret = entry.func(inst, header, settings, buf);
		return ret;
	}

	// ========================================================================
	//	NON-PARALLEL MOVE OPCODE FUNCTIONS
	// ========================================================================

	static int andi(instruction& inst, uint32_t header, const decode_settings& settings, buffer_reader& buf)
	{
		uint32_t EE = (header >> 0) & 0x3;
		static Reg dests[4] = 		{ Reg::MR, Reg::CCR, Reg::OMR, Reg::NONE };
		set_imm(inst.operands[0], (header >> 8) & 0xff);
		set_reg(inst.operands[1], dests[EE]);
		return dests[EE] == Reg::NONE ? 1 : 0;
	}

	static int bchg(instruction& inst, uint32_t header, const decode_settings& settings, buffer_reader& buf)
	{
		uint32_t bbbbb = (header >> 0) & 0x1f;
		uint32_t S = (header >> 6) & 0x1;
		uint32_t rrr = (header >> 8) & 0x7;
		uint32_t mmm = (header >> 11) & 0x7;
		set_imm_short(inst.operands[0], bbbbb);
		return decode_mmmrrr(inst.operands[1], S ? MEM_Y : MEM_X, mmm, rrr, EA_MODE_WRITE, buf);
	}

	// ========================================================================
	//	OPCODE TABLE FOR NON-PARALLEL MOVE INSTRUCTIONS
	// ========================================================================
	static const op_entry non_pm_entries[256] =
	{
		// 0x
		ENTRY(INVALID, dummy),					// 0000 0000
		ENTRY(INVALID, dummy),					// 0000 0001
		ENTRY(INVALID, dummy),					// 0000 0010
		ENTRY(ANDI,    andi),					// 0000 0011
		ENTRY(INVALID, dummy),					// 0000 0100
		ENTRY(INVALID, dummy),					// 0000 0101
		ENTRY(INVALID, dummy),					// 0000 0110
		ENTRY(ANDI,    andi),					// 0000 0111
		ENTRY(INVALID, dummy),					// 0000 1000
		ENTRY(INVALID, dummy),					// 0000 1001
		ENTRY(INVALID, dummy),					// 0000 1010
		ENTRY(ANDI,    andi),					// 0000 1011
		ENTRY(INVALID, dummy),					// 0000 1100
		ENTRY(INVALID, dummy),					// 0000 1101
		ENTRY(INVALID, dummy),					// 0000 1110
		ENTRY(ANDI,    andi),					// 0000 1111

		// 1x
		ENTRY(INVALID, dummy),					// 0001 0000
		ENTRY(INVALID, dummy),					// 0001 0001
		ENTRY(INVALID, dummy),					// 0001 0010
		ENTRY(INVALID, dummy),					// 0001 0011
		ENTRY(INVALID, dummy),					// 0001 0100
		ENTRY(INVALID, dummy),					// 0001 0101
		ENTRY(INVALID, dummy),					// 0001 0110
		ENTRY(INVALID, dummy),					// 0001 0111
		ENTRY(INVALID, dummy),					// 0001 1000
		ENTRY(INVALID, dummy),					// 0001 1001
		ENTRY(INVALID, dummy),					// 0001 1010
		ENTRY(INVALID, dummy),					// 0001 1011
		ENTRY(INVALID, dummy),					// 0001 1100
		ENTRY(INVALID, dummy),					// 0001 1101
		ENTRY(INVALID, dummy),					// 0001 1110
		ENTRY(INVALID, dummy),					// 0001 1111

		// 2x
		ENTRY(INVALID, dummy),					// 0010 0000
		ENTRY(INVALID, dummy),					// 0010 0001
		ENTRY(INVALID, dummy),					// 0010 0010
		ENTRY(INVALID, dummy),					// 0010 0011
		ENTRY(INVALID, dummy),					// 0010 0100
		ENTRY(INVALID, dummy),					// 0010 0101
		ENTRY(INVALID, dummy),					// 0010 0110
		ENTRY(INVALID, dummy),					// 0010 0111
		ENTRY(INVALID, dummy),					// 0010 1000
		ENTRY(INVALID, dummy),					// 0010 1001
		ENTRY(INVALID, dummy),					// 0010 1010
		ENTRY(INVALID, dummy),					// 0010 1011
		ENTRY(INVALID, dummy),					// 0010 1100
		ENTRY(INVALID, dummy),					// 0010 1101
		ENTRY(INVALID, dummy),					// 0010 1110
		ENTRY(INVALID, dummy),					// 0010 1111

		// 3x
		ENTRY(INVALID, dummy),					// 0011 0000
		ENTRY(INVALID, dummy),					// 0011 0001
		ENTRY(INVALID, dummy),					// 0011 0010
		ENTRY(INVALID, dummy),					// 0011 0011
		ENTRY(INVALID, dummy),					// 0011 0100
		ENTRY(INVALID, dummy),					// 0011 0101
		ENTRY(INVALID, dummy),					// 0011 0110
		ENTRY(INVALID, dummy),					// 0011 0111
		ENTRY(INVALID, dummy),					// 0011 1000
		ENTRY(INVALID, dummy),					// 0011 1001
		ENTRY(INVALID, dummy),					// 0011 1010
		ENTRY(INVALID, dummy),					// 0011 1011
		ENTRY(INVALID, dummy),					// 0011 1100
		ENTRY(INVALID, dummy),					// 0011 1101
		ENTRY(INVALID, dummy),					// 0011 1110
		ENTRY(INVALID, dummy),					// 0011 1111

		// 4x
		ENTRY(INVALID, dummy),					// 0100 0000
		ENTRY(INVALID, dummy),					// 0100 0001
		ENTRY(INVALID, dummy),					// 0100 0010
		ENTRY(INVALID, dummy),					// 0100 0011
		ENTRY(INVALID, dummy),					// 0100 0100
		ENTRY(INVALID, dummy),					// 0100 0101
		ENTRY(INVALID, dummy),					// 0100 0110
		ENTRY(INVALID, dummy),					// 0100 0111
		ENTRY(INVALID, dummy),					// 0100 1000
		ENTRY(INVALID, dummy),					// 0100 1001
		ENTRY(INVALID, dummy),					// 0100 1010
		ENTRY(INVALID, dummy),					// 0100 1011
		ENTRY(INVALID, dummy),					// 0100 1100
		ENTRY(INVALID, dummy),					// 0100 1101
		ENTRY(INVALID, dummy),					// 0100 1110
		ENTRY(INVALID, dummy),					// 0100 1111

		// 5x
		ENTRY(INVALID, dummy),					// 0101 0000
		ENTRY(INVALID, dummy),					// 0101 0001
		ENTRY(INVALID, dummy),					// 0101 0010
		ENTRY(INVALID, dummy),					// 0101 0011
		ENTRY(INVALID, dummy),					// 0101 0100
		ENTRY(INVALID, dummy),					// 0101 0101
		ENTRY(INVALID, dummy),					// 0101 0110
		ENTRY(INVALID, dummy),					// 0101 0111
		ENTRY(INVALID, dummy),					// 0101 1000
		ENTRY(INVALID, dummy),					// 0101 1001
		ENTRY(INVALID, dummy),					// 0101 1010
		ENTRY(INVALID, dummy),					// 0101 1011
		ENTRY(INVALID, dummy),					// 0101 1100
		ENTRY(INVALID, dummy),					// 0101 1101
		ENTRY(INVALID, dummy),					// 0101 1110
		ENTRY(INVALID, dummy),					// 0101 1111

		// 6x
		ENTRY(INVALID, dummy),					// 0110 0000
		ENTRY(INVALID, dummy),					// 0110 0001
		ENTRY(INVALID, dummy),					// 0110 0010
		ENTRY(INVALID, dummy),					// 0110 0011
		ENTRY(INVALID, dummy),					// 0110 0100
		ENTRY(INVALID, dummy),					// 0110 0101
		ENTRY(INVALID, dummy),					// 0110 0110
		ENTRY(INVALID, dummy),					// 0110 0111
		ENTRY(INVALID, dummy),					// 0110 1000
		ENTRY(INVALID, dummy),					// 0110 1001
		ENTRY(INVALID, dummy),					// 0110 1010
		ENTRY(INVALID, dummy),					// 0110 1011
		ENTRY(INVALID, dummy),					// 0110 1100
		ENTRY(INVALID, dummy),					// 0110 1101
		ENTRY(INVALID, dummy),					// 0110 1110
		ENTRY(INVALID, dummy),					// 0110 1111

		// 7x
		ENTRY(INVALID, dummy),					// 0111 0000
		ENTRY(INVALID, dummy),					// 0111 0001
		ENTRY(INVALID, dummy),					// 0111 0010
		ENTRY(INVALID, dummy),					// 0111 0011
		ENTRY(INVALID, dummy),					// 0111 0100
		ENTRY(INVALID, dummy),					// 0111 0101
		ENTRY(INVALID, dummy),					// 0111 0110
		ENTRY(INVALID, dummy),					// 0111 0111
		ENTRY(INVALID, dummy),					// 0111 1000
		ENTRY(INVALID, dummy),					// 0111 1001
		ENTRY(INVALID, dummy),					// 0111 1010
		ENTRY(INVALID, dummy),					// 0111 1011
		ENTRY(INVALID, dummy),					// 0111 1100
		ENTRY(INVALID, dummy),					// 0111 1101
		ENTRY(INVALID, dummy),					// 0111 1110
		ENTRY(INVALID, dummy),					// 0111 1111

		// 8x
		ENTRY(INVALID, decode_pm),				// 1000 0000
		ENTRY(INVALID, decode_pm),				// 1000 0001
		ENTRY(INVALID, decode_pm),				// 1000 0010
		ENTRY(INVALID, decode_pm),				// 1000 0011
		ENTRY(INVALID, dummy),					// 1000 0100
		ENTRY(INVALID, dummy),					// 1000 0101
		ENTRY(INVALID, dummy),					// 1000 0110
		ENTRY(INVALID, dummy),					// 1000 0111
		ENTRY(INVALID, decode_pm),				// 1000 1000
		ENTRY(INVALID, decode_pm),				// 1000 1001
		ENTRY(INVALID, decode_pm),				// 1000 1010
		ENTRY(INVALID, decode_pm),				// 1000 1011
		ENTRY(INVALID, dummy),					// 1000 1100
		ENTRY(INVALID, dummy),					// 1000 1101
		ENTRY(INVALID, dummy),					// 1000 1110
		ENTRY(INVALID, dummy),					// 1000 1111

		// 9x
		ENTRY(INVALID, decode_pm),				// 1001 0000
		ENTRY(INVALID, decode_pm),				// 1001 0001
		ENTRY(INVALID, decode_pm),				// 1001 0010
		ENTRY(INVALID, decode_pm),				// 1001 0011
		ENTRY(INVALID, dummy),					// 1001 0100
		ENTRY(INVALID, dummy),					// 1001 0101
		ENTRY(INVALID, dummy),					// 1001 0110
		ENTRY(INVALID, dummy),					// 1001 0111
		ENTRY(INVALID, decode_pm),				// 1001 1000
		ENTRY(INVALID, decode_pm),				// 1001 1001
		ENTRY(INVALID, decode_pm),				// 1001 1010
		ENTRY(INVALID, decode_pm),				// 1001 1011
		ENTRY(INVALID, dummy),					// 1001 1100
		ENTRY(INVALID, dummy),					// 1001 1101
		ENTRY(INVALID, dummy),					// 1001 1110
		ENTRY(INVALID, dummy),					// 1001 1111

		// Ax
		ENTRY(INVALID, dummy),					// 1010 0000
		ENTRY(INVALID, dummy),					// 1010 0001
		ENTRY(INVALID, dummy),					// 1010 0010
		ENTRY(INVALID, dummy),					// 1010 0011
		ENTRY(INVALID, dummy),					// 1010 0100
		ENTRY(INVALID, dummy),					// 1010 0101
		ENTRY(INVALID, dummy),					// 1010 0110
		ENTRY(INVALID, dummy),					// 1010 0111
		ENTRY(INVALID, dummy),					// 1010 1000
		ENTRY(INVALID, dummy),					// 1010 1001
		ENTRY(INVALID, dummy),					// 1010 1010
		ENTRY(INVALID, dummy),					// 1010 1011
		ENTRY(INVALID, dummy),					// 1010 1100
		ENTRY(INVALID, dummy),					// 1010 1101
		ENTRY(INVALID, dummy),					// 1010 1110
		ENTRY(INVALID, dummy),					// 1010 1111

		// Bx
		ENTRY(INVALID, dummy),					// 1011 0000
		ENTRY(INVALID, dummy),					// 1011 0001
		ENTRY(INVALID, dummy),					// 1011 0010
		ENTRY(INVALID, dummy),					// 1011 0011
		ENTRY(INVALID, dummy),					// 1011 0100
		ENTRY(INVALID, dummy),					// 1011 0101
		ENTRY(INVALID, dummy),					// 1011 0110
		ENTRY(INVALID, dummy),					// 1011 0111
		ENTRY(BCHG,    bchg),					// 1011 1000		// incorrect in the PDF
		ENTRY(INVALID, bchg),					// 1011 1001
		ENTRY(INVALID, bchg),					// 1011 1010
		ENTRY(INVALID, bchg),					// 1011 1011
		ENTRY(INVALID, dummy),					// 1011 1100
		ENTRY(INVALID, dummy),					// 1011 1101
		ENTRY(INVALID, dummy),					// 1011 1110
		ENTRY(INVALID, dummy),					// 1011 1111

		// Cx
		ENTRY(INVALID, dummy),					// 1100 0000
		ENTRY(INVALID, dummy),					// 1100 0001
		ENTRY(INVALID, dummy),					// 1100 0010
		ENTRY(INVALID, dummy),					// 1100 0011
		ENTRY(INVALID, dummy),					// 1100 0100
		ENTRY(INVALID, dummy),					// 1100 0101
		ENTRY(INVALID, dummy),					// 1100 0110
		ENTRY(INVALID, dummy),					// 1100 0111
		ENTRY(INVALID, dummy),					// 1100 1000
		ENTRY(INVALID, dummy),					// 1100 1001
		ENTRY(INVALID, dummy),					// 1100 1010
		ENTRY(INVALID, dummy),					// 1100 1011
		ENTRY(INVALID, dummy),					// 1100 1100
		ENTRY(INVALID, dummy),					// 1100 1101
		ENTRY(INVALID, dummy),					// 1100 1110
		ENTRY(INVALID, dummy),					// 1100 1111

		// Dx
		ENTRY(INVALID, dummy),					// 1101 0000
		ENTRY(INVALID, dummy),					// 1101 0001
		ENTRY(INVALID, dummy),					// 1101 0010
		ENTRY(INVALID, dummy),					// 1101 0011
		ENTRY(INVALID, dummy),					// 1101 0100
		ENTRY(INVALID, dummy),					// 1101 0101
		ENTRY(INVALID, dummy),					// 1101 0110
		ENTRY(INVALID, dummy),					// 1101 0111
		ENTRY(INVALID, dummy),					// 1101 1000
		ENTRY(INVALID, dummy),					// 1101 1001
		ENTRY(INVALID, dummy),					// 1101 1010
		ENTRY(INVALID, dummy),					// 1101 1011
		ENTRY(INVALID, dummy),					// 1101 1100
		ENTRY(INVALID, dummy),					// 1101 1101
		ENTRY(INVALID, dummy),					// 1101 1110
		ENTRY(INVALID, dummy),					// 1101 1111

		// Ex
		ENTRY(INVALID, dummy),					// 1110 0000
		ENTRY(INVALID, dummy),					// 1110 0001
		ENTRY(INVALID, dummy),					// 1110 0010
		ENTRY(INVALID, dummy),					// 1110 0011
		ENTRY(INVALID, dummy),					// 1110 0100
		ENTRY(INVALID, dummy),					// 1110 0101
		ENTRY(INVALID, dummy),					// 1110 0110
		ENTRY(INVALID, dummy),					// 1110 0111
		ENTRY(INVALID, dummy),					// 1110 1000
		ENTRY(INVALID, dummy),					// 1110 1001
		ENTRY(INVALID, dummy),					// 1110 1010
		ENTRY(INVALID, dummy),					// 1110 1011
		ENTRY(INVALID, dummy),					// 1110 1100
		ENTRY(INVALID, dummy),					// 1110 1101
		ENTRY(INVALID, dummy),					// 1110 1110
		ENTRY(INVALID, dummy),					// 1110 1111

		// Fx
		ENTRY(INVALID, dummy),					// 1111 0000
		ENTRY(INVALID, dummy),					// 1111 0001
		ENTRY(INVALID, dummy),					// 1111 0010
		ENTRY(INVALID, dummy),					// 1111 0011
		ENTRY(INVALID, dummy),					// 1111 0100
		ENTRY(INVALID, dummy),					// 1111 0101
		ENTRY(INVALID, dummy),					// 1111 0110
		ENTRY(INVALID, dummy),					// 1111 0111
		ENTRY(INVALID, dummy),					// 1111 1000
		ENTRY(INVALID, dummy),					// 1111 1001
		ENTRY(INVALID, dummy),					// 1111 1010
		ENTRY(INVALID, dummy),					// 1111 1011
		ENTRY(INVALID, dummy),					// 1111 1100
		ENTRY(INVALID, dummy),					// 1111 1101
		ENTRY(INVALID, dummy),					// 1111 1110
		ENTRY(INVALID, dummy),					// 1111 1111
	};

	int decode_non_pm(instruction& inst, uint32_t header, const decode_settings& settings, buffer_reader& buf)
	{
		// 6 bits from 14 onwards, bit 7, bit 5
		// form an 8-bit word from that
		uint8_t base_type = (inst.header >> 12) & 0xfc;
		base_type |= (inst.header >> 6) & 0x2;
		base_type |= (inst.header >> 5) & 0x1;
		printf("non-pm base_type: %b\n", base_type);
		const op_entry& entry = non_pm_entries[base_type];
		inst.opcode = entry.opcode;
		inst.word_count = 1;		// default values

		int ret = entry.func(inst, inst.header, settings, buf);
		return ret;
	}

	int decode(instruction& inst, buffer_reader& buf, const decode_settings& settings)
	{
		inst.reset();
		buffer_reader buf_copy = buf;

		if (buf.read_word(inst.header))
			return 1;

		uint32_t top_bits = (inst.header >> 20) & 0xffff;
		int ret = 0;
		if (top_bits == 0x0)
			// Instruction without parallel move
			ret = decode_non_pm(inst, inst.header, settings, buf);
		else
			ret = decode_pm(inst, inst.header, settings, buf);

		if (ret == 0)
		{
			inst.word_count = buf.get_pos() - buf_copy.get_pos();
			return ret;
		}
		inst.opcode = instruction::Opcode::INVALID;
		set_abs(inst.operands[0], inst.header);
		return ret;
	}
}

