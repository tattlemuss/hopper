// Sample functions to write an instruction to a file pointer.
// It should be easy to update this to write to other text streams, or
// representations that suit the use-case.
#include "print.h"

#include "lib/instruction68.h"
#include "symbols.h"

// ----------------------------------------------------------------------------
//	INSTRUCTION DISPLAY FORMATTING
// ----------------------------------------------------------------------------

bool calc_relative_address(const hop68::operand& op, uint32_t inst_address, uint32_t& target_address)
{
	if (op.type == hop68::PC_DISP)
	{
		target_address = inst_address + op.pc_disp.inst_disp;
		return true;
	}
	else if (op.type == hop68::PC_DISP_INDEX)
	{
		target_address = inst_address + op.pc_disp_index.inst_disp;
		return true;
	}
	else if (op.type == hop68::RELATIVE_BRANCH)
	{
		target_address = inst_address + op.relative_branch.inst_disp;
		return true;
	}
	else if (op.type == hop68::INDIRECT_POSTINDEXED || op.type == hop68::INDIRECT_PREINDEXED ||
			 op.type == hop68::MEMORY_INDIRECT || op.type == hop68::NO_MEMORY_INDIRECT)
	{
		if (op.indirect_index_68020.base_register == hop68::INDEX_REG_PC)
		{
			target_address = inst_address + op.indirect_index_68020.base_displacement;
			return true;
		}
	}
	return false;
}

// ----------------------------------------------------------------------------
static int print_index_indirect(const hop68::index_indirect& ind, FILE* pFile)
{
	if (ind.index_reg == hop68::INDEX_REG_NONE)
		return 0;
	return fprintf(pFile, "%s.%s%s",
		hop68::get_index_register_string(ind.index_reg),
		ind.is_long ? "l" : "w",
		hop68::get_scale_shift_string(ind.scale_shift));
}

// ----------------------------------------------------------------------------
static int print_bitfield_number(uint8_t is_reg, uint8_t offset, FILE* pFile)
{
	if (is_reg)
		return fprintf(pFile, "d%u", offset & 7);
	else
		return fprintf(pFile, "%u", offset);
}

// ----------------------------------------------------------------------------
static int print_bitfield(const hop68::bitfield& bf, FILE* pFile)
{
	int count = 0;
	count += fprintf(pFile, "{");
	print_bitfield_number(bf.offset_is_dreg, bf.offset, pFile);
	count += fprintf(pFile, ":");
	print_bitfield_number(bf.width_is_dreg, bf.width, pFile);
	count += fprintf(pFile, "}");
	return count;
}

// ----------------------------------------------------------------------------
enum LastOutput
{
	kNone,			// start token
	kValue,			// number or similar
	kComma,			// ','
	kOpenBrace,		// '['
	kCloseBrace		// ']'
};

// ----------------------------------------------------------------------------
static int open_brace(LastOutput& last, bool& is_brace_open, FILE* pFile)
{
	if (!is_brace_open)
	{
		is_brace_open = true;
		last = kOpenBrace;
		return fprintf(pFile, "[");
	}
	return 0;
}

// ----------------------------------------------------------------------------
static int close_brace(LastOutput& last, bool& is_brace_open, FILE* pFile)
{
	if (is_brace_open)
	{
		last = kCloseBrace;
		return fprintf(pFile, "]");
	}
	//else
	//{
	//	// Insert an empty region
	//	fprintf(pFile, "[0]");
	//	last = kCloseBrace;
	//}
	is_brace_open = false;
	return 0;
}

// ----------------------------------------------------------------------------
static int insert_comma(LastOutput& last, FILE* pFile)
{
	if (last == kValue || last == kCloseBrace)
	{
		last = kComma;
		return fprintf(pFile, ",");
	}
	return 0;
}

