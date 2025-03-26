#include "core/globals.hpp"
#include "string_block_allocator.hpp"

DeclareFunction(char*, __thiscall, _StringBlockAllocator_AllocStringByLength_Impl, 0x005b6830, StringBlockAllocator*, int, int, const char*, int);
DeclareFunction(void, __thiscall, _StringBlockAllocator_FreeString_Impl, 0x005952c0, StringBlockAllocator*, char*);


StringBlockAllocator::StringBlockAllocator(std::size_t _initial_string_len, int _alloc_size)
{
	initial_string_len = _initial_string_len;
	if (_alloc_size == -1) {
		alloc_size = _initial_string_len;
	}
	else {
		alloc_size = _alloc_size;
	}
}

__declspec(naked) StringBlockAllocator::~StringBlockAllocator()
{
	UNIMPLEMENTED(0x00595300);
}

__declspec(naked) void StringBlockAllocator::Create(int unk)
{
	UNIMPLEMENTED(0x005b6780);
}

char* StringBlockAllocator::StringBlockAllocator_AllocStringByLength(int length, int unk)
{
	return _StringBlockAllocator_AllocStringByLength_Impl(this, length, unk, nullptr, 0);
}

char* StringBlockAllocator::StringBlockAllocator_AllocStringByString(char* str, int unk)
{
	return StringBlockAllocator_AllocStringByLength(strlen(str), unk);
}

void StringBlockAllocator::FreeString(char* str) {
	_StringBlockAllocator_FreeString_Impl(this, str);
}
