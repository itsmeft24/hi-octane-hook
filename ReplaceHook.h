#pragma once
#include "InlineHook32.h"
#include <Windows.h>
#include <unordered_map>

std::unordered_map<std::string, DWORD*> BackedUpFunctions;

bool InstallReplacementHook(char* src, char* dst, const DWORD src_function_size, const std::string& func_sym)
{

    DWORD oldProtect;
    VirtualProtect(src, src_function_size, PAGE_EXECUTE_READWRITE, &oldProtect);

    char* copied_func = AllocateCode(src_function_size);

    memcpy(copied_func, src, src_function_size);

    BackedUpFunctions.insert({func_sym, (DWORD*)copied_func});

    WriteJMPImpl((char*)src, dst);

    return true;
}

void* GetBackedUpFunction(std::string sym)
{
    return BackedUpFunctions[sym];
}

bool UninstallReplacementHook(char* src, const std::string& func_sym)
{
    DWORD* backed_up_func = (DWORD*)GetBackedUpFunction(func_sym);
    
    memcpy(src, backed_up_func, 5);

    FreeCode(backed_up_func);

    BackedUpFunctions.erase(func_sym);
}

#define CallBackedUpFunction(ret_type, call_conv, sym, ...) (ret_type(call_conv*)(##__VA_ARGS__))(GetBackedUpFunction(sym))()