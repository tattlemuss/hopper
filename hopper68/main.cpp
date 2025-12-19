#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <vector>
#include <string>
#include <cstring>

#include "lib/buffer68.h"
#include "lib/decode68.h"
#include "lib/instruction68.h"
#include "lib/timing68.h"
#include "symbols.h"
#include "print.h"

// ----------------------------------------------------------------------------
// User options for output.
struct output_settings
{
	bool show_address;			// print address for each line before opcode
	bool show_timings;			// print (guessed) timings for each line (valid for 68000 only)
	bool autolabel;				// autolabelling on/off
	std::string label_prefix;	// prefix for all auto-labels, normally "L"
	uint32_t label_start_id;	// starting number of label prefix, normally 0
};

// ----------------------------------------------------------------------------
//	HIGHER-LEVEL DISASSEMBLY CREATION
// ----------------------------------------------------------------------------

// Storage for line numbers
class line_numbers
{
public:
	struct line
	{
		size_t file_index;
		uint32_t line;
	};

	std::vector<std::string>	filenames;

	typedef std::pair<uint32_t, line> pair;
	std::map<uint32_t, line>	lines;

	void add(size_t file_index, uint32_t line_num, uint32_t pc)
	{
		pair p;
		p.first = pc;
		p.second.file_index = file_index;
		p.second.line = line_num;
		lines.insert(p);
	}

	bool find(uint32_t address, line& result) const
	{
		std::map<uint32_t, line>::const_iterator it = lines.find(address);
		if (it != lines.end())
		{
			result = it->second;
			return true;
		}
		return false;
	}

	size_t add_filename(const char* filename)
	{
		size_t index = filenames.size();
		filenames.push_back(filename);
		return index;
	}
};

// ----------------------------------------------------------------------------
// Storage for an attempt at tokenising the memory
class disassembly
{
public:
	struct line
	{
		uint32_t address;
		hop68::instruction inst;
	};

	std::vector<line>    lines;
};

// ----------------------------------------------------------------------------
// Read the buffer in a simple single pass.
int decode_buf(hop68::buffer_reader& buf, const hop68::decode_settings& dsettings, disassembly& disasm)
{
	while (buf.get_remain() >= 2)
	{
		disassembly::line line;
		line.address = buf.get_pos();

		// decode uses a copy of the buffer state
		hop68::buffer_reader buf_copy(buf);

		// We can ignore the return code, since it just says "this instruction is valid"
		// rather than "something catastrophic happened"
		hop68::decode(line.inst, buf_copy, dsettings);

		// Handle failure
		disasm.lines.push_back(line);

		buf.advance(line.inst.byte_count);
	}
	return 0;
}

