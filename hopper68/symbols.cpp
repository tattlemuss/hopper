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
	symbols::sym_map::const_iterator it = symbols.table.find(address);
	if (it != symbols.table.end())
	{
		result = it->second;
		return true;
	}
	return false;
}

bool find_reloc(const symbols& symbols, uint32_t address, uint32_t& target)
{
	symbols::reloc_map::const_iterator it = symbols.relocs.find(address);
	if (it != symbols.relocs.end())
	{
		target = it->second;
		return true;
	}
	return false;
}

