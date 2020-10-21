#ifndef TIMING_H
#define TIMING_H
#include <cstdint>

class instruction;

struct timing
{
	uint16_t min;
	uint16_t max;
};

extern int calc_timing(const instruction& inst, timing& result);

#endif // TIMING_H