// ----------------------------------------------------------------------------
// Print a set of diassembled lines.
int print(const symbols& symbols, const line_numbers& lines,
	const disassembly& disasm, const output_settings& osettings, FILE* pOutput)
{
	// previous flag for timing pairs
	uint8_t prev_flag = 0;

	size_t last_file_index = (size_t)-1;
	symbols::sym_map::const_iterator sym_it = symbols.table.begin();
	for (size_t i = 0; i < disasm.lines.size(); ++i)
	{
		const disassembly::line& line = disasm.lines[i];
		const hop68::instruction& inst = line.inst;

		// TODO very naive label check
		while (sym_it != symbols.table.end())
		{
			if (sym_it->first >= line.inst.address + line.inst.byte_count)
				break;

			const symbol& sym = sym_it->second;
			uint32_t sym_off = sym.address - line.address;
			if (sym_off)
				fprintf(pOutput, "%s: = *+%u\n", sym.label.c_str(), sym_off);
			else
				fprintf(pOutput, "%s:\n", sym.label.c_str());
			++sym_it;
		}

		// Debug line-number checks
		line_numbers::line ln;
		if (lines.find(line.address, ln))
		{
			if (ln.file_index != last_file_index)
			{
				// Change of active file
				std::string filename = lines.filenames[ln.file_index];
				fprintf(pOutput, "; File: %s\n", filename.c_str());
				last_file_index = ln.file_index;
			}
			fprintf(pOutput, "; line %04u:\n", ln.line);
		}

		fprintf(pOutput, "\t");
		int count = print(inst, symbols, line.address, pOutput);

		// Insert tabs up to 32 characters
		// NOTE: assumes tab size of 8
		if (osettings.show_address)
		{
			while (count < 32)
			{
				fprintf(pOutput, "\t");
				count = ((count + 8) / 8) * 8;
			}
			fprintf(pOutput, "; %x", inst.address);
		}

		if (osettings.show_timings && inst.opcode != hop68::Opcode::NONE)
		{
			hop68::timing timing;
			if (calc_timing(inst, timing) != 0)
			{
				fprintf(pOutput, "\t; ?");
			}
			else
			{
				// Adjust timing for pairing.
				// By default, round up to a multiple of four.
				uint16_t time = (timing.min + 3) & 0xfffc;
				const char* comment = "";

				// Exception: previous inst has pair_back and we have pair_front,
				// in which case we subtract 4
				if ((prev_flag & PAIR_BACK) && (timing.flags & PAIR_FRONT))
				{
					time -= 4;
					comment = " (pair)";
				}
				fprintf(pOutput, "\t; %d%s", time, comment);
			}
			prev_flag = timing.flags;
		}
		else
		{
			prev_flag = 0;
		}

		fprintf(pOutput, "\n");
	}
	return 0;
}

// ----------------------------------------------------------------------------
// Find addresses referenced by disasm instructions and add them to the
// symbol table
void add_reference_symbols(const disassembly& disasm, const output_settings& settings, symbols& symbols)
{
	uint32_t label_id = settings.label_start_id;
	uint32_t last_address = disasm.lines.back().address;

	for (size_t i = 0; i < disasm.lines.size(); ++i)
	{
		const disassembly::line& line = disasm.lines[i];
		uint32_t target_address;

		if (calc_relative_address(line.inst.op0, line.address, target_address))
		{
			symbol sym;
			if (!find_symbol(symbols, target_address, sym))
			{
				sym.address = target_address;
				sym.section = symbol::section_type::TEXT;
				add_symbol(symbols, sym);
				++label_id;
			}
		}

		if (line.inst.op0.type == hop68::ABSOLUTE_LONG)
		{
			target_address = line.inst.op0.absolute_long.longaddr;
			symbol sym;
			if (target_address <= last_address && !find_symbol(symbols, target_address, sym))
			{
				sym.address = target_address;
				sym.section = symbol::section_type::TEXT;
				add_symbol(symbols, sym);
				++label_id;
			}
		}

		if (calc_relative_address(line.inst.op1, line.address, target_address))
		{
			symbol sym;
			if (!find_symbol(symbols, target_address, sym))
			{
				sym.address = target_address;
				sym.section = symbol::section_type::TEXT;
				add_symbol(symbols, sym);
				++label_id;
			}
		}
	}
}

// ----------------------------------------------------------------------------
//	TOS EXECUTABLE READING
// ----------------------------------------------------------------------------
struct tos_header
{
   //  See http://toshyp.atari.org/en/005005.html for TOS header details
	uint16_t  ph_branch;	  /* Branch to start of the program  */
							  /* (must be 0x601a!)			   */

	uint32_t  ph_tlen;		  /* Length of the TEXT segment	  */
	uint32_t  ph_dlen;		  /* Length of the DATA segment	  */
	uint32_t  ph_blen;		  /* Length of the BSS segment	   */
	uint32_t  ph_slen;		  /* Length of the symbol table	  */
	uint32_t  ph_res1;		  /* Reserved, should be 0;		  */
							  /* Required by PureC			   */
	uint32_t  ph_prgflags;	  /* Program flags				   */
	uint16_t  ph_absflag;	  /* 0 = Relocation info present	 */
};

