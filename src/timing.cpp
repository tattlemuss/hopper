#include "timing.h"
#include "instruction.h"

struct time_entry
{
	Opcode		op;
	Suffix		suffix;
	OpType		type0;	
	OpType		type1;
	uint16_t	time_min;
	uint16_t	time_max;
};

#define ENTRY(op, suf, type0, type1, time)	{	Opcode::op, Suffix::suf, OpType::type0, OpType::type1, time, time }

static const time_entry g_timingEntry[] = 
{
	ENTRY	(TRAP,			NONE,	IMMEDIATE,	INVALID,		34 ),

	{ Opcode::COUNT}
};

int calc_timing(const instruction& inst, timing& result)
{
	for (const time_entry* curr_entry = g_timingEntry;
		curr_entry->op != Opcode::COUNT;
		++curr_entry)
	{
		if (curr_entry->op != inst.opcode) 
			continue;
		if (curr_entry->suffix != inst.suffix) 
			continue;
		if (curr_entry->type0 != OpType::INVALID && curr_entry->type0 != inst.op0.type)
			continue;
		if (curr_entry->type1 != OpType::INVALID && curr_entry->type1 != inst.op1.type)
			continue;

		result.min = curr_entry->time_min;
		result.max = curr_entry->time_max;
		return 0;
	}
    return 1;
}