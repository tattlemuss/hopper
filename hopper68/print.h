// Sample functions to write an instruction to a file pointer.
// It should be easy to update this to write to other text streams, or
// representations that suit the use-case.
#ifndef PRINT_H
#define PRINT_H

#include <stdint.h>
#include <stdio.h>

namespace hop68
{
// Forward declarations
struct instruction;
struct operand;
}
class symbols;

// Check if an opcode jumps to another known address, and return that address
extern bool calc_relative_address(const hop68::operand& op, uint32_t inst_address, uint32_t& target_address);

// Write out an instruction's opcode and operands to the file stream.
extern void print(const hop68::instruction& inst, const symbols& symbols, uint32_t inst_address, FILE* pFile);

#endif