// ----------------------------------------------------------------------------
//	DRI SYMBOL READING
// ----------------------------------------------------------------------------
static const int DRI_TABLE_SIZE = 14;
static const uint16_t DRI_EXT_SYMBOL_FLAG = 0x0048;
static const uint16_t DRI_SECT_TEXT = 0x0200;
static const uint16_t DRI_SECT_DATA = 0x0400;
static const uint16_t DRI_SECT_BSS  = 0x0100;

int read_symbols(hop68::buffer_reader& buf, const tos_header& header, symbols& symbols)
{
	// Calculate text, data and bss addresses
	uint32_t text_address = 0;
	uint32_t data_address = text_address + header.ph_tlen;
	uint32_t bss_address  = data_address + header.ph_dlen;

	while (buf.get_remain() >= DRI_TABLE_SIZE)
	{
		uint8_t name[8 + 14 + 1];
		uint16_t symbol_id;
		uint32_t symbol_address;

		// Name (8 bytes) -> ID (2 bytes) -> address (4 bytes)
		memset(name, 0, sizeof(name));
		if (buf.read(name, 8))
			return 1;
		if (buf.read_word(symbol_id))
			return 1;
		if (buf.read_long(symbol_address))
			return 1;

		// Looks like either bit denotes an extended symbol??
		if ((symbol_id & DRI_EXT_SYMBOL_FLAG) != 0)
		{
			// 14 bytes: symbol name extended
			if (buf.read(name + 8, 14))
				return 1;
		}

		printf("; Symbol %s addr: %d id:%x\n", (const char*)name, symbol_address, symbol_id);

		symbol sym;
		sym.label = std::string((const char*)name);
		sym.address = symbol_address;
		sym.section = symbol::section_type::UNKNOWN;

		// Parse the flags to work out which section, then resolve an address
		switch (symbol_id & 0xf00)
		{
			case DRI_SECT_TEXT: sym.section = symbol::section_type::TEXT; sym.address += text_address; break;
			case DRI_SECT_DATA: sym.section = symbol::section_type::DATA; sym.address += data_address; break;
			case DRI_SECT_BSS:  sym.section = symbol::section_type::BSS;  sym.address += bss_address;  break;
			default:
				break;
		}
		if (sym.section != symbol::section_type::UNKNOWN)
			add_symbol(symbols, sym);
	}

	return 0;
}

// ----------------------------------------------------------------------------
//	DEBUG LINE READING
// ----------------------------------------------------------------------------
// Read N bytes of string data in to a std::string, omitting pad bytes
static int read_string(hop68::buffer_reader& buf, uint32_t length, std::string& str)
{
	uint8_t ch;
	for (uint32_t i = 0; i < length; ++i)
	{
		if (buf.read_byte(ch))
			return 1;
		if (ch)	// Don't add any padded zero bytes
			str += (char)ch;
	}
	return 0;
}

// ----------------------------------------------------------------------------
// Read hunk of "LINE" format line information.
// This is a simple set of "line", "pc" 8-byte structures
static int read_debug_line_info(hop68::buffer_reader& buf, line_numbers& lines, uint32_t offset)
{
	// Filename length is stored as divided by 4
	uint32_t flen;
	if (buf.read_long(flen))
		return 1;
	flen <<= 2;
	std::string fname;
	if (read_string(buf, flen, fname))
		return 1;
	size_t file_index = lines.add_filename(fname.c_str());

	// Calculate remaining number of structures to read
	uint32_t numlines = buf.get_remain() / 8;
	while (numlines)
	{
		uint32_t line, pc;
		if (buf.read_long(line))
			return 1;
		if (buf.read_long(pc))
			return 1;

		lines.add(file_index, line, pc + offset);
		--numlines;
	}
	return 0;
}

