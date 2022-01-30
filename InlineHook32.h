
void WriteJMPImpl(char* src, char* dst)
{
    DWORD relativeAddress = (DWORD)(dst - (DWORD)src) - 5;

    *src = 0xE9;
    *(DWORD*)((DWORD)src + 1) = relativeAddress;
}

void WriteCALLImpl(char* src, char* dst)
{
    DWORD relativeAddress = (DWORD)(dst - (DWORD)src) - 5;

    *src = 0xE8;
    *(DWORD*)((DWORD)src + 1) = relativeAddress;
}

bool InlineHook32Impl(char* src, char* dst, const DWORD len)
{
    if (len < 5) return false;

    DWORD oldProtect;
    VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oldProtect);

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

#define WriteJMP(src, dst) WriteJMPImpl((char*)src, (char*)dst)

#define WriteCALL(src, dst) WriteCALLImpl((char*)src, (char*)dst)

#define InlineHook32(src, len, dst) InlineHook32Impl((char*)src, (char*)dst, len)

#define AllocateCode(size) (char*)VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

#define DECL_CARSHOOK __declspec(naked)

#define RETURN __asm ret
