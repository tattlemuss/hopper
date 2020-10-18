#include "symbols.h"

bool find_symbol(const symbols& symbols, uint32_t address, symbol& result)
{
	for (size_t i = 0; i < symbols.table.size(); ++i)
	{
		const symbol& sym = symbols.table[i];
		if (sym.address == address)
		{
			result = sym;
			return true;
		}
	}
	return false;
}

