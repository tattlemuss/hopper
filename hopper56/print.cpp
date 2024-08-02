#include "print.h"

#include <stdlib.h>
#include <memory.h>
#include <string>
#include <cstring>

#include "lib/instruction.h"

#define REGNAME		hop56::get_register_string

// Print an operand, for all operand types
static void print(const hop56::operand& operand, FILE* pOutput)
{
	fprintf(pOutput, "%s", hop56::get_memory_string(operand.memory));
	switch (operand.type)
	{
		case hop56::operand::IMM_SHORT:
			fprintf(pOutput, "#%d", operand.imm_short.val);
			break;
		case hop56::operand::REG:
			fprintf(pOutput, "%s",
					REGNAME(operand.reg.index));
			break;
		case hop56::operand::POSTDEC_OFFSET:
			fprintf(pOutput, "(%s)-%s",
					REGNAME(operand.postdec_offset.index_1),
					REGNAME(operand.postdec_offset.index_2));
			break;
		case hop56::operand::POSTINC_OFFSET:
			fprintf(pOutput, "(%s)+%s",
					REGNAME(operand.postinc_offset.index_1),
					REGNAME(operand.postinc_offset.index_2));
			break;
		case hop56::operand::POSTDEC:
			fprintf(pOutput, "(%s)-",
					REGNAME(operand.postdec.index));
			break;
		case hop56::operand::POSTINC:
			fprintf(pOutput, "(%s)+",
					REGNAME(operand.postinc.index));
			break;
		case hop56::operand::NO_UPDATE:
			fprintf(pOutput, "(%s)",
					REGNAME(operand.no_update.index));
			break;
		case hop56::operand::INDEX_OFFSET:
			fprintf(pOutput, "(%s+%s)",
					REGNAME(operand.index_offset.index_1),
					REGNAME(operand.index_offset.index_2));
			break;
		case hop56::operand::PREDEC:
			fprintf(pOutput, "-(%s)", REGNAME(operand.predec.index));
			break;
		case hop56::operand::ABS:
			fprintf(pOutput, "$%x", operand.abs.address);
			break;
		case hop56::operand::ABS_SHORT:
			fprintf(pOutput, ">$%x", operand.abs_short.address);
			break;
		case hop56::operand::IMM:
			fprintf(pOutput, "#$%x", operand.imm.val);
			break;
		case hop56::operand::IO_SHORT:
			fprintf(pOutput, "<<$%x", operand.io_short.address);
			break;
		default:
			fprintf(pOutput, "unknown %d?", operand.type);
			break;
	}
}

int print(const hop56::instruction& inst, uint32_t address, FILE* pOutput)
{
	if (inst.opcode == hop56::INVALID)
	{
		fprintf(pOutput, "DC\t$%06x", inst.header);
		return 0;
	}

	fprintf(pOutput, "%s", hop56::get_opcode_string(inst.opcode));
	for (int i = 0; i < 3; ++i)
	{
		const hop56::operand& op = inst.operands[i];
		if (op.type == hop56::operand::NONE)
			break;

		if (i == 0)
		{
			fprintf(pOutput, "\t");
			if (inst.neg_operands)
				fprintf(pOutput, "-");
		}
		else
			fprintf(pOutput, ",");

		print(op, pOutput);
	}

	for (int i = 0; i < 2; ++i)
	{
		const hop56::operand& op = inst.operands2[i];
		if (op.type == hop56::operand::NONE)
			break;

		if (i == 0)
			fprintf(pOutput, "\t");
		else
			fprintf(pOutput, ",");

		print(op, pOutput);
	}

	for (int i = 0; i < 2; ++i)
	{
		const hop56::pmove& pmove = inst.pmoves[i];
		if (pmove.operands[0].type == hop56::operand::NONE)
			continue;	// skip if there is no first operand

		fprintf(pOutput, "\t");
		print(pmove.operands[0], pOutput);

		if (pmove.operands[1].type == hop56::operand::NONE)
			continue;	// next pmove
		fprintf(pOutput, ",");
		print(pmove.operands[1], pOutput);
	}
	return 0;
}