// ----------------------------------------------------------------------------
static int print_indexed_68020(const hop68::indirect_index_full& ref, const symbols& symbols,
	int brace_open, int brace_close, uint32_t inst_address, FILE* pFile)
{
	int count = 0;
	count += fprintf(pFile, "(");
	LastOutput last = kNone;
	bool is_brace_open = false;
	symbol sym;
	for (int index = 0; index < 4; ++index)
	{
		if (ref.used[index])
		{
			// if an item is printed, we might need to open a brace or
			// insert a separating comma
			if (index >= brace_open && index <= brace_close)
				count += open_brace(last, is_brace_open, pFile);
			count += insert_comma(last, pFile);
			switch (index)
			{
				case 0:
					if (ref.base_register == hop68::IndexRegister::INDEX_REG_PC)
					{
						// Decode PC-relative addresses
						uint32_t address = ref.base_displacement + inst_address;
						if (find_symbol(symbols, address, sym))
							count += fprintf(pFile, "%s", sym.label.c_str());
						else
							count += fprintf(pFile, "$%x", address);
					}
					else
						count += fprintf(pFile, "$%x", ref.base_displacement);
					break;
				case 1:
					count += fprintf(pFile, "%s", hop68::get_index_register_string(ref.base_register)); break;
				case 2:
					count += print_index_indirect(ref.index, pFile); break;
				case 3:
					count += fprintf(pFile, "$%x", ref.outer_displacement); break;
			}
			last = kValue;;
		}
		// Brace might need to be closed whether even if a new value wasn't printed
		if (index == brace_close)
			count += close_brace(last, is_brace_open, pFile);
	}
	count += fprintf(pFile, ")");
	return count;
}

// ----------------------------------------------------------------------------
int print(const hop68::operand& operand, const symbols& symbols, uint32_t inst_address, FILE* pFile)
{
	int count = 0;
	switch (operand.type)
	{
		case hop68::OpType::D_DIRECT:
			return fprintf(pFile, "d%d", operand.d_register.reg);
		case hop68::OpType::A_DIRECT:
			return fprintf(pFile, "a%d", operand.a_register.reg);
		case hop68::OpType::INDIRECT:
			return fprintf(pFile, "(a%d)", operand.indirect.reg);
		case hop68::INDIRECT_POSTINC:
			return fprintf(pFile, "(a%d)+", operand.indirect_postinc.reg);
		case hop68::OpType::INDIRECT_PREDEC:
			return fprintf(pFile, "-(a%d)", operand.indirect_predec.reg);
		case hop68::OpType::INDIRECT_DISP:
			return fprintf(pFile, "%d(a%d)", operand.indirect_disp.disp, operand.indirect_disp.reg);
		case hop68::OpType::INDIRECT_INDEX:
			count += fprintf(pFile, "%d(a%d,",
					operand.indirect_index.disp,
					operand.indirect_index.a_reg);
			count += print_index_indirect(operand.indirect_index.indirect_info, pFile);
			count += fprintf(pFile, ")");
			return count;
		case hop68::OpType::ABSOLUTE_WORD:
			if (operand.absolute_word.wordaddr & 0x8000)
				return fprintf(pFile, "$ffff%x.w", operand.absolute_word.wordaddr);
			else
				return fprintf(pFile, "$%x.w", operand.absolute_word.wordaddr);
		case hop68::OpType::ABSOLUTE_LONG:
		{
			symbol sym;
			if (find_symbol(symbols, operand.absolute_long.longaddr, sym))
				return fprintf(pFile, "%s", sym.label.c_str());
			else
				return fprintf(pFile, "$%x.l",
					operand.absolute_long.longaddr);
		}
		case hop68::OpType::PC_DISP:
		{
			symbol sym;
			uint32_t target_address;
			calc_relative_address(operand, inst_address, target_address);
			if (find_symbol(symbols, target_address, sym))
				return fprintf(pFile, "%s(pc)", sym.label.c_str());
			else
				return fprintf(pFile, "$%x(pc)", target_address);
		}
		case hop68::OpType::PC_DISP_INDEX:
		{
			symbol sym;
			uint32_t target_address;
			calc_relative_address(operand, inst_address, target_address);

			if (find_symbol(symbols, target_address, sym))
			{
				return fprintf(pFile, "%s(pc,%s.%s%s)",
						sym.label.c_str(),
						hop68::get_index_register_string(operand.pc_disp_index.indirect_info.index_reg),
						operand.pc_disp_index.indirect_info.is_long ? "l" : "w",
						hop68::get_scale_shift_string(operand.pc_disp_index.indirect_info.scale_shift));
			}
			else
			{
				return fprintf(pFile, "$%x(pc,%s.%s%s)",
					target_address,
					hop68::get_index_register_string(operand.pc_disp_index.indirect_info.index_reg),
					operand.pc_disp_index.indirect_info.is_long ? "l" : "w",
					hop68::get_scale_shift_string(operand.pc_disp_index.indirect_info.scale_shift));
			}
		}
		case hop68::OpType::MOVEM_REG:
		{
			bool first = true;
			for (int i = 0; i < 16; ++i)
				if (operand.movem_reg.reg_mask & (1 << i))
				{
					if (!first)
						count += fprintf(pFile, "/");
					count += fprintf(pFile, "%s", hop68::get_movem_reg_string(i));
					first = false;
				}
			return count;
		}
		case hop68::OpType::RELATIVE_BRANCH:
		{
			symbol sym;
			uint32_t target_address;
			calc_relative_address(operand, inst_address, target_address);
			if (find_symbol(symbols, target_address, sym))
				return fprintf(pFile, "%s", sym.label.c_str());
			else
				return fprintf(pFile, "$%x", target_address);
		}
		case hop68::OpType::INDIRECT_POSTINDEXED:
			return print_indexed_68020(operand.indirect_index_68020, symbols, 0, 1, inst_address, pFile);
		case hop68::OpType::INDIRECT_PREINDEXED:
			return print_indexed_68020(operand.indirect_index_68020, symbols, 0, 2, inst_address, pFile);
		case hop68::OpType::MEMORY_INDIRECT:
			// This is the same as postindexed, except IS is suppressed!
			return print_indexed_68020(operand.indirect_index_68020, symbols, 0, 1, inst_address, pFile);
		case hop68::OpType::NO_MEMORY_INDIRECT:
			return print_indexed_68020(operand.indirect_index_68020, symbols, -1, -1, inst_address, pFile);
		case hop68::OpType::IMMEDIATE:
		{
			// Special case: show long immediates as labels, if we know a reloc
			// has taken place here.
			// We know the reloc has to be at +2 in the instruction, since it is
			// always a source operand for immediates.
			uint32_t target = 0;
			if (operand.imm.size == hop68::Size::LONG &&
				find_reloc(symbols, inst_address + 2, target))
			{
				symbol sym;
				if (target == operand.imm.val0 &&
					find_symbol(symbols, operand.imm.val0, sym))
				{
					return fprintf(pFile, "#%s", sym.label.c_str());
				}
			}
			if (operand.imm.is_signed && (int32_t)operand.imm.val0 < 0)
				return fprintf(pFile, "#-$%x", -(int32_t)operand.imm.val0);
			else
				return fprintf(pFile, "#$%x", operand.imm.val0);
		}
		case hop68::OpType::D_REGISTER_PAIR:
			return fprintf(pFile, "d%u:d%u", operand.d_register_pair.dreg1, operand.d_register_pair.dreg2);
		case hop68::OpType::INDIRECT_REGISTER_PAIR:
			return fprintf(pFile, "(%s):(%s)",
				hop68::get_index_register_string(operand.indirect_register_pair.reg1),
				hop68::get_index_register_string(operand.indirect_register_pair.reg2));
		case hop68::OpType::SR:
			return fprintf(pFile, "sr");
		case hop68::OpType::USP:
			return fprintf(pFile, "usp");
		case hop68::OpType::CCR:
			return fprintf(pFile, "ccr");
		case hop68::OpType::CONTROL_REGISTER:
			return fprintf(pFile, "%s", hop68::get_control_register_string(operand.control_register.cr));
		default:
			return fprintf(pFile, "???");
	}
	return 0;
}

