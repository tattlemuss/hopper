#ifndef SYMBOLS_H
#define SYMBOLS_H
#include <cstdint>
#include <string>
#include <map>
#include "lib/instruction.h"

// ----------------------------------------------------------------------------
//	SYMBOL STORAGE
// ----------------------------------------------------------------------------
struct symbol
{
	struct addr_t
	{
		hop56::Memory mem;
		uint32_t	  addr;

		bool operator<(const addr_t& other) const
		{
			if (mem != other.mem)
				return mem < other.mem;
			return addr < other.addr;
		}
	};

	std::string		label;
};

class symbols
{
public:
	std::map<symbol::addr_t, symbol>		table;
};

extern bool add_symbol(symbols& symbols, const hop56::Memory mem, uint32_t address, const symbol& new_symbol);
extern bool find_symbol(const symbols& symbols, const hop56::Memory mem, uint32_t address, symbol& result);

#endif