// ----------------------------------------------------------------------------
// Read a single compressed HCLN value (line or PC)
// The format is a simple prefix compression:
// 		 1 byte: return value if != 0
// 	else 2 byte word: return value if != 0
//  else 4 byte long.
static int read_hcln_long(hop68::buffer_reader& buf, uint32_t& val)
{
	uint8_t tmpB;
	if (buf.read_byte(tmpB))
		return 1;
	if (tmpB)
	{
		val = tmpB; return 0;
	}

	uint16_t tmpW;
	if (buf.read_word(tmpW))
		return 1;
	if (tmpW)
	{
		val = tmpW; return 0;
	}

	return buf.read_long(val);
}

// ----------------------------------------------------------------------------
// Read hunk of "HCLN" (HiSoft Compressed Line Number) format line information.
static int read_debug_hcln_info(hop68::buffer_reader& buf, line_numbers& lines, uint32_t offset)
{
	uint32_t flen, numlines;
	// Filename length is stored as divided by 4
	if (buf.read_long(flen))
		return 1;
	std::string fname;
	flen <<= 2;
	if (read_string(buf, flen, fname))
		return 1;
	size_t file_index = lines.add_filename(fname.c_str());

	if (buf.read_long(numlines))
		return 1;
	uint32_t curr_line = 0;
	uint32_t curr_pc = offset;
	while (numlines)
	{
		uint32_t line, pc;
		if (read_hcln_long(buf, line))
			return 1;
		if (read_hcln_long(buf, pc))
			return 1;
		// Accumulate current position
		curr_line += line;
		curr_pc += pc;
		lines.add(file_index, curr_line, curr_pc);
		--numlines;
	}
	return 0;
}

// ----------------------------------------------------------------------------
static int add_reloc_label(uint32_t addr, hop68::buffer_reader& text_buf,
	symbols& symbols)
{
	text_buf.set_pos(0);
	if (addr + 4 > text_buf.get_remain())
		return 0;

	// This longword is relocated, so can be treated as a symbol.
	// Read the longword value, then create a text symbol
	uint32_t data;
	text_buf.set_pos(addr);
	if (text_buf.read_long(data))
		return 1;
	text_buf.set_pos(0);

	symbol sym;
	sym.address = data;
	sym.section = symbol::section_type::TEXT;
	sym.label = "";
	add_symbol(symbols, sym);

	// Flag that a relocation address existed
	symbols.relocs[addr] = data;
	return 0;
}

// ----------------------------------------------------------------------------
// Read relocation information and debug line number information.
static int read_reloc(hop68::buffer_reader& buf, hop68::buffer_reader& text_buf,
	symbols& symbols, line_numbers& lines, bool autolabel)
{
	uint32_t addr;
	if (buf.read_long(addr))
		return 1;

	// 0 at start meeans "no reloc info"
	if (addr)
	{
		if (autolabel)
			add_reloc_label(addr, text_buf, symbols);
		while (1)
		{
			uint8_t offset;
			if (buf.read_byte(offset))
				return 1;
			if (offset == 0)
				break;

			if (offset == 1)
			{
				addr += 254;
				continue;
			}
			else
			{
				addr += offset;
				if (autolabel)
					add_reloc_label(addr, text_buf, symbols);
			}
		}
	}

	// Read debugging information
	// Align to word
	if (buf.get_pos() & 1)
		buf.advance(1);

	// Debug information is a set of blocks ("hunks"?) with a
	// type and size, similar to IFF.
	bool got_header = false;
	while (buf.get_remain() >= 4)
	{
		uint32_t hunk_header;
		uint32_t hlen, hstart, offset, htype;

		if (buf.read_long(hunk_header))
			return 1;
		if ( (hunk_header==0) && got_header)
			break;			// zero might mean padded, so stop without error

		// Expect the magic "this is a hunk" value
		if (hunk_header != 0x3F1)
			return 1;			// (bad file)

		// Read hunk length, which is number of 32-byte longs
		if (buf.read_long(hlen))
			return 1;
		// Convert header length to bytes
		hlen <<= 2;
		hstart = buf.get_pos();	// record position for jumping

		// Create a sub-reader so we don't read off end of the hunk
		hop68::buffer_reader hunk_buffer(buf.get_data(), hlen, buf.get_address());
		// This appears to be an offset to apply to the PC values
		if (hunk_buffer.read_long(offset))
			return 1;
		if (hunk_buffer.read_long(htype))
			return 1;

		switch (htype)
		{
			// NOTE: the X-Debug example does stricter checks on format here.
			// We just record that a header is found before the per-file hunks
			// are encountered.
			case 0x48454144: // "HEAD"
				got_header = true;
				break;
			case 0x4c494e45: // "LINE"
				read_debug_line_info(hunk_buffer, lines, offset);
				break;
			case 0x48434c4e: // "HCLN"
				read_debug_hcln_info(hunk_buffer, lines, offset);
				break;
			default:
				return 1;
		}

		// Jump to next hunk
		buf.set_pos(hstart + hlen);
	}

	return 0;
}

