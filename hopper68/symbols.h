#ifndef SYMBOLS_H
#define SYMBOLS_H
#include <cstdint>
#include <string>
#include <map>

// ----------------------------------------------------------------------------
//	SYMBOL STORAGE
// ----------------------------------------------------------------------------
struct symbol
{
	enum section_type
	{
		TEXT,
		DATA,
		BSS,
		UNKNOWN			// placeholder for unexpected symbols
	};

	std::string		label;
	section_type	section;
	// TODO section, flags.
	uint32_t		address;		// Address with section start factored in, so global across the executable
};

class symbols
{
public:
	typedef		std::map<uint32_t, symbol> sym_map;
	typedef		std::map<uint32_t, uint32_t> reloc_map;		// reloc_addr -> offset address
	sym_map			table;
	reloc_map		relocs;								// locations where relocations happened
};

extern bool add_symbol(symbols& symbols, const symbol& new_symbol);
extern bool find_symbol(const symbols& symbols, uint32_t address, symbol& result);
extern bool find_reloc(const symbols& symbols, uint32_t address, uint32_t& target);

#endif
