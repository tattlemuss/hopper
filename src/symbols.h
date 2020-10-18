#ifndef SYMBOLS_H
#define SYMBOLS_H
#include <cstdint>
#include <string>
#include <vector>

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
	std::vector<symbol>		table;
};

extern bool find_symbol(const symbols& symbols, uint32_t address, symbol& result);

#endif