// ----------------------------------------------------------------------------
int process_tos_file(const uint8_t* data_ptr, long size, const hop68::decode_settings& dsettings,
		const output_settings& osettings, FILE* pOutput)
{
	hop68::buffer_reader buf(data_ptr, size, 0);
	tos_header header = {};

	if (buf.read_word(header.ph_branch))
		return 1;
	if (buf.read_long(header.ph_tlen))
		return 1;
	if (buf.read_long(header.ph_dlen))
		return 1;
	if (buf.read_long(header.ph_blen))
		return 1;
	if (buf.read_long(header.ph_slen))
		return 1;
	if (buf.read_long(header.ph_res1))
		return 1;
	if (buf.read_long(header.ph_prgflags))
		return 1;
	if (buf.read_word(header.ph_absflag))
		return 1;

	if (header.ph_branch != 0x601a)
		return 1;

	fprintf(pOutput, "; Text size %d...\n", header.ph_tlen);
	fprintf(pOutput, "; Data size %d...\n", header.ph_dlen);
	fprintf(pOutput, "; BSS size  %d...\n", header.ph_blen);
	fprintf(pOutput, "; Symbol size  %d...\n", header.ph_slen);

	// Next section is text
	fprintf(pOutput, "; Reading text section\n");
	hop68::buffer_reader text_buf(buf.get_data(), header.ph_tlen, 0);

	// Skip the text
	buf.advance(header.ph_tlen);
	hop68::buffer_reader data_buf(buf.get_data(), header.ph_dlen, 0);

	// Skip the data
	buf.advance(header.ph_dlen);

	// (No BSS in the file, so symbols should be next)
	hop68::buffer_reader symbol_buf(buf.get_data(), header.ph_slen, 0);

	symbols exe_symbols;
	line_numbers lines;

	fprintf(pOutput, "; Reading symbols...\n");
	int ret = read_symbols(symbol_buf, header, exe_symbols);
	if (ret)
	{
		fprintf(stderr, "Error reading symbol table\n");
		return ret;
	}

	buf.advance(header.ph_slen);
	hop68::buffer_reader reloc_buf(buf.get_data(), buf.get_remain(), 0);

	// Read relocations (and autolabel them if necessary)
	// Read line-information data from Hisoft tools
	read_reloc(reloc_buf, text_buf, exe_symbols, lines, osettings.autolabel);

	disassembly disasm;
	if (decode_buf(text_buf, dsettings, disasm))
		return 1;

	// Scan decoded instructions and add labels from operands
	if (osettings.autolabel)
		add_reference_symbols(disasm, osettings, exe_symbols);

	// Rename auto-labelled symbols to be in address-order
	uint32_t id = osettings.label_start_id;
	for (symbols::sym_map::iterator it = exe_symbols.table.begin();
			it != exe_symbols.table.end();
			++it)
	{
		if (it->second.label.size() == 0)
			it->second.label = osettings.label_prefix + std::to_string(id++);
	}

	print(exe_symbols, lines, disasm, osettings, pOutput);
	return 0;
}

