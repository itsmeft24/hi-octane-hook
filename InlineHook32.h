
CARSHOOK_API bool SetExecuteReadWritePermission(void* ptr, size_t sz) {
    DWORD oldProtect;
    return (bool)VirtualProtect(ptr, sz, PAGE_EXECUTE_READWRITE, &oldProtect);
}

CARSHOOK_API void WriteJMPImpl(char* src, char* dst)
{
    DWORD relativeAddress = (DWORD)(dst - (DWORD)src) - 5;

    SetExecuteReadWritePermission(src, 5);
    *src = 0xE9;
    *(DWORD*)((DWORD)src + 1) = relativeAddress;
}

CARSHOOK_API void WriteCALLImpl(char* src, char* dst)
{
    DWORD relativeAddress = (DWORD)(dst - (DWORD)src) - 5;

    SetExecuteReadWritePermission(src, 5);
    *src = 0xE8;
    *(DWORD*)((DWORD)src + 1) = relativeAddress;
}

CARSHOOK_API bool InlineHook32Impl(char* src, char* dst, const DWORD len)
{
    if (len < 5) return false;

    DWORD oldProtect;
    if (!SetExecuteReadWritePermission(src, len))
        return false;

    char* trampoline = (char*)VirtualAlloc(NULL, len + 10 + 2, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    memcpy(trampoline + 4 + 5, src, len); // Write overwritten instruction.

    WriteJMPImpl((char*)src, trampoline);
    memset(src + 5, 0x90, len - 5); // Make sure the overwritten data is padded with NOPs.

    memset(trampoline, 0x60, 1); // PUSHAD
    memset(trampoline + 1, 0x9C, 1); // PUSHFD

    WriteCALLImpl(trampoline + 2, (char*)dst);

    memset(trampoline + 2 + 5, 0x9D, 1); // POPFD
    memset(trampoline + 3 + 5, 0x61, 1); // POPAD

    WriteJMPImpl(trampoline + len + 4 + 5, (char*)src + len);

    return true;
}

CARSHOOK_API bool RemoveInlineHook32Impl(char* src, char* dst, const DWORD len)
{
    if (len < 5) return false;

    SetExecuteReadWritePermission(src, len);

    DWORD* trampoline_addr = *(DWORD*)(src + 1) + (DWORD*)src + 5;

    memcpy(src, trampoline_addr + 4 + 5, len);

    return (bool)VirtualFree(trampoline_addr, 0, MEM_RELEASE);
}

#define WriteJMP(src, dst) WriteJMPImpl((char*)src, (char*)dst)

#define WriteCALL(src, dst) WriteCALLImpl((char*)src, (char*)dst)

#define InlineHook32(src, len, dst) InlineHook32Impl((char*)src, (char*)dst, len)

#define RemoveInlineHook32(src, len, dst) RemoveInlineHook32Impl((char*)src, (char*)dst, len)

#define AllocateCode(size) (char*)VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

#define FreeCode(ptr) (bool)VirtualFree(ptr, 0, MEM_RELEASE);

#define DECL_CARSINLINEHOOK __declspec(naked)

#define RETURN __asm ret
