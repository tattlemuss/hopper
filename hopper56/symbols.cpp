#include "symbols.h"

extern bool add_symbol(symbols& symbols, const hop56::Memory mem, uint32_t address, const symbol& new_symbol)
{
	std::pair<symbol::addr_t, symbol> p;
	p.first.mem = mem;
	p.first.addr = address;
	p.second = new_symbol;
	symbols.table.insert(p);
	return true;
}

bool find_symbol(const symbols& symbols, const hop56::Memory mem, uint32_t address, symbol& result)
{
	symbol::addr_t a;
	a.mem = mem;
	a.addr = address;
	std::map<symbol::addr_t, symbol>::const_iterator it = symbols.table.find(a);
	if (it != symbols.table.end())
	{
		result = it->second;
		return true;
	}
	return false;
}

