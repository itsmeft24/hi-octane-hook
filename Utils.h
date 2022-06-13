#pragma once
#include <iostream>
#include <algorithm>
#include <dbghelp.h>

#define HIOCTANE_API extern "C" __declspec(dllexport)

const inline void make_lowercase(std::string& str) {
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
}
const inline void make_uppercase(std::string& str) {
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::toupper(c); });
}

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

#define WriteJMP(src, dst) WriteJMP((void*)(src), (void*)(dst))

#define WriteCALL(src, dst) WriteCALL((void*)(src), (void*)(dst))

#define WritePUSH(src, dst) WritePUSH((void*)(src), (void*)(dst))

#define WriteNOP(src, size) WriteNOP((void*)(src), size)

#define AllocateCode(size) (char*)VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

#define FreeCode(ptr) (bool)VirtualFree(ptr, 0, MEM_RELEASE);