// ----------------------------------------------------------------------------
unsigned char interpret_ascii(unsigned char i)
{
	if (i >= 32 && i < 128)
		return i;
	return '.';
}

// ----------------------------------------------------------------------------
int print(const hop68::instruction& inst, const symbols& symbols, uint32_t inst_address, FILE* pFile)
{
	int count = 0;
	if (inst.opcode == hop68::Opcode::NONE)
	{
		count += fprintf(pFile, "dc.w     $%04x  ; %c%c", inst.header,
			interpret_ascii(inst.header >> 8),
			interpret_ascii(inst.header & 0xff));
		return count;
	}
	int len = fprintf(pFile, "%s%s", hop68::get_opcode_string(inst.opcode),
				hop68::get_suffix_string(inst.suffix));
	count += len;
	if (inst.op0.type == hop68::OpType::INVALID)
		return count; // early out with no operands, avoids trailing spaces

	while (len++ < 9)
		count += fprintf(pFile, " ");

	count += print(inst.op0, symbols, inst_address, pFile);

	if (inst.bf0.valid)
		count += print_bitfield(inst.bf0, pFile);

	if (inst.op1.type != hop68::OpType::INVALID)
	{
		count += fprintf(pFile, ",");
		count += print(inst.op1, symbols, inst_address, pFile);
	}
	if (inst.bf1.valid)
		count += print_bitfield(inst.bf1, pFile);

	if (inst.op2.type != hop68::OpType::INVALID)
	{
		count += fprintf(pFile, ",");
		count += print(inst.op2, symbols, inst_address, pFile);
	}
	return count;
}

