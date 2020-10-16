#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include <memory.h>
#include <assert.h>
#include <vector>

// ----------------------------------------------------------------------------
class buffer_reader
{
public:
	buffer_reader(const uint8_t* pData, uint32_t  length) :
		m_pData(pData),
		m_length(length),
		m_pos(0)
	{}

	int read_byte(uint8_t& data)
	{
		if (m_pos + 1 > m_length)
			return 1;
		data = m_pData[m_pos++];
		return 0;
	}

	int read_word(uint16_t& data)
	{
		if (m_pos + 2 > m_length)
			return 1;
		data = m_pData[m_pos++];
		data <<= 8;
		data |= m_pData[m_pos++];
		return 0;
	}

	int read_long(uint32_t& data)
	{
		if (m_pos + 4 > m_length)
			return 1;
		data = m_pData[m_pos++];
		data <<= 8;
		data |= m_pData[m_pos++];
		data <<= 8;
		data |= m_pData[m_pos++];
		data <<= 8;
		data |= m_pData[m_pos++];
		return 0;
	}

	void advance(uint32_t count)
	{
		m_pos += count;
		if (m_pos > m_length)
			m_pos = m_length;
	}

	const uint8_t* get_data() const
	{
		return m_pData + m_pos;
	}

	uint32_t get_pos() const
	{
		return m_pos;
	}	

	uint32_t get_remain() const
	{
		return m_length - m_pos;
	}

private:
	const uint8_t*  m_pData;
	const uint32_t  m_length;
	uint32_t		m_pos;
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
	kNone = 0,
	D_DIRECT,			// Dn		 000 reg. number:Dn
	A_DIRECT,			// An		 001 reg. number:An
	INDIRECT,			// (An)	   010 reg. number:An
	INDIRECT_POSTINC,	// (An) +	 011 reg. number:An
	INDIRECT_PREDEC,	 // – (An)	 100 reg. number:An
	INDIRECT_DISP,	   // (d16,An)   101 reg. number:An
	INDIRECT_INDEX,	  // (d8,An,Xn) 110 reg. number:An
	ABSOLUTE_WORD,	   // (xxx).W	111 000
	ABSOLUTE_LONG,	   // (xxx).L	111 001
	PC_DISP,			 // (d16,PC)   111 010
	PC_DISP_INDEX,	   // (d8,PC,Xn) 111 011
	IMMEDIATE,		   // <data>	 111 100
	MOVEM_REG,		  // mask of registers

	// Specific registers
	SR,
	USP
};

// ----------------------------------------------------------------------------
struct operand
{
	operand() :
		type(OpType::kNone)
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
			Size		size;
			uint32_t	val0;
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
			uint8_t reg;
		} indirect_disp;

		struct
		{
			int8_t disp;
			uint8_t a_reg;
			uint8_t d_reg;
			bool is_long;
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
			int16_t disp;
		} pc_disp;

		struct
		{
			int8_t disp;
			uint8_t d_reg;
			bool is_long;
		} pc_disp_index;

		struct
		{
			uint16_t reg_mask;
		} movem_reg;
	};
};

enum Suffix
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
		suffix(NONE),
		tag(NULL)
	{
	}

	uint16_t	header;			// first 16-bit word of data
	uint16_t	byte_count;
	Suffix		suffix;
	const char*	tag;			// TODO replace with instruction enum
	operand		op0;
	operand		op1;
};

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

// ----------------------------------------------------------------------------
//	PARSER CODE
// ----------------------------------------------------------------------------
// Effective Address modes
enum ea_group
{
	DATA_ALT		= 0,	// Data alterable (no A-reg, no PC-rel, no immedidate)
	DATA			= 1,	// Data, non-alterable (source)
	MEM_ALT		 = 2,	// Memory, alterable (no A-reg, no PC-rel, no immedidate)
	MEM			 = 3,	// Memory, non-alterable (source)
	CONTROL		 = 4,
	CONTROL_MOVEM1  = 5,	// Movem to memory
	CONTROL_MOVEM2  = 6,	// Movem to reg
	ALT			 = 7,	// Alterable including A-reg
	ALL			 = 8,	 // e.g. cmp
	COUNT
};

// Defines which instruction modes are allowed to have which EA modes
bool mode_availability[][ea_group::COUNT] = 
{
	// DataAlt Data	 MemAlt   Mem	  Con	  ConMovem CMovem2  Alt	 All
	{ true,	true,	false,   false,   false,   false,   false,   true,   true	}, // D_DIRECT			000 regno
	{ false,   false,   false,   false,   false,   false,   false,   true,   true	}, // A_DIRECT			001 regno
	{ true,	true,	true,	true,	true,	true,	true,	true,   true	}, // INDIRECT			010 regno
	{ true,	true,	true,	true,	false,   false,   true,	true,   true	}, // INDIRECT_POSTINC	011 regno
	{ true,	true,	true,	true,	false,   true,	true,	true,   true	}, // INDIRECT_PREDEC	 100 regno
	{ true,	true,	true,	true,	true,	true,	true,	true,   true	}, // INDIRECT_DISP	   101 regno
	{ true,	true,	true,	true,	true,	true,	true,	true,   true	}, // INDIRECT_INDEX,	 110 regno
	{ true,	true,	true,	true,	true,	true,	true,	true,   true	}, // ABSOLUTE_WORD	   111 000
	{ true,	true,	true,	true,	true,	true,	true,	true,   true	}, // ABSOLUTE_LONG	   111 001  // There is a typo in the doc here
	{ false,   true,	false,   true,	true,	true,	true,	false,  true	}, // PC_DISP			 111 010
	{ false,   true,	false,   true,	true,	true,	true,	false,  true	}, // PC_DISP_INDEX	   111 011
	{ false,   true,	false,   true,	false,   false,   false,   false,  true	}, // IMMEDIATE		   111 100
};

