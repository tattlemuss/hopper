#ifndef DECODE_H
#define DECODE_H

class buffer_reader;
struct instruction;

enum
{
	CPU_TYPE_68000,
	CPU_TYPE_68010,
	CPU_TYPE_68020,
	CPU_TYPE_68030
};

struct decode_settings
{
	int cpu_type;
};

extern int decode(buffer_reader& buffer, const decode_settings& dsettings, instruction& inst);

#endif
