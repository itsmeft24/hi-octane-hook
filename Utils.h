#pragma once
#include <iostream>
#include <algorithm>
#include <Windows.h>

#include "Globals.h"

const inline void make_lowercase(std::string& str) {
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
}
const inline void make_uppercase(std::string& str) {
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::toupper(c); });
}

HIOCTANE_API bool __stdcall SetExecuteReadWritePermission(void* ptr, size_t sz);

HIOCTANE_API bool __stdcall SetReadWritePermission(void* ptr, size_t sz);

HIOCTANE_API void __stdcall WriteJMP(void* src, void* dst);

HIOCTANE_API void __stdcall WriteCALL(void* src, void* dst);

HIOCTANE_API void __stdcall WritePUSH(void* src, void* dst);

HIOCTANE_API bool __stdcall WriteNOP(void* addr, size_t code_size);

#define AllocateCode(size) (char*)VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

#define FreeCode(ptr) (bool)VirtualFree(ptr, 0, MEM_RELEASE);