// There is a consistent approach to decoding the mode bits (0-6),
// and when the mode is 7, using the register bits
OpType raw_bits_to_operand_type(uint8_t mode_bits, uint8_t reg_bits)
{
	static const OpType types0_6[] =
	{
		D_DIRECT,			// Dn		 000 reg. number:Dn
		A_DIRECT,			// An		 001 reg. number:An
		INDIRECT,			// (An)	   010 reg. number:An
		INDIRECT_POSTINC,	// (An) +	 011 reg. number:An
		INDIRECT_PREDEC,	 // – (An)	 100 reg. number:An
		INDIRECT_DISP,	   // (d16,An)   101 reg. number:An
		INDIRECT_INDEX,	  // (d8,An,Xn) 110 reg. number:An
	};
	static const OpType regtypes0_4[] =
	{
		ABSOLUTE_WORD,	   // (xxx).W	111 000
		ABSOLUTE_LONG,	   // (xxx).L	111 001
		PC_DISP,			 // (d16,PC)   111 010
		PC_DISP_INDEX,	   // (d8,PC,Xn) 111 011
		IMMEDIATE,		   // <data>	 111 100
	};

	if (mode_bits < 7)
		return types0_6[mode_bits];
	
	if (reg_bits <= 4)
		return regtypes0_4[reg_bits];

	return OpType::kNone;
}

#if 0
	def __init__(self, eatype, mode, reg, size, reader):
		self.mode = self.raw_bits_to_ea_mode(mode, reg)
		self.final_address = None
		if self.mode == None:
			raise ParseException('unknown EA')

		# Check EA type is valid for this instruction
		if self.mode_availability{self.mode][eatype] == false:
			raise ParseException('invalid EA')

		elif self.mode == self.PC_DISP_INDEX:
			base_pc = reader.pc
			disp = reader.read(2)
			(disp, dreg, indsize) = self.decode_brief_extension_word(disp)
			self.final_address = disp + base_pc
			sym = reader.ctx.get_symbol(disp + base_pc)
			if sym != None:
				self.token = '%s(pc,d%u.%s)' % (sym, dreg, indsize)
			else:
				self.token = '$%x(pc,d%u.%s)' % (disp + base_pc, dreg, indsize)
		# Immediate. Number of bytes read will vary!
		elif self.mode == self.IMMEDIATE:
			if size == ea.LONG:
				imm = reader.read(4)
			elif size == ea.WORD:
				imm = reader.read(2)
			elif size == ea.BYTE:
				imm = reader.read(2) & 0xff
			else:
				# Should never get passed in
				assert "Wrong size"
			self.token = '#$%x' % imm
		else:
			# Corrupt
			raise ParseException

	def decode_brief_extension_word(self, word):
		disp = extend_s8(word & 0x7f)
		reg = (header >> 12) & 7
		# Check .w/.l bit
		sizes = ['w', 'l']
		size = sizes[((header >> 11) & 1)]
		return (disp, reg, size)

