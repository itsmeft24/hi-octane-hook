#pragma once
#include <unordered_map>
#include <detours/detours.h>
#include <optional>

enum class FunctionHookType : unsigned int
{
	Invalid = -1,
	EntireReplacement, // Replace an entire function with your own implementation, with intent to be able to call the original at any time.
	InlineReplacement, // Replace a segment of code within a function with your own code.
	IATReplacement, // Replace an entry in the IAT of the running executable, provided its address pointer.
	InlineReplacementJMP, // An InlineReplacement that does not use a function call, instead relying on the user to properly JMP back to the target function.
};

struct HookedFunctionInfo {
	void* source_address;
	void* destination_address;
	void* original_code;
	FunctionHookType type;
	size_t code_size;

	static constexpr HookedFunctionInfo Default() {
		return { (void*)(0xCCCCCCCC), (void*)(0xCCCCCCCC), (void*)(0xCCCCCCCC), FunctionHookType::Invalid, 0xCCCCCCCC };
	}
};

// TODO: PROPERLY IMPLEMENT (Not entirely necessary, but would be helpful in the future.)
void RelocateCodeSegment(void* src, void* dst, const size_t size) {
	memcpy(dst, src, size);
}

HookedFunctionInfo HookFunction(void*& src, void* dst, const size_t code_size, FunctionHookType type) {

	HookedFunctionInfo info = HookedFunctionInfo::Default();

	if (code_size < 5) return info; // You cannot hook at code segments smaller than 5 bytes.

	if (type == FunctionHookType::EntireReplacement) {
        
        info = { src, dst, nullptr, type, code_size };
        
        // Create and commit a Microsoft Detour.
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach((void**)&src, dst);
        DetourTransactionCommit();
        
        info.original_code = src;
	}
	else if (type == FunctionHookType::IATReplacement) {
		SetExecuteReadWritePermission(src, 4);

		info = { src, dst, *(void**)src, type, 0 };

		*(void**)src = dst;
	}
	else if (type == FunctionHookType::InlineReplacement) {
		SetExecuteReadWritePermission(src, code_size);

		char* original_code = AllocateCode(code_size); // Allocate code for backed up instructions.

		RelocateCodeSegment(src, original_code, code_size); // Relocate code from the source pointer into the allocated backup buffer.

		info = { src, dst, original_code, type, code_size };

		memset(src, 0x90, code_size); // Pad out the original code with NOPs.

		WriteCALL(src, dst); // Write a function call from the source to the detour function.
	}
	else if (type == FunctionHookType::InlineReplacementJMP) {
		SetExecuteReadWritePermission(src, code_size);

		char* original_code = AllocateCode(code_size); // Allocate code for backed up instructions.

		RelocateCodeSegment(src, original_code, code_size); // Relocate code from the source pointer into the allocated backup buffer.

		info = { src, dst, original_code, type, code_size };

		memset(src, 0x90, code_size); // Pad out the original code with NOPs.

		WriteCALL(src, dst); // Write a function call from the source to the detour function.
	}
	return info;
}

HookedFunctionInfo HookFunction(uint32_t src, void* dst, const size_t code_size, FunctionHookType type) {

	HookedFunctionInfo info = HookedFunctionInfo::Default();

	if (code_size < 5 && (type == FunctionHookType::InlineReplacement || type == FunctionHookType::InlineReplacementJMP)) return info; // You cannot inline hook at code segments smaller than 5 bytes.

	else if (type == FunctionHookType::IATReplacement) {
		SetReadWritePermission((void*)src, 4);

		info = { (void*)src, dst, *(void**)src, type, 0 };

		*(void**)src = dst;
	}
	else if (type == FunctionHookType::InlineReplacement) {
		SetExecuteReadWritePermission((void*)src, code_size);

		char* original_code = AllocateCode(code_size); // Allocate code for backed up instructions.

		RelocateCodeSegment((void*)src, original_code, code_size); // Relocate code from the source pointer into the allocated backup buffer.

		info = { (void*)src, dst, original_code, type, code_size };

		memset((void*)src, 0x90, code_size); // Pad out the original code with NOPs.

		WriteCALL(src, dst); // Write a function call from the source to the detour function.
	}
	else if (type == FunctionHookType::InlineReplacementJMP) {
		SetExecuteReadWritePermission((void*)src, code_size);

		char* original_code = AllocateCode(code_size); // Allocate code for backed up instructions.

		RelocateCodeSegment((void*)src, original_code, code_size); // Relocate code from the source pointer into the allocated backup buffer.

		info = { (void*)src, dst, original_code, type, code_size };

		memset((void*)src, 0x90, code_size); // Pad out the original code with NOPs.

		WriteJMP(src, dst); // Write a function call from the source to the detour function.
	}
	return info;
}

bool UninstallFunctionHook(HookedFunctionInfo& function_info) {
	if (function_info.type == FunctionHookType::Invalid)
		return false;
    if (function_info.type == FunctionHookType::EntireReplacement) {
        // Remove the Microsoft Detour.
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach((void**)&function_info.source_address, function_info.destination_address);
        DetourTransactionCommit();
		return true;
    }
    else if (function_info.type == FunctionHookType::IATReplacement) {
        *(void**)function_info.source_address = function_info.original_code; // Set the pointer back to its original value.
		return true;
    }
    else if (function_info.type == FunctionHookType::InlineReplacement) {
        RelocateCodeSegment(function_info.original_code, function_info.source_address, function_info.code_size); // Relocate the original code back into the correct address.
        
		FreeCode(function_info.original_code); // Free the allocated code buffer.
		return true;
    }
	return false;
}
