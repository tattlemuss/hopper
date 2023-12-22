#ifndef HOPPER56_DECODE_H
#define HOPPER56_DECODE_H

namespace hopper56
{
	struct instruction;
	struct buffer_reader;

	struct decode_settings
	{
	};

	int decode(instruction& inst, buffer_reader& buf, const decode_settings& settings);
}

#endif // HOPPER56_DECODE_H