#endif

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
int read_ea(buffer_reader& buffer, operand& operand, ea_group group, uint8_t mode_bits, uint8_t reg_bits, Size size)
{
	// Convert to an operand type
	operand.type = raw_bits_to_operand_type(mode_bits, reg_bits);
	// TODO Check EA type is valid for this instruction
	//if self.mode_availability{self.mode][eatype] == false:
	//		raise ParseException('invalid EA')

	uint16_t val16;
	uint32_t val32;
	switch (operand.type)
	{
		case OpType::D_DIRECT:
			operand.d_register.reg = reg_bits;
			return 0;

		case OpType::A_DIRECT:
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
Size standard_size_table[] =
{
	Size::BYTE,
	Size::WORD,
	Size::LONG,
	Size::NONE
};

Suffix size_to_suffix(Size size)
{
	switch (size)
	{
		case Size::BYTE: return Suffix::BYTE;
		case Size::WORD: return Suffix::WORD;
		case Size::LONG: return Suffix::LONG;
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
	return read_ea(buffer, inst.op1, DATA_ALT, mode, reg, ea_size);
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

	return read_ea(buffer, inst.op0, DATA_ALT, mode, reg, ea_size);
}

// Single EA operand
int Inst_lea(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t size = (header >> 6) & 3;
	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;
	uint8_t dest_reg  = (header >> 9) & 7;

	// Target register
	inst.op1.type = OpType::A_DIRECT;
	inst.op1.a_register.reg = dest_reg;

	inst.suffix = Suffix::LONG;
	return read_ea(buffer, inst.op0, CONTROL, mode, reg, Size::LONG);
}

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
	if (read_ea(buffer, inst.op1, CONTROL_MOVEM1, mode, reg, ea_size))
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
	return read_ea(buffer, inst.op0, CONTROL_MOVEM1, mode, reg, ea_size);
}

// ----------------------------------------------------------------------------
int Inst_move(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t sizebits = (header >> 12) & 3;
	uint8_t src_reg  = (header >> 0) & 7;
	uint8_t src_mode = (header >> 3) & 7;
	uint8_t dst_reg  = (header >> 9) & 7;
	uint8_t dst_mode = (header >> 6) & 7;

	Size sizes[] = { Size::BYTE, Size::LONG, Size::WORD, Size::NONE };
	Size ea_size = sizes[sizebits];
	if (ea_size == Size::NONE)
		return 1;
	inst.suffix = size_to_suffix(ea_size);

	// Src data comes first in the encoding
	int ret = read_ea(buffer, inst.op0, ea_group::ALL, src_mode, src_reg, ea_size);
	if (ret != 0)
		return ret;
	ret = read_ea(buffer, inst.op1, ea_group::DATA_ALT, dst_mode, dst_reg, ea_size);
	return ret;
}

// ----------------------------------------------------------------------------
int Inst_movea(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t size = (header >> 12) & 3;
	uint8_t areg = (header >> 9) & 7;
	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;

	Size sizes[] = { Size::NONE, Size::LONG, Size::WORD, Size::NONE };
	Size ea_size = sizes[size];
	if (ea_size == Size::NONE)
		return 1;
	inst.suffix = size_to_suffix(ea_size);

	if (read_ea(buffer, inst.op0, ALL, mode, reg, ea_size))
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
	return read_ea(buffer, inst.op1, ALT, mode, reg, ea_size);
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
	return read_ea(buffer, inst.op1, DATA, mode, reg, Size::WORD);
}

// ----------------------------------------------------------------------------
int Inst_move_to_sr(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	inst.suffix = Suffix::WORD;
	inst.op1.type = OpType::SR;

	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;
	return read_ea(buffer, inst.op0, DATA, mode, reg, Size::WORD);
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
	if (read_ea(buffer, inst.op1, DATA_ALT, mode, reg, Size::NONE))
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
	read_ea(buffer, inst.op1, DATA_ALT, mode, reg, Size::BYTE);
	return 0;
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
		return read_ea(buffer, inst.op1, MEM_ALT, mode, reg, ea_size);
	}
	else
	{
		// Dest is d-reg
		inst.op1.type = OpType::D_DIRECT;
		inst.op1.d_register.reg = dreg;
		return read_ea(buffer, inst.op0, ALL, mode, reg, ea_size);
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
	return read_ea(buffer, inst.op0, ALL, mode, reg, ea_size);
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

int Inst_stop(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint16_t val;
	if (buffer.read_word(val))
		return 1;
	set_imm_word(inst.op0, val);
	return 0;
}

int Inst_simple(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	return 0;
}

int Inst_swap(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t reg = (header >> 0) & 7;
	set_dreg(inst.op0, reg);
	return 0;
}

int Inst_link_w(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t reg = (header >> 0) & 7;
	set_areg(inst.op0, reg);

	uint16_t disp;
	if (buffer.read_word(disp))
		return 1;

	// NO CHECK unsigned
	set_imm_word(inst.op1, disp);
	return 0;
}

int Inst_unlk(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t reg = (header >> 0) & 7;
	set_areg(inst.op0, reg);
	return 0;
}

int Inst_move_from_usp(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t reg = (header >> 0) & 7;
	inst.op0.type = OpType::USP;
	set_areg(inst.op1, reg);
	return 0;
}

int Inst_move_to_usp(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t reg = (header >> 0) & 7;
	set_areg(inst.op0, reg);
	inst.op1.type = OpType::USP;
	return 0;
}

int Inst_nbcd(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	inst.suffix = Suffix::BYTE;
	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;
	return read_ea(buffer, inst.op0, DATA_ALT, mode, reg, Size::BYTE);
}

int Inst_pea(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	inst.suffix = Suffix::LONG;
	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;
	return read_ea(buffer, inst.op0, CONTROL, mode, reg, Size::LONG);
}

// NOTE: identical to nbcd
int Inst_tas(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;
	return read_ea(buffer, inst.op0, DATA_ALT, mode, reg, Size::BYTE);
}

// jmp/jsr
int Inst_jump(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;
	return read_ea(buffer, inst.op0, CONTROL, mode, reg, Size::NONE);
}

int Inst_asl_asr_mem(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t size = (header >> 6) & 3;
	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;
	// "An operand in memory can be shifted one bit only, and the operand size is restricted to a word."
	inst.suffix = Suffix::WORD;
	return read_ea(buffer, inst.op0, MEM_ALT, mode, reg, Size::WORD);
}

int Inst_branch(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	int8_t disp8 = (int8_t)(header & 0xff);

	inst.op0.type = PC_DISP;
	if (disp8 == 0)
	{
		// 16-bit offset
		uint16_t disp16;
		if (buffer.read_word(disp16))
			return 1;
		inst.op0.pc_disp.disp = (int16_t)disp16;
		inst.suffix = Suffix::WORD;
	}
	else
	{
		inst.op0.pc_disp.disp = disp8;
		inst.suffix = Suffix::SHORT;
	}
	return 0;
}

int Inst_ext(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	// NOTE: this needs to be changed if handling ext byte->long
	uint8_t mode = (header >> 6) && 1;
	inst.suffix = (mode == 0) ? Suffix::WORD : Suffix::LONG;
	uint8_t reg = (header >> 0) & 7;
	set_dreg(inst.op0, reg);
	return 0;
}

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

int Inst_dbcc(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t dreg  = (header >> 0) & 7;
	uint16_t disp16;
	if (buffer.read_word(disp16))
		return 1;

	set_dreg(inst.op0, dreg);
	inst.op1.type = PC_DISP;
	inst.op1.pc_disp.disp = (int16_t)disp16;
	return 0;
}

int Inst_scc(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;
	return read_ea(buffer, inst.op0, DATA_ALT, mode, reg, Size::BYTE);
}

int Inst_sbcd_reg(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t regx = (header >> 0) & 7;
	uint8_t regy = (header >> 9) & 7;
	set_dreg(inst.op0, regx);
	set_dreg(inst.op1, regy);
	return 0;
}

int Inst_sbcd_predec(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t regx = (header >> 0) & 7;
	uint8_t regy = (header >> 9) & 7;
	set_predec(inst.op0, regx);
	set_predec(inst.op1, regy);
	return 0;
}

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

	if (read_ea(buffer, inst.op0, ALL, mode, reg, ea_size))
		return 1;
	set_areg(inst.op1, areg);
	return 0;
}

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

	if (read_ea(buffer, inst.op0, ALL, mode, reg, ea_size))
		return 1;
	set_dreg(inst.op1, dreg);
	return 0;
}

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
	if (read_ea(buffer, inst.op1, DATA_ALT, mode, reg, ea_size))
		return 1;
	return 0;
}

