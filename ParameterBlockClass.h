#pragma once
#include "Offsets.h"

#define DECL_FUNCTION(ret_type, call_conv, name, offset, ...) auto name = (ret_type(call_conv*)(##__VA_ARGS__))(offset)

DECL_FUNCTION(void, __thiscall, ParameterBlockConstructor, 0x00587E60, void*);
DECL_FUNCTION(void, __thiscall, ParameterBlockDestructor, 0x005C4440, void*);
DECL_FUNCTION(void, __thiscall, ParameterBlockOpenFile, 0x005D87C0, void*, char*, int, int, size_t*, size_t);
DECL_FUNCTION(int, __thiscall, ParameterBlockGetParameterDefaultInt, 0x00588250, void*, char*, int, int*);
DECL_FUNCTION(int, __thiscall, ParameterBlockGetParameterDefaultInt, 0x00588250, void*, char*, int, int*);

// This is an example of a wrapper class that can be used for directly calling the original games' code without reimplementing the original class.
class ParameterBlock {
public:
	char reserved[0x7C];
	ParameterBlock() {
		ParameterBlockConstructor(reserved);
	}
	~ParameterBlock() {
		ParameterBlockDestructor(reserved);
	}
	void OpenFile(char* ptr) {
		ParameterBlockOpenFile(reserved, ptr, 0, -1, nullptr, -1);
	}
	void OpenFile(const char* ptr) {
		ParameterBlockOpenFile(reserved, (char*)ptr, 0, -1, nullptr, -1);
	}
	BOOL GetParameter(char* ptr, int default_val, int* res) {
		return ParameterBlockGetParameterDefaultInt(reserved, ptr, default_val, res);
	}
	BOOL GetParameter(const char* ptr, int default_val, int* res) {
		return ParameterBlockGetParameterDefaultInt(reserved, (char*)ptr, default_val, res);
	}
};
