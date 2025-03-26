#pragma once
#include "bind.hpp"

struct UnkArrayEntry {
	char unk[0x410];
};

class ParameterBlock {
public:
	struct Parameter {
		int label_string_table_index;
		char* value;
	};

	struct ParameterList {
		int token_count;
		char* tokens[80];
	};

	struct Header {
		int label_string_table_index;
		// ParameterBlocks have a feature where headers that are indexed (think "ShaderPass_1" and stuff)
		// get stored differently than ones that aren't. this flag parameter is equal to that number,
		// otherwise its equal to -1.
		short flag_or_index;
		Parameter* parameters;
		short parameter_count;
		short parameter_list_count;
		// additionally, any lines that it doesn't immediately recognize as the definition of a parameter get
		// thrown in this; it splits each these lines by splitting them by commas and stores them in this array.
		// honestly, i would be very surprised if this ever isn't unused because i don't see any code that reads
		// from this /shrug
		ParameterList* parameter_lists;
	};
	class PBSearch {
	public:
		ParameterBlock* parent;
		int current_header_index;
		char current_header_name[1024];
		char current_parameter_name[1024];
		int current_parameter_index;
		int reset_parameter_search;
	};
private:
	int loaded_from_memory;
	char** label_string_table;
	Header* headers;
	int label_string_table_len;
	int headers_capacity;
	// the total capacity of all headers' parameter arrays.
	int total_parameter_capacity;
	// the total capacity of all parameters' parameter list arrays. i'm pretty sure this feature goes unused anyway /shrug
	int total_parameter_list_capacity;
	// the DataAccess resource handle.
	int resource_handle;
	// no idea what this does, at any rate the game never tries to "load a ParameterBlock from memory" so w/e
	int successfully_loaded_from_memory;
	// i have no clue what these were supposed to do, but as far as i know this goes unused anyway so idc
	int unk_array_len;
	int unk_array_capacity;
	UnkArrayEntry* unk_array;

	int value_string_table_len;
	char** value_string_table;
	// always equal to -1, no matter what. goes unused anyway so /shrug
	int unk;
	class PBSearch search;
public:
	ParameterBlock();
	ParameterBlock(const ParameterBlock&) = delete;
	ParameterBlock& operator=(const ParameterBlock&) = delete;
	~ParameterBlock();

	// pretty much always just set load_from_memory to 0, resource_handle to -1, buffer to nullptr, and buffer_len to 0xFFFFFFFF.
	int OpenFile(const char* name, int load_from_memory, int resource_handle, std::size_t* buffer, std::size_t buffer_len);
	int GetParameter(const char* parameter, const char* default_value, char* dest, std::size_t dest_len);
	int GetParameter(const char* parameter, int default_value, int* dest);
	int GetParameter(const char* parameter, float default_value, float* dest);

	REPLACE_OPERATOR_NEW_DELETE;
};

static_assert(sizeof(ParameterBlock::Parameter) == 8);
static_assert(sizeof(ParameterBlock::ParameterList) == 0x144);
static_assert(sizeof(ParameterBlock::Header) == 0x14);
static_assert(sizeof(ParameterBlock::PBSearch) == 0x810);
static_assert(sizeof(ParameterBlock) == 2124);