
#include "Utils.h"

HIOCTANE_API bool __stdcall SetExecuteReadWritePermission(void* ptr, size_t sz) {
	DWORD oldProtect;
	return (bool)VirtualProtect(ptr, sz, PAGE_EXECUTE_READWRITE, &oldProtect);
}

HIOCTANE_API bool __stdcall SetReadWritePermission(void* ptr, size_t sz) {
	DWORD oldProtect;
	return (bool)VirtualProtect(ptr, sz, PAGE_READWRITE, &oldProtect);
}

HIOCTANE_API void __stdcall WriteJMP(void* src, void* dst)
{
	DWORD relativeAddress = (DWORD)((unsigned char*)dst - (DWORD)src) - 5;

	SetExecuteReadWritePermission(src, 5);
	*(unsigned char*)src = 0xE9;
	*(DWORD*)((DWORD)src + 1) = relativeAddress;
}

HIOCTANE_API void __stdcall WriteCALL(void* src, void* dst)
{
	DWORD relativeAddress = (DWORD)((unsigned char*)dst - (DWORD)src) - 5;

	SetExecuteReadWritePermission(src, 5);
	*(unsigned char*)src = 0xE8;
	*(DWORD*)((DWORD)src + 1) = relativeAddress;
}

HIOCTANE_API void __stdcall WritePUSH(void* src, void* dst)
{
	SetExecuteReadWritePermission(src, 5);
	*(unsigned char*)src = 0x68;
	*(DWORD*)((DWORD)src + 1) = (DWORD)dst;
}

HIOCTANE_API bool __stdcall WriteNOP(void* addr, size_t code_size) {
	if (SetExecuteReadWritePermission(addr, code_size)) {
		memset(addr, 0x90, code_size);
		return true;
	}
	return false;
}