// ----------------------------------------------------------------------------
int process_bin_file(const uint8_t* data_ptr, long size, const hop68::decode_settings& dsettings,
		const output_settings& osettings, FILE* pOutput)
{
	hop68::buffer_reader buf(data_ptr, size, 0);
	symbols bin_symbols;
	line_numbers dummy_lines;

	disassembly disasm;
	if (decode_buf(buf, dsettings, disasm))
		return 1;

	add_reference_symbols(disasm, osettings, bin_symbols);

	print(bin_symbols, dummy_lines, disasm, osettings, pOutput);
	return 0;
}

// ----------------------------------------------------------------------------
static bool get_hex_value(char c, uint8_t& val)
{
	if (c >= '0' && c <= '9')
	{
		val = c - '0';
		return true;
	}
	if (c >= 'A' && c <= 'F')
	{
		val = 10 + c - 'A';
		return true;
	}
	if (c >= 'a' && c <= 'f')
	{
		val = 10 + c - 'a';
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
static bool is_whitespace(char c)
{
	return c == ' ' || c == '\t';
}

// ----------------------------------------------------------------------------
int process_hex_string(const char* hex_string, const hop68::decode_settings& dsettings, const output_settings& osettings, FILE* pOutput)
{
	size_t strsize = strlen(hex_string);
	// Allocate maximum bound for data
	size_t max_byte_count = strsize / 2;
	uint8_t* data_ptr = (uint8_t*)malloc(max_byte_count);

	size_t num_written = 0;
	size_t read_index = 0;
	// We need at least 2 chars left to parse a hex byte.
	while (read_index <= strsize - 2)
	{
		uint8_t val1, val2;
		// Get the next two bytes
		bool success = get_hex_value(hex_string[read_index], val1);
		success &= get_hex_value(hex_string[read_index + 1], val2);
		read_index += 2;

		if (!success)
		{
			fprintf(stderr, "Not a valid hex string '%s'\n", hex_string);
			free(data_ptr);
			return 1;
		}
		data_ptr[num_written++] = (val1 << 4) | val2;
		// Skip any trailing whitespace
		while (read_index < strsize && is_whitespace(hex_string[read_index]))
			++read_index;
	}
	if (read_index != strsize)
	{
		// characters left at end of buffer
		fprintf(stderr, "Not a valid hex string '%s'\n", hex_string);
		free(data_ptr);
		return 1;
	}

	// Wrap it up and decode
	hop68::buffer_reader buf(data_ptr, num_written, 0);
	disassembly disasm;
	int ret = decode_buf(buf, dsettings, disasm);
	free(data_ptr);

	if (ret)
		return ret;

	// Print it out
	symbols dummy_symbols;
	line_numbers dummy_lines;

	print(dummy_symbols, dummy_lines, disasm, osettings, pOutput);
	return 0;
}

// ----------------------------------------------------------------------------
// Operating mode: .prg file, binary file, or cmdline input
enum DECODE_MODE
{
	MODE_TOS = 0,
	MODE_BIN = 1,
	MODE_HEX = 2
};

// ----------------------------------------------------------------------------
void usage()
{
	fprintf(stdout, "hopper68\n\n"
		"Usage: hopper68 [options] input_filename|hexstring\n\n"
		"options:\n"
		"\t--hex       Input argument is hex string rather than filename\n"
		"\t--bin       Read binary file rather than .prg\n"
		"\t--address   Print instruction addresses\n"
		"\t--timings   Print estimated timings (Atari ST 68000 only)\n"
		"\t--no-labels Do not add automatically-detected labels\n"
		"\t--m68010\n"
		"\t--m68020\n"
		"\t--m68030    Select CPU type (default m68000)\n"
		"\t--label-prefix <string>   Set prefix for auto-labels\n"
		"\t--label-start <int>       Set starting suffix number for auto-labels\n"
	);
}

// ----------------------------------------------------------------------------
int main(int argc, char** argv)
{
	if (argc < 2)
	{
		fprintf(stderr, "Error: No filename or hexstring\n");
		usage();
		return 1;
	}


	DECODE_MODE mode = MODE_TOS;
	output_settings osettings = {};
	osettings.show_address = false;
	osettings.show_timings = false;
	osettings.autolabel = true;
	osettings.label_prefix = "L";
	osettings.label_start_id = 0;

	hop68::decode_settings dsettings = {};
	dsettings.cpu_type = hop68::CPU_TYPE_68000;
	const int last_arg = argc - 1;							// last arg is reserved for filename or hex data.

	for (int opt = 1; opt < last_arg; ++opt)
	{
		if (strcmp(argv[opt], "--bin") == 0)
			mode = MODE_BIN;
		else if (strcmp(argv[opt], "--hex") == 0)
			mode = MODE_HEX;							// Hex is now just a mode switch like --bin
		else if (strcmp(argv[opt], "--address") == 0)
			osettings.show_address = true;
		else if (strcmp(argv[opt], "--timings") == 0)
			osettings.show_timings = true;
		else if (strcmp(argv[opt], "--no-labels") == 0)
			osettings.autolabel = false;
		else if (strcmp(argv[opt], "--m68010") == 0)
			dsettings.cpu_type = hop68::CPU_TYPE_68010;
		else if (strcmp(argv[opt], "--m68020") == 0)
			dsettings.cpu_type = hop68::CPU_TYPE_68020;
		else if (strcmp(argv[opt], "--m68030") == 0)
			dsettings.cpu_type = hop68::CPU_TYPE_68030;
		else if (strcmp(argv[opt], "--label-prefix") == 0)
		{
			opt++;
			if (opt < last_arg)
			{
				osettings.label_prefix = argv[opt];
			}
			else
			{
				fprintf(stderr, "Error: --label-prefix misses parameter\n");
				return 1;
			}
		}
		else if (strcmp(argv[opt], "--label-start") == 0)
		{
			opt++;
			if (opt < last_arg)
			{
				osettings.label_start_id = atoi(argv[opt]);
			}
			else
			{
				fprintf(stderr, "Error: --label-start misses parameter\n");
				return 1;
			}
		}
		else
		{
			fprintf(stderr, "Error: Unknown option: '%s'\n", argv[opt]);
			usage();
			return 1;
		}
	}

	if (mode != MODE_HEX)
	{
		const char* fname = argv[argc - 1];
		FILE* pInfile = fopen(fname, "rb");
		if (!pInfile)
		{
			fprintf(stderr, "Error: Can't open file: %s\n", fname);
			return 1;
		}

		(void) fseek(pInfile, 0, SEEK_END);
		long size = ftell(pInfile);

		rewind(pInfile);
		uint8_t* data_ptr = (uint8_t*) malloc(size);
		long readBytes = fread(data_ptr, 1, size, pInfile);
		fclose(pInfile);
		if (readBytes != size)
		{
			fprintf(stderr, "Error: Failed to read file contents\n");
			return 1;
		}
		int ret = 0;
		if (mode == MODE_TOS)
			ret = process_tos_file(data_ptr, size, dsettings, osettings, stdout);
		else if (mode == MODE_BIN)
			ret = process_bin_file(data_ptr, size, dsettings, osettings, stdout);

		free(data_ptr);
		return ret;
	}
	else if (mode == MODE_HEX)
	{
		const char* hex_data = argv[argc - 1];
		return process_hex_string(hex_data, dsettings, osettings, stdout);
	}
}
