
#include "core/globals.hpp"
#include "parameter_block.hpp"

DeclareFunction(void, __thiscall, ParameterBlock_Ctor, 0x00587E60, ParameterBlock*);
DeclareFunction(void, __thiscall, ParameterBlock_Dtor, 0x005c4440, ParameterBlock*);
DeclareFunction(int, __thiscall, ParameterBlock_OpenFile, 0x005d87c0, ParameterBlock*, const char*, int, int, size_t*, size_t);
DeclareFunction(int, __thiscall, ParameterBlock_GetParameterDefaultStr, 0x005881a0, ParameterBlock*, const char*, const char*, char*, int);
DeclareFunction(int, __thiscall, ParameterBlock_GetParameterDefaultInt, 0x00588250, ParameterBlock*, const char*, int, int*);
DeclareFunction(int, __thiscall, ParameterBlock_GetParameterDefaultFloat, 0x005882b0, ParameterBlock*, const char*, float, float*);

ParameterBlock::ParameterBlock() {
	ParameterBlock_Ctor(this);
}

ParameterBlock::~ParameterBlock() {
	ParameterBlock_Dtor(this);
}

int ParameterBlock::OpenFile(const char* name, int load_from_memory, int resource_handle, std::size_t* buffer, std::size_t buffer_len) {
	return ParameterBlock_OpenFile(this, name, load_from_memory, resource_handle, buffer, buffer_len);
}

int ParameterBlock::GetParameter(const char* parameter, const char* default_value, char* dest, std::size_t dest_len)
{
	return ParameterBlock_GetParameterDefaultStr(this, parameter, default_value, dest, dest_len);
}

int ParameterBlock::GetParameter(const char* parameter, int default_value, int* dest)
{
	return ParameterBlock_GetParameterDefaultInt(this, parameter, default_value, dest);
}

int ParameterBlock::GetParameter(const char* parameter, float default_value, float* dest)
{
	return ParameterBlock_GetParameterDefaultFloat(this, parameter, default_value, dest);
}

