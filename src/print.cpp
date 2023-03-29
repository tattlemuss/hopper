// Sample functions to write an instruction to a file pointer.
// It should be easy to update this to write to other text streams, or
// representations that suit the use-case.
#include "print.h"

#include "instruction.h"
#include "symbols.h"

// ----------------------------------------------------------------------------
//	INSTRUCTION DISPLAY FORMATTING
// ----------------------------------------------------------------------------

bool calc_relative_address(const operand& op, uint32_t inst_address, uint32_t& target_address)
{
	if (op.type == PC_DISP)
	{
		target_address = inst_address + op.pc_disp.inst_disp;
		return true;
	}
	else if (op.type == PC_DISP_INDEX)
	{
		target_address = inst_address + op.pc_disp_index.inst_disp;
		return true;
	}
	if (op.type == RELATIVE_BRANCH)
	{
		target_address = inst_address + op.relative_branch.inst_disp;
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
static void print_index_indirect(const index_indirect& ind, FILE* pFile)
{
	if (ind.index_reg == INDEX_REG_NONE)
		return;
	fprintf(pFile, "%s.%s%s",
		get_index_register_string(ind.index_reg),
		ind.is_long ? "l" : "w",
		get_scale_shift_string(ind.scale_shift));
}

// ----------------------------------------------------------------------------
static void print_bitfield_number(uint8_t is_reg, uint8_t offset, FILE* pFile)
{
	if (is_reg)
		fprintf(pFile, "d%u", offset & 7);
	else
		fprintf(pFile, "%u", offset);
}

// ----------------------------------------------------------------------------
static void print_bitfield(const bitfield& bf, FILE* pFile)
{
	fprintf(pFile, "{");
	print_bitfield_number(bf.offset_is_dreg, bf.offset, pFile);
	fprintf(pFile, ":");
	print_bitfield_number(bf.width_is_dreg, bf.width, pFile);
	fprintf(pFile, "}");
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
static void open_brace(LastOutput& last, bool& is_brace_open, FILE* pFile)
{
	if (!is_brace_open)
	{
		fprintf(pFile, "[");
		is_brace_open = true;
		last = kOpenBrace;
	}
}

// ----------------------------------------------------------------------------
static void close_brace(LastOutput& last, bool& is_brace_open, FILE* pFile)
{
	if (is_brace_open)
	{
		fprintf(pFile, "]");
		last = kCloseBrace;
	}
	//else
	//{
	//	// Insert an empty region
	//	fprintf(pFile, "[0]");
	//	last = kCloseBrace;
	//}
	is_brace_open = false;
}

// ----------------------------------------------------------------------------
static void insert_comma(LastOutput& last, FILE* pFile)
{
	if (last == kValue || last == kCloseBrace)
	{
		fprintf(pFile, ",");
		last = kComma;
	}
}

// ----------------------------------------------------------------------------
static void print_indexed_68020(const indirect_index_full& ref, const symbols& symbols, 
	int brace_open, int brace_close, uint32_t inst_address, FILE* pFile)
{
	fprintf(pFile, "(");
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
				open_brace(last, is_brace_open, pFile);
			insert_comma(last, pFile);
			switch (index)
			{
				case 0:
					if (ref.base_register == IndexRegister::INDEX_REG_PC)
					{
						// Decode PC-relative addresses
						uint32_t address = ref.base_displacement + inst_address;
						if (find_symbol(symbols, address, sym))
							fprintf(pFile, "%s", sym.label.c_str());
						else
							fprintf(pFile, "$%x", address);
					}
					else
						fprintf(pFile, "$%x", ref.base_displacement);
					break;
				case 1:
					fprintf(pFile, "%s", get_index_register_string(ref.base_register)); break;
				case 2:
					print_index_indirect(ref.index, pFile); break;
				case 3:
					fprintf(pFile, "$%x", ref.outer_displacement); break;
			}
			last = kValue;
		}
		// Brace might need to be closed whether even if a new value wasn't printed
		if (index == brace_close)
			close_brace(last, is_brace_open, pFile);
	}
	fprintf(pFile, ")");
}

// ----------------------------------------------------------------------------
void print(const operand& operand, const symbols& symbols, uint32_t inst_address, FILE* pFile)
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
			fprintf(pFile, "%d(a%d,",
					operand.indirect_index.disp,
					operand.indirect_index.a_reg);
			print_index_indirect(operand.indirect_index.indirect_info, pFile);
			fprintf(pFile, ")");
			return;
		case OpType::ABSOLUTE_WORD:
			if (operand.absolute_word.wordaddr & 0x8000)
				fprintf(pFile, "$ffff%x.w", operand.absolute_word.wordaddr);
			else
				fprintf(pFile, "$%x.w", operand.absolute_word.wordaddr);
			return;
		case OpType::ABSOLUTE_LONG:
		{
			symbol sym;
			if (find_symbol(symbols, operand.absolute_long.longaddr, sym))
				fprintf(pFile, "%s", sym.label.c_str());
			else
				fprintf(pFile, "$%x.l",
					operand.absolute_long.longaddr);
			return;
		}
		case OpType::PC_DISP:
		{
			symbol sym;
			uint32_t target_address;
			calc_relative_address(operand, inst_address, target_address);
			if (find_symbol(symbols, target_address, sym))
				fprintf(pFile, "%s(pc)", sym.label.c_str());
			else
				fprintf(pFile, "$%x(pc)", target_address);
			return;
		}
		case OpType::PC_DISP_INDEX:
		{
			symbol sym;
			uint32_t target_address;
			calc_relative_address(operand, inst_address, target_address);

			if (find_symbol(symbols, target_address, sym))
			{
				fprintf(pFile, "%s(pc,%s.%s%s)",
						sym.label.c_str(),
						get_index_register_string(operand.pc_disp_index.indirect_info.index_reg),
						operand.pc_disp_index.indirect_info.is_long ? "l" : "w",
						get_scale_shift_string(operand.pc_disp_index.indirect_info.scale_shift));
			}
			else
			{
				fprintf(pFile, "$%x(pc,%s.%s%s)",
					target_address,
					get_index_register_string(operand.pc_disp_index.indirect_info.index_reg),
					operand.pc_disp_index.indirect_info.is_long ? "l" : "w",
					get_scale_shift_string(operand.pc_disp_index.indirect_info.scale_shift));
			}
			return;
		}
		case OpType::MOVEM_REG:
		{
			bool first = true;
			for (int i = 0; i < 16; ++i)
				if (operand.movem_reg.reg_mask & (1 << i))
				{
					if (!first)
						fprintf(pFile, "/");
					fprintf(pFile, "%s", get_movem_reg_string(i));
					first = false;
				}
			return;
		}
		case OpType::RELATIVE_BRANCH:
		{
			symbol sym;
			uint32_t target_address;
			calc_relative_address(operand, inst_address, target_address);
			if (find_symbol(symbols, target_address, sym))
				fprintf(pFile, "%s", sym.label.c_str());
			else
				fprintf(pFile, "$%x", target_address);
			return;
		}
		case OpType::INDIRECT_POSTINDEXED:
			print_indexed_68020(operand.indirect_index_68020, symbols, 0, 1, inst_address, pFile);
			return;
		case OpType::INDIRECT_PREINDEXED:
			print_indexed_68020(operand.indirect_index_68020, symbols, 0, 2, inst_address, pFile);
			return;
		case OpType::MEMORY_INDIRECT:
			// This is the same as postindexed, except IS is suppressed!
			print_indexed_68020(operand.indirect_index_68020, symbols, 0, 1, inst_address, pFile);
			return;
		case OpType::NO_MEMORY_INDIRECT:
			print_indexed_68020(operand.indirect_index_68020, symbols, -1, -1, inst_address, pFile);
			return;
		case OpType::IMMEDIATE:
			if (operand.imm.is_signed && (int32_t)operand.imm.val0 < 0)
				fprintf(pFile, "#-$%x", -(int32_t)operand.imm.val0);
			else
				fprintf(pFile, "#$%x", operand.imm.val0);
			return;
		case OpType::D_REGISTER_PAIR:
			fprintf(pFile, "d%u:d%u", operand.d_register_pair.dreg1, operand.d_register_pair.dreg2);
			return;
		case OpType::INDIRECT_REGISTER_PAIR:
			fprintf(pFile, "(%s):(%s)",
				get_index_register_string(operand.indirect_register_pair.reg1),
				get_index_register_string(operand.indirect_register_pair.reg2));
			return;
		case OpType::SR:
			fprintf(pFile, "sr");
			return;
		case OpType::USP:
			fprintf(pFile, "usp");
			return;
		case OpType::CCR:
			fprintf(pFile, "ccr");
			return;
		case OpType::CONTROL_REGISTER:
			fprintf(pFile, "%s", get_control_register_string(operand.control_register.cr));
			return;
		default:
			fprintf(pFile, "???");
	}
}

// ----------------------------------------------------------------------------
void print(const instruction& inst, const symbols& symbols, uint32_t inst_address, FILE* pFile)
{
	if (inst.opcode == Opcode::NONE)
	{
		fprintf(pFile, "dc.w $%x", inst.header);
		return;
	}
	fprintf(pFile, "%s", get_opcode_string(inst.opcode));
	fprintf(pFile, "%s", get_suffix_string(inst.suffix));

	if (inst.op0.type != OpType::INVALID)
	{
		fprintf(pFile, " ");
		print(inst.op0, symbols, inst_address, pFile);
	}
	if (inst.bf0.valid)
		print_bitfield(inst.bf0, pFile);

	if (inst.op1.type != OpType::INVALID)
	{
		fprintf(pFile, ",");
		print(inst.op1, symbols, inst_address, pFile);
	}
	if (inst.bf1.valid)
		print_bitfield(inst.bf1, pFile);

	if (inst.op2.type != OpType::INVALID)
	{
		fprintf(pFile, ",");
		print(inst.op2, symbols, inst_address, pFile);
	}
}
