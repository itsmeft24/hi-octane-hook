#pragma once
#include <unordered_map>

enum class FunctionHookType : unsigned int
{
	EntireReplacement,
	InlineReplacement,
	IATReplacement,
	InlineDetour,
};

struct HookedFunctionInfo {
	void* source_address;
	void* destination_address;
	void* original_code;
	FunctionHookType type;
	size_t code_size;
};

std::unordered_map<void*, HookedFunctionInfo> HookedFunctions;

// TODO: PROPERLY IMPLEMENT
void RelocateCodeSegment(void* src, void* dst, const size_t size) {
	memcpy(dst, src, size);
}

bool HookFunction(void* src, void* dst, const size_t code_size, FunctionHookType type) {
	if (code_size < 5) return false;

	if (HookedFunctions.find(src) != HookedFunctions.end()) return false;

	// You cannot hook at code segments smaller than 5 bytes, and you also cannot hook the same address twice.

	if (type == FunctionHookType::EntireReplacement) {
		SetExecuteReadWritePermission(src, code_size);

		char* original_code = AllocateCode(code_size); // Allocate code for backed up instructions.

		RelocateCodeSegment(src, original_code, code_size); // Relocate code from the source pointer into the allocated backup buffer.

		HookedFunctionInfo info{ src, dst, original_code, type, code_size };

		HookedFunctions.insert({ src, info }); // Insert the new HookedFunctionInfo into the hashmap.

		memset(src, 0x90, code_size); // Pad out the original code with NOPs.

		WriteJMP(src, dst); // Write a function call from the source to the detour function.
	}
	else if (type == FunctionHookType::IATReplacement) {
		SetExecuteReadWritePermission(src, 4);

		HookedFunctionInfo info{ src, dst, *(void**)src, type, 0 };

		HookedFunctions.insert({ src, info }); // Insert the new HookedFunctionInfo into the hashmap.

		*(void**)src = dst;
	}
	else if (type == FunctionHookType::InlineReplacement) {
		SetExecuteReadWritePermission(src, code_size);

		char* original_code = AllocateCode(code_size); // Allocate code for backed up instructions.

		RelocateCodeSegment(src, original_code, code_size); // Relocate code from the source pointer into the allocated backup buffer.

		HookedFunctionInfo info{ src, dst, original_code, type, code_size };

		HookedFunctions.insert({ src, info }); // Insert the new HookedFunctionInfo into the hashmap.

		memset(src, 0x90, code_size); // Pad out the original code with NOPs.

		WriteCALL(src, dst); // Write a function call from the source to the detour function.
	}
	else if (type == FunctionHookType::InlineDetour) {
		SetExecuteReadWritePermission(src, code_size);

		char* trampoline = AllocateCode(8 + code_size); // Allocate code for trampoline.

		*trampoline = 0x60; // PUSHAD
		WriteCALL(trampoline + 1, dst);
		*(trampoline + 7) = 0x61; // POPAD

		// Inline hook function must look like:
		// __declspec(naked) void MyInlineHook(InlineContext ctx);

		RelocateCodeSegment(src, trampoline + 8, code_size);
		*(trampoline + code_size + 8) = 0xC3;

		HookedFunctionInfo info{ src, trampoline, trampoline + 15, type, code_size };

		HookedFunctions.insert({ src, info }); // Insert the new HookedFunctionInfo into the hashmap.

		memset(src, 0x90, code_size); // Pad out the original code with NOPs.

		WriteCALL(src, trampoline); // Write a function call from the source to the detour function.
	}
}

// TODO: PROPERLY IMPLEMENT
bool UninstallFunctionHook(void* src) {
	if (HookedFunctions.find(src) != HookedFunctions.end()) {
		auto& function_info = HookedFunctions.at(src);
	
	}
	return false;
}

void* GetRelocatedCode(void* ptr) {
	if (HookedFunctions.find(ptr) == HookedFunctions.end()) return nullptr;
	return HookedFunctions.at(ptr).original_code;
}

#define HookFunction(src, dst, code_size, type) HookFunction((void*)(src), (void*)(dst), code_size, type)

#define UninstallFunctionHook(src) UninstallFunctionHook((void*)(src))

#define GetRelocatedCode(ptr) GetRelocatedCode((void*)(ptr))
