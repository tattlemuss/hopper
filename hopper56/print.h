#ifndef PRINT_H
#define PRINT_H

#include <stdio.h>
#include <stdint.h>

namespace hop56
{
	struct instruction;
}

// Write an instruction to the given file stream.
extern int print(const hop56::instruction& inst, uint32_t address, FILE* pOutput);

#endif // PRINT_H
