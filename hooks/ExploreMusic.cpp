#include "../HookFunction.h"
#include "../Logging.h"
#include "../Utils.h"

#include "ExploreMusic.h"

DeclareFunction(void, __thiscall, Music_SetCurrentPlaylist, 0x00574af0, void*, int);

BYTE* LpCarsGame = (BYTE*)0x00718a74;
DWORD* sret_address = (DWORD*)0x004eb879;

__declspec(naked) void CarsActivityExplore_MusicReEnabled() {

    __asm {
        mov eax, [LpCarsGame]
        mov eax, [eax]
        mov ecx, dword ptr[eax + 0x440]
        mov ecx, dword ptr[ecx + 0x40]
        push 0xd
        Call Music_SetCurrentPlaylist
        mov edx, [LpCarsGame]
        mov edx, [edx]
        mov eax, dword ptr[edx + 0x440]
        mov ecx, dword ptr[eax + 0x40]
        mov edx, dword ptr[ecx]
        mov eax, dword ptr[edx + 0xa4]
        push 0x1
        call eax
        jmp sret_address
    }
}

void ExploreMusic::install() {
    HookedFunctionInfo info =
        HookFunction(0x004eb861, &CarsActivityExplore_MusicReEnabled, 0x18, FunctionHookType::InlineReplacementJMP);
    if (info.type != FunctionHookType::Invalid) {
        Logging::log(
            "[ExploreMusic::Install] Successfully installed patch!");
    }
};