int Inst_mul(buffer_reader& buffer, instruction& inst, uint32_t header)
{
	uint8_t mode = (header >> 3) & 7;
	uint8_t reg  = (header >> 0) & 7;
	uint8_t dreg = (header >> 9) & 7;
	Size ea_size = Size::WORD;
	if (ea_size == Size::NONE)
		return 1;
	inst.suffix = Suffix::WORD;

	// src is EA
	if (read_ea(buffer, inst.op0, DATA, mode, reg, ea_size))
		return 1;

	// dst is d-reg
	set_dreg(inst.op1, dreg);
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
struct matcher_entry
{
	uint32_t mask0;
	uint32_t val0;
	bool	is32;
	const char* tag;
	pfnDecoderFunc func;
};

#define MATCH_ENTRY1(shift, bitcount, val, is32, tag, func)   \
	{ (((1U<<(bitcount))-1U)<<(shift)), (val<<(shift)), is32, tag, NULL }

#define MATCH_ENTRY2(shift, bitcount, val, shift2, bitcount2, val2, is32, tag, func)   \
	{ (((1U<<(bitcount))-1U)<<(shift)) | (((1U<<(bitcount2))-1U)<<(shift2)), \
		(val<<(shift)) | (val2<<(shift2)), \
	   is32, tag, NULL }

#define MATCH_ENTRY3(shift, bitcount, val, shift2, bitcount2, val2, shift3, bitcount3, val3, is32, tag, func)   \
	{ (((1U<<(bitcount))-1U)<<(shift)) | (((1U<<(bitcount2))-1U)<<(shift2))  | (((1U<<(bitcount3))-1U)<<(shift3)), \
		(val<<(shift)) | (val2<<(shift2)) | (val3<<(shift3)), \
	   is32, tag, NULL }

#define MATCH_ENTRY1_IMPL(shift, bitcount, val, is32, tag, func)   \
	{ (((1U<<(bitcount))-1U)<<(shift)), (val<<(shift)), is32, tag, func }

#define MATCH_ENTRY2_IMPL(shift, bitcount, val, shift2, bitcount2, val2, is32, tag, func)   \
	{ (((1U<<(bitcount))-1U)<<(shift)) | (((1U<<(bitcount2))-1U)<<(shift2)), \
		(val<<(shift)) | (val2<<(shift2)), \
	   is32, tag, func }

#define MATCH_ENTRY3_IMPL(shift, bitcount, val, shift2, bitcount2, val2, shift3, bitcount3, val3, is32, tag, func)   \
	{ (((1U<<(bitcount))-1U)<<(shift)) | (((1U<<(bitcount2))-1U)<<(shift2))  | (((1U<<(bitcount3))-1U)<<(shift3)), \
		(val<<(shift)) | (val2<<(shift2)) | (val3<<(shift3)), \
	   is32, tag, func }

matcher_entry g_matcher_table[] =
{
	//		   SH CT									  F32	Tag				  Decoder
	MATCH_ENTRY1( 8,24, 0b00100011110000000000	   ,	true,  "andi",			  Inst_imm_ccr ),
	MATCH_ENTRY1( 8,24, 0b10100011110000000000	   ,	true,  "eori",			  Inst_imm_ccr ),
	MATCH_ENTRY1( 8,24, 0b000000000011110000000000   ,	true,  "ori",			   Inst_imm_ccr ),
	MATCH_ENTRY1_IMPL( 0,16, 0b0100101011111100		   ,	false, "illegal",		   Inst_simple ),
	MATCH_ENTRY1_IMPL( 0,16, 0b0100111001110000		   ,	false, "reset",			 Inst_simple ), // supervisor
	MATCH_ENTRY1_IMPL( 0,16, 0b0100111001110001		   ,	false, "nop",			   Inst_simple ),
	MATCH_ENTRY1_IMPL( 0,16, 0b0100111001110011		   ,	false, "rte",			   Inst_simple ), // supervisor
	MATCH_ENTRY1_IMPL( 0,16, 0b0100111001110101		   ,	false, "rts",			   Inst_simple ),
	MATCH_ENTRY1_IMPL( 0,16, 0b0100111001110110		   ,	false, "trapv",			 Inst_simple ),
	MATCH_ENTRY1_IMPL( 0,16, 0b0100111001110111		   ,	false, "rtr",			   Inst_simple ),
	MATCH_ENTRY1_IMPL( 0,16, 0b0100111001110010		   ,	false, "stop",			  Inst_stop ),
	MATCH_ENTRY1( 0,16, 0b0000000001111100		   ,	false, "ori",			   Inst_imm_sr ), // supervisor
	MATCH_ENTRY1( 0,16, 0b0000001001111100		   ,	false, "andi",			  Inst_imm_sr ), // supervisor
	MATCH_ENTRY1( 0,16, 0b0000101001111100		   ,	false, "eori",			  Inst_imm_sr ), // supervisor

	MATCH_ENTRY1_IMPL( 3,13, 0b0100100001000			  ,	false, "swap",			  Inst_swap ),
	MATCH_ENTRY1_IMPL( 3,13, 0b0100111001010			  ,	false, "link.w",			Inst_link_w ),
	//([ ( 3,13, 0b0100100000001)			  ,	false, "link.l",			Inst_link_l ),  # not on 68000
	MATCH_ENTRY1_IMPL( 3,13, 0b0100111001011			  ,	false, "unlk",			  Inst_unlk ),
	MATCH_ENTRY1_IMPL( 3,13, 0b0100111001100			  ,	false, "move",			  Inst_move_to_usp ),
	MATCH_ENTRY1_IMPL( 3,13, 0b0100111001101			  ,	false, "move",			  Inst_move_from_usp ),
	MATCH_ENTRY1_IMPL( 3,13, 0b0100100010000			  ,	false, "ext",			 Inst_ext ),
	MATCH_ENTRY1_IMPL( 3,13, 0b0100100011000			  ,	false, "ext",			 Inst_ext ),
	//([ ( 3,13, 0b0100100011000)			  ,	false, "extb.l",			Inst_ext ),	 # not on 68000

	MATCH_ENTRY1_IMPL( 4,12, 0b010011100100			   ,	false, "trap",			  Inst_trap ),

	MATCH_ENTRY1_IMPL( 6,10, 0b0100000011				 ,	false, "move",			  Inst_move_from_sr ),   // supervisor
	MATCH_ENTRY1_IMPL( 6,10, 0b0100011011				 ,	false, "move",			  Inst_move_to_sr ),   // supervisor
	//([ ( 6,10, 0b0100001011)				 ,	false, "move from ccr",	 Inst ),		  # not on 68000
	MATCH_ENTRY1( 6,10, 0b0100010011				 ,	false, "move",			  Inst_move_to_ccr ),
	MATCH_ENTRY1_IMPL( 6,10, 0b0100100000				 ,	false, "nbcd",			  Inst_nbcd ),
	MATCH_ENTRY1_IMPL( 6,10, 0b0100100001				 ,	false, "pea",			   Inst_pea ),
	MATCH_ENTRY1_IMPL( 6,10, 0b0100101011				 ,	false, "tas",			   Inst_tas ),
	MATCH_ENTRY1_IMPL( 6,10, 0b0100111010				 ,	false, "jsr",			   Inst_jump ),
	MATCH_ENTRY1_IMPL( 6,10, 0b0100111011				 ,	false, "jmp",			   Inst_jump ),
	MATCH_ENTRY1_IMPL( 6,10, 0b1110000011				 ,	false, "asr",			   Inst_asl_asr_mem ),
	MATCH_ENTRY1_IMPL( 6,10, 0b1110000111				 ,	false, "asl",			   Inst_asl_asr_mem ),

	MATCH_ENTRY2_IMPL(12, 4, 0b0000, 3, 6, 0b100001	 ,	false, "movep",		   Inst_movep_mem_reg ),
	MATCH_ENTRY2_IMPL(12, 4, 0b0000, 3, 6, 0b101001	 ,	false, "movep",		   Inst_movep_mem_reg ),
	MATCH_ENTRY2_IMPL(12, 4, 0b0000, 3, 6, 0b110001	 ,	false, "movep",		   Inst_movep_reg_mem ),
	MATCH_ENTRY2_IMPL(12, 4, 0b0000, 3, 6, 0b111001	 ,	false, "movep",		   Inst_movep_reg_mem ),
	MATCH_ENTRY1_IMPL(6,10, 0b0000100001	  ,	false,  "bchg",			  Inst_bchg_imm ),
	MATCH_ENTRY1_IMPL(6,10, 0b0000100010	  ,	false,  "bclr",			  Inst_bchg_imm ),
	MATCH_ENTRY1_IMPL(6,10, 0b0000100011	  ,	false,  "bset",			  Inst_bchg_imm ),
	MATCH_ENTRY1_IMPL(6,10, 0b0000100000	  ,	false,  "btst",			  Inst_bchg_imm ),
	MATCH_ENTRY1_IMPL( 7, 9, 0b010010001	 ,	false, "movem",		   Inst_movem_reg_mem ), // Register to memory.
	MATCH_ENTRY1_IMPL( 7, 9, 0b010011001	 ,	false, "movem",		   Inst_movem_mem_reg ), // Memory to register.
	MATCH_ENTRY1_IMPL( 8, 8, 0b00000000				  ,	false, "ori",			   Inst_integer_imm_ea ),
	MATCH_ENTRY1_IMPL( 8, 8, 0b00000010				  ,	false, "andi",			  Inst_integer_imm_ea ),
	MATCH_ENTRY1_IMPL( 8, 8, 0b00000100				   ,	false, "subi",			  Inst_integer_imm_ea ),
	MATCH_ENTRY1_IMPL( 8, 8, 0b00000110				  ,	false, "addi",			  Inst_integer_imm_ea ),
	MATCH_ENTRY1_IMPL( 8, 8, 0b00001010				   ,	false, "eori",			  Inst_integer_imm_ea ),
	MATCH_ENTRY1_IMPL( 8, 8, 0b00001100				   ,	false, "cmpi",			  Inst_integer_imm_ea ),
	MATCH_ENTRY1_IMPL( 8, 8, 0b01000000				   ,	false, "negx",			  Inst_size_ea ),
	MATCH_ENTRY1_IMPL( 8, 8, 0b01000010				   ,	false, "clr",			   Inst_size_ea ),
	MATCH_ENTRY1_IMPL( 8, 8, 0b01000100				   ,	false, "neg",			   Inst_size_ea ),
	MATCH_ENTRY1_IMPL( 8, 8, 0b01000110				   ,	false, "not",			   Inst_size_ea ),
	MATCH_ENTRY1_IMPL( 8, 8, 0b01001010				   ,	false, "tst",			   Inst_size_ea ),
	MATCH_ENTRY1_IMPL( 8, 8, 0b01100000				   ,	false, "bra",			   Inst_branch ),
	MATCH_ENTRY1_IMPL( 8, 8, 0b01100001				   ,	false, "bsr",			   Inst_branch ),
	MATCH_ENTRY1_IMPL( 8, 8, 0b01100010				   ,	false, "bhi",			   Inst_branch ),
	MATCH_ENTRY1_IMPL( 8, 8, 0b01100011				   ,	false, "bls",			   Inst_branch ),
	MATCH_ENTRY1_IMPL( 8, 8, 0b01100100				   ,	false, "bcc",			   Inst_branch ),
	MATCH_ENTRY1_IMPL( 8, 8, 0b01100101				   ,	false, "bcs",			   Inst_branch ),
	MATCH_ENTRY1_IMPL( 8, 8, 0b01100110				   ,	false, "bne",			   Inst_branch ),
	MATCH_ENTRY1_IMPL( 8, 8, 0b01100111				   ,	false, "beq",			   Inst_branch ),
	MATCH_ENTRY1_IMPL( 8, 8, 0b01101000				   ,	false, "bvc",			   Inst_branch ),
	MATCH_ENTRY1_IMPL( 8, 8, 0b01101001				   ,	false, "bvs",			   Inst_branch ),
	MATCH_ENTRY1_IMPL( 8, 8, 0b01101010				   ,	false, "bpl",			   Inst_branch ),
	MATCH_ENTRY1_IMPL( 8, 8, 0b01101011				   ,	false, "bmi",			   Inst_branch ),
	MATCH_ENTRY1_IMPL( 8, 8, 0b01101100				   ,	false, "bge",			   Inst_branch ),
	MATCH_ENTRY1_IMPL( 8, 8, 0b01101101				   ,	false, "blt",			   Inst_branch ),
	MATCH_ENTRY1_IMPL( 8, 8, 0b01101110				   ,	false, "bgt",			   Inst_branch ),
	MATCH_ENTRY1_IMPL( 8, 8, 0b01101111				   ,	false, "ble",			   Inst_branch ),
	
	//MATCH_ENTRY1_IMPL(12, 4, 0b0110					   ,	false, "bcc",			   Inst_branch),

	MATCH_ENTRY2_IMPL(12, 4, 0b0000, 6, 3, 0b101		,	false, "bchg",			  Inst_bchg ),
	MATCH_ENTRY2_IMPL(12, 4, 0b0000, 6, 3, 0b110		,	false, "bclr",			  Inst_bchg ),
	MATCH_ENTRY2_IMPL(12, 4, 0b0000, 6, 3, 0b111		,	false, "bset",			  Inst_bchg ),
	MATCH_ENTRY2_IMPL(12, 4, 0b0000, 6, 3, 0b100		,	false, "btst",			  Inst_bchg ),

	//Table 3-19. Conditional Tests
	// These sneakily take the "001" in the bottom 3 bits to override the EA parts of Scc
	MATCH_ENTRY1_IMPL(3, 13, 0b0101000011001	 ,	false, "dbra",				Inst_dbcc ),
	MATCH_ENTRY1_IMPL(3, 13, 0b0101000111001	 ,	false, "dbf",				 Inst_dbcc ),
	MATCH_ENTRY1_IMPL(3, 13, 0b0101001011001	 ,	false, "dbhi",				Inst_dbcc ),
	MATCH_ENTRY1_IMPL(3, 13, 0b0101001111001	 ,	false, "dbls",				Inst_dbcc ),
	MATCH_ENTRY1_IMPL(3, 13, 0b0101010011001	 ,	false, "dbcc",				Inst_dbcc ),
	MATCH_ENTRY1_IMPL(3, 13, 0b0101010111001	 ,	false, "dbcs",				Inst_dbcc ),
	MATCH_ENTRY1_IMPL(3, 13, 0b0101011011001	 ,	false, "dbne",				Inst_dbcc ),
	MATCH_ENTRY1_IMPL(3, 13, 0b0101011111001	 ,	false, "dbeq",				Inst_dbcc ),
	MATCH_ENTRY1_IMPL(3, 13, 0b0101100011001	 ,	false, "dbvc",				Inst_dbcc ),
	MATCH_ENTRY1_IMPL(3, 13, 0b0101100111001	 ,	false, "dbvs",				Inst_dbcc ),
	MATCH_ENTRY1_IMPL(3, 13, 0b0101101011001	 ,	false, "dbpl",				Inst_dbcc ),
	MATCH_ENTRY1_IMPL(3, 13, 0b0101101111001	 ,	false, "dbmi",				Inst_dbcc ),
	MATCH_ENTRY1_IMPL(3, 13, 0b0101110011001	 ,	false, "dbge",				Inst_dbcc ),
	MATCH_ENTRY1_IMPL(3, 13, 0b0101110111001	 ,	false, "dblt",				Inst_dbcc ),
	MATCH_ENTRY1_IMPL(3, 13, 0b0101111011001	 ,	false, "dbgt",				Inst_dbcc ),
	MATCH_ENTRY1_IMPL(3, 13, 0b0101111111001	 ,	false, "dble",				Inst_dbcc ),

	MATCH_ENTRY1_IMPL(6, 10, 0b0101000011	 ,	false, "st",				 Inst_scc ),
	MATCH_ENTRY1_IMPL(6, 10, 0b0101000111	 ,	false, "sf",				 Inst_scc ),
	MATCH_ENTRY1_IMPL(6, 10, 0b0101001011	 ,	false, "shi",				Inst_scc ),
	MATCH_ENTRY1_IMPL(6, 10, 0b0101001111	 ,	false, "sls",				Inst_scc ),
	MATCH_ENTRY1_IMPL(6, 10, 0b0101010011	 ,	false, "scc",				Inst_scc ),
	MATCH_ENTRY1_IMPL(6, 10, 0b0101010111	 ,	false, "scs",				Inst_scc ),
	MATCH_ENTRY1_IMPL(6, 10, 0b0101011011	 ,	false, "sne",				Inst_scc ),
	MATCH_ENTRY1_IMPL(6, 10, 0b0101011111	 ,	false, "seq",				Inst_scc ),
	MATCH_ENTRY1_IMPL(6, 10, 0b0101100011	 ,	false, "svc",				Inst_scc ),
	MATCH_ENTRY1_IMPL(6, 10, 0b0101100111	 ,	false, "svs",				Inst_scc ),
	MATCH_ENTRY1_IMPL(6, 10, 0b0101101011	 ,	false, "spl",				Inst_scc ),
	MATCH_ENTRY1_IMPL(6, 10, 0b0101101111	 ,	false, "smi",				Inst_scc ),
	MATCH_ENTRY1_IMPL(6, 10, 0b0101110011	 ,	false, "sge",				Inst_scc ),
	MATCH_ENTRY1_IMPL(6, 10, 0b0101110111	 ,	false, "slt",				Inst_scc ),
	MATCH_ENTRY1_IMPL(6, 10, 0b0101111011	 ,	false, "sgt",				Inst_scc ),
	MATCH_ENTRY1_IMPL(6, 10, 0b0101111111	 ,	false, "sle",				Inst_scc ),

	MATCH_ENTRY2_IMPL(12, 4, 0b0101, 8, 1, 0b1		  ,	false, "subq",			  Inst_subq ),
	MATCH_ENTRY2_IMPL(12, 4, 0b0101, 8, 1, 0b0		  ,	false, "addq",			  Inst_subq ),

	MATCH_ENTRY2_IMPL(12, 4, 0b0111, 8, 1, 0b0	 ,	false, "moveq",			 Inst_moveq ),

	MATCH_ENTRY2_IMPL(12, 4, 0b1000, 3, 6, 0b100000	 ,	false, "sbcd",			  Inst_sbcd_reg ),
	MATCH_ENTRY2_IMPL(12, 4, 0b1000, 3, 6, 0b100001	 ,	false, "sbcd",			  Inst_sbcd_predec ),
	MATCH_ENTRY2_IMPL(12, 4, 0b1100, 3, 6, 0b100000	 ,	false, "abcd",			  Inst_sbcd_reg ),
	MATCH_ENTRY2_IMPL(12, 4, 0b1100, 3, 6, 0b100001	 ,	false, "abcd",			  Inst_sbcd_predec ),

	MATCH_ENTRY3_IMPL(12, 4, 0b1001, 8, 1, 1, 3, 3, 0 ,	false, "subx",			  Inst_subx_reg ),
	MATCH_ENTRY3_IMPL(12, 4, 0b1001, 8, 1, 1, 3, 3, 1 ,	false, "subx",			  Inst_subx_predec ),
	MATCH_ENTRY3_IMPL(12, 4, 0b1101, 8, 1, 1, 3, 3, 0 ,	false, "addx",			  Inst_subx_reg ),
	MATCH_ENTRY3_IMPL(12, 4, 0b1101, 8, 1, 1, 3, 3, 1 ,	false, "addx",			  Inst_subx_predec ),

	MATCH_ENTRY3_IMPL(12, 4, 0b1011, 8,1,1, 3,3,1 ,		false, "cmpm",			  Inst_cmpm ),
	MATCH_ENTRY2_IMPL(12, 4, 0b1011, 6,2,3,				false, "cmpa",			  Inst_cmpa ),

	// Nasty case where eor and cmp mirror one another
	MATCH_ENTRY2_IMPL(12, 4, 0b1011, 6, 3, 0b100		,	false, "eor",			   Inst_eor ),
	MATCH_ENTRY2_IMPL(12, 4, 0b1011, 6, 3, 0b101		,	false, "eor",			   Inst_eor ),
	MATCH_ENTRY2_IMPL(12, 4, 0b1011, 6, 3, 0b110		,	false, "eor",			   Inst_eor ),
	MATCH_ENTRY2_IMPL(12, 4, 0b1100, 6, 3, 0b011		,	false, "mulu",			Inst_mul ),
	MATCH_ENTRY2_IMPL(12, 4, 0b1100, 6, 3, 0b111		,	false, "muls",			Inst_mul ),

	MATCH_ENTRY3_IMPL(12, 4, 0b1110, 3, 2, 0, 8, 1, 1 ,	false, "asl",			   Inst_shift_reg ),
	MATCH_ENTRY3_IMPL(12, 4, 0b1110, 3, 2, 0, 8, 1, 0 ,	false, "asr",			   Inst_shift_reg ),
	MATCH_ENTRY3_IMPL(12, 4, 0b1110, 3, 2, 1, 8, 1, 1 ,	false, "lsl",			   Inst_shift_reg ),
	MATCH_ENTRY3_IMPL(12, 4, 0b1110, 3, 2, 1, 8, 1, 0 ,	false, "lsr",			   Inst_shift_reg ),
	MATCH_ENTRY3_IMPL(12, 4, 0b1110, 3, 2, 2, 8, 1, 1 ,	false, "roxl",			  Inst_shift_reg ),
	MATCH_ENTRY3_IMPL(12, 4, 0b1110, 3, 2, 2, 8, 1, 0 ,	false, "roxr",			  Inst_shift_reg ),
	MATCH_ENTRY3_IMPL(12, 4, 0b1110, 3, 2, 3, 8, 1, 1 ,	false, "rol",			   Inst_shift_reg ),
	MATCH_ENTRY3_IMPL(12, 4, 0b1110, 3, 2, 3, 8, 1, 0 ,	false, "ror",			   Inst_shift_reg ),

	MATCH_ENTRY2_IMPL(12, 4, 0b0100, 6, 3, 0b111   ,	false, "lea",			   Inst_lea ),
	MATCH_ENTRY2(12, 4, 0b1000, 6, 3, 0b111		,	false, "divs.w",			Inst_div ),
	MATCH_ENTRY2(12, 4, 0b1000, 6, 3, 0b011		,	false, "divu.w",			Inst_div ),
	MATCH_ENTRY2(12, 4, 0b0100, 6, 1, 0			,	false, "chk",			   Inst_chk ),
	MATCH_ENTRY2(12, 4, 0b1100, 3, 6, 0b101000	 ,	false, "exg",			   Inst_exg_dd ),
	MATCH_ENTRY2(12, 4, 0b1100, 3, 6, 0b101001	 ,	false, "exg",			   Inst_exg_aa ),
	MATCH_ENTRY2(12, 4, 0b1100, 3, 6, 0b110001	 ,	false, "exg",			   Inst_exg_da ),

	//MATCH_ENTRY2_IMPL(12, 4, 0b1000, 6, 2, 0b11,   ,	false, "ora",				Inst_addsuba ),
	MATCH_ENTRY2_IMPL(12, 4, 0b1001, 6, 2, 0b11   ,	false, "suba",				Inst_addsuba ),
	MATCH_ENTRY2_IMPL(12, 4, 0b1101, 6, 2, 0b11   ,	false, "adda",				Inst_addsuba ),

	// Fallback generics
	MATCH_ENTRY1_IMPL(12, 4, 0b1011					   ,	false, "cmp",			   Inst_cmp ),
	
	// Following where src/dest is d-register 
	MATCH_ENTRY1_IMPL(12, 4, 0b1000					   ,	false, "or",		   Inst_alu_dreg ),
	MATCH_ENTRY1_IMPL(12, 4, 0b1001					   ,	false, "sub",		  Inst_alu_dreg ),
	MATCH_ENTRY1_IMPL(12, 4, 0b1101					   ,	false, "add",		  Inst_alu_dreg ),
	MATCH_ENTRY1_IMPL(12, 4, 0b1100					   ,	false, "and",		  Inst_alu_dreg ),

	MATCH_ENTRY2_IMPL(13, 3, 0b001, 6, 3, 0b001		   ,	false, "movea",			 Inst_movea ),
	MATCH_ENTRY1_IMPL(14, 2, 0b00						 ,	false, "move",			  Inst_move ),

	{ 0 }			 // end sentinel
};

// ----------------------------------------------------------------------------
static const char* g_reg_names[] = 
{
	"d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7",
	"a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7"
};

// ----------------------------------------------------------------------------
void print(const operand& operand, FILE* pFile)
{
	switch (operand.type)
	{
		case OpType::D_DIRECT:
			fprintf(pFile, "d%d", operand.d_register.reg);
			return;
		case OpType::A_DIRECT:
			fprintf(pFile, "a%d", operand.a_register.reg);
			return;
		case OpType::INDIRECT:
			fprintf(pFile, "(a%d)", operand.indirect.reg);
			return;
		case OpType::INDIRECT_POSTINC:
			fprintf(pFile, "(a%d)+", operand.indirect_postinc.reg);
			return;
		case OpType::INDIRECT_PREDEC:
			fprintf(pFile, "-(a%d)", operand.indirect_predec.reg);
			return;
		case OpType::INDIRECT_DISP:
			fprintf(pFile, "%d(a%d)", operand.indirect_disp.disp, operand.indirect_disp.reg);
			return;
		case OpType::INDIRECT_INDEX:
			fprintf(pFile, "%d(a%d,d%d.%s)",
					operand.indirect_index.disp,
					operand.indirect_index.a_reg,
					operand.indirect_index.d_reg,
					operand.indirect_index.is_long ? "l" : "w");
			return;
		case OpType::ABSOLUTE_WORD:
			if (operand.absolute_word.wordaddr & 0x8000)
				fprintf(pFile, "$ffff%x.w", operand.absolute_word.wordaddr);
			else
				fprintf(pFile, "$%x.w", operand.absolute_word.wordaddr);
			return;
		case OpType::ABSOLUTE_LONG:
			fprintf(pFile, "$%x.l",
					operand.absolute_long.longaddr);
			return;
		case OpType::PC_DISP:
			fprintf(pFile, "%d(pc)", operand.pc_disp.disp);
			return;
		case OpType::PC_DISP_INDEX:
			fprintf(pFile, "%d(pc,d%d.%s)",
					operand.pc_disp_index.disp,
					operand.pc_disp_index.d_reg,
					operand.pc_disp_index.is_long ? "l" : "w");
			return;
		case OpType::MOVEM_REG:
		{
			bool first = true;
			for (int i = 0; i < 16; ++i)
				if (operand.movem_reg.reg_mask & (1 << i))
				{
					if (!first)
						fprintf(pFile, "/");
					fprintf(pFile, "%s", g_reg_names[i]);
					first = false;
				}
			return;
		}

		case OpType::IMMEDIATE:
			fprintf(pFile, "#$%x", operand.imm.val0);
			return;
		case OpType::SR:
			fprintf(pFile, "sr");
			return;
		case OpType::USP:
			fprintf(pFile, "usp");
			return;
		default:
			fprintf(pFile, "???");
	}
}

// ----------------------------------------------------------------------------
void print(const instruction& inst, FILE* pFile)
{
	if (!inst.tag)
	{
		fprintf(pFile, "dc.w $%x", inst.header);
		return;
	}
	fprintf(pFile, "%s", inst.tag);

	switch (inst.suffix)
	{
		case Suffix::BYTE:
			fprintf(pFile, ".b"); break;
		case Suffix::WORD:
			fprintf(pFile, ".w"); break;
		case Suffix::LONG:
			fprintf(pFile, ".l"); break;
		case Suffix::SHORT:
			fprintf(pFile, ".s"); break;
		default:
			break;
	}

	if (inst.op0.type == OpType::kNone)
		return;
	fprintf(pFile, " ");
	print(inst.op0, pFile);

	if (inst.op1.type == OpType::kNone)
		return;
	fprintf(pFile, ",");
	print(inst.op1, pFile);
}
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

	if (buffer.get_remain() >= 2)
	{
		buffer.read_word(header1);
		has32 = true;
	}

	for (const matcher_entry* pEntry = g_matcher_table;
		pEntry->mask0 != 0;
		++pEntry)
	{
		// Check size first
		if (pEntry->is32 && !has32)
			continue;

		// Choose 16 or 32 bits for the check
		uint32_t header = header0;
		if (pEntry->is32)
		{
			header <<= 16;
			header |= header1;
		}

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

// ----------------------------------------------------------------------------
struct tos_header
{
	   //  See http://toshyp.atari.org/en/005005.html for TOS header details
	uint16_t  ph_branch;	  /* Branch to start of the program  */
							  /* (must be 0x601a!)			   */

	uint32_t  ph_tlen;		  /* Length of the TEXT segment	  */
	uint32_t  ph_dlen;		  /* Length of the DATA segment	  */
	uint32_t  ph_blen;		  /* Length of the BSS segment	   */
	uint32_t  ph_slen;		  /* Length of the symbol table	  */
	uint32_t  ph_res1;		  /* Reserved, should be 0;		  */
							  /* Required by PureC			   */
	uint32_t  ph_prgflags;	  /* Program flags				   */
	uint16_t  ph_absflag;	  /* 0 = Relocation info present	 */
};

// ----------------------------------------------------------------------------
int process_tos_file(const uint8_t* pData, long size)
{
	buffer_reader buf(pData, size);
	tos_header header = {};

	if (buf.read_word(header.ph_branch))
		return 1;
	if (buf.read_long(header.ph_tlen))
		return 1;
	if (buf.read_long(header.ph_dlen))
		return 1;
	if (buf.read_long(header.ph_blen))
		return 1;
	if (buf.read_long(header.ph_slen))
		return 1;
	if (buf.read_long(header.ph_res1))
		return 1;
	if (buf.read_long(header.ph_prgflags))
		return 1;
	if (buf.read_word(header.ph_absflag))
		return 1;

	if (header.ph_branch != 0x601a)
		return 1;

	// Next section is text
	fprintf(stdout, "Reading text section\n");

	while (buf.get_remain() >= 2)
	{
		buffer_reader buf_copy2(buf);
		uint16_t tmp = 0;
		buf_copy2.read_word(tmp);
		printf(">> %04x:   $%04x ", buf.get_pos(), tmp);
		
		buffer_reader buf_copy = buf;

		instruction inst;
		int res = decode(buf_copy, inst);

		if (res == 0)
			print(inst, stdout);
		else
			printf("??");
		printf("\n");
		
		// Move to next
		buf.advance(inst.byte_count);
	}

	return 0;
}

// ----------------------------------------------------------------------------
int main(int argc, char** argv)
{
	if (argc < 2)
	{
		fprintf(stderr, "No filename\n");
		return 1;
	}
	FILE* pInfile = fopen(argv[1], "rb");
	if (!pInfile)
	{
		fprintf(stderr, "Can't read file\n");
		return 1;
	}

	int r = fseek(pInfile, 0, SEEK_END);
	long size = ftell(pInfile);

	rewind(pInfile);

	uint8_t* pData = (uint8_t*) malloc(size);
	int readBytes = fread(pData, 1, size, pInfile);
	printf("Read %d bytes\n", readBytes);
	fclose(pInfile);

	return process_tos_file(pData, size);
}
