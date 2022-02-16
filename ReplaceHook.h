#pragma once
#include <Windows.h>
#include <unordered_map>

struct HookedFunctionInfo {
    DWORD* source_address;
    DWORD* detour_address;
    DWORD* backed_up_data;
    DWORD size_of_backed_up_data;

    void RestoreOriginalFunction() {
        memcpy(source_address, backed_up_data, size_of_backed_up_data);
    }

    void ReinstallReplacementHook() {
        WriteJMPImpl((char*)source_address, (char*)detour_address);
    }
};

std::unordered_map<std::string, HookedFunctionInfo> HookedFunctions;

bool InstallReplacementHook(char* src, char* dst, const DWORD instruction_size, const std::string& func_sym)
{
    if (instruction_size < 5) return false;

    DWORD oldProtect;
    VirtualProtect(src, instruction_size, PAGE_EXECUTE_READWRITE, &oldProtect);

    char* backed_up_instr = AllocateCode(instruction_size);

    memcpy(backed_up_instr, src, instruction_size);

    HookedFunctionInfo info{ (DWORD*)src, (DWORD*)dst, (DWORD*)backed_up_instr, instruction_size };

    HookedFunctions.insert({func_sym, info});

    WriteJMPImpl((char*)src, dst);

    memset(src + 5, 0x90, instruction_size - 5);

    return true;
}

HookedFunctionInfo& GetFunctionInfo(std::string sym)
{
    return HookedFunctions[sym];
}

bool UninstallReplacementHook(const std::string& func_sym)
{
    auto& info = GetFunctionInfo(func_sym);
    
    info.RestoreOriginalFunction();

    FreeCode(info.backed_up_data);

    HookedFunctions.erase(func_sym);

    return true;
}

// To call original function:
// Yeah, I know its not that intuitive, but it works.
// 
// GetFunctionInfo("PreviouslyDeclaredFunctionPointer").RestoreOriginalFunction();
// auto return_value = PreviouslyDeclaredFunctionPointer(arg1, arg2);
// GetFunctionInfo("PreviouslyDeclaredFunctionPointer").ReinstallReplacementHook();