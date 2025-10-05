#include "symbols.h"

extern bool add_symbol(symbols& symbols, const symbol& new_symbol)
{
	if (symbols.table.find(new_symbol.address) != symbols.table.end())
		return false;
	std::pair<uint32_t, symbol> p;
	p.first = new_symbol.address;
	p.second = new_symbol;
	symbols.table.insert(p);
	return true;
}

bool find_symbol(const symbols& symbols, uint32_t address, symbol& result)
{
	std::map<uint32_t, symbol>::const_iterator it = symbols.table.find(address);
	if (it != symbols.table.end())
	{
		result = it->second;
		return true;
	}
	return false;
}

