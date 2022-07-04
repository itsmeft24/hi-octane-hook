
#include <iostream>
#include <Windows.h>

#include "../HookFunction.h"
#include "../Logging.h"
#include "LoadingScreenPatch.h"

HookedFunctionInfo loading_screen_finfo;

BYTE* lpCarsGame = (BYTE*)0x00718A74;

DWORD* ret_address = (DWORD*)0x004053A2;

const char E_UI_Tex_[] = "e\\ui\\tex\\%s";

__declspec(naked) void CalculateLoadingScreenPath() {
	
	__asm {
		mov eax, [lpCarsGame]
		mov eax, [eax]
		add eax, 0x764 // lpCarsGame->LoadingScreenPath
		push eax
		push OFFSET [E_UI_Tex_]
		lea eax, [esp + 0x10]
		push 0x104
		push eax
		jmp ret_address
	}
}

void LoadingScreenPatch::Install() {
	loading_screen_finfo = HookFunction(0x0040538e, &CalculateLoadingScreenPath, 0x14, FunctionHookType::InlineReplacementJMP);
	if (loading_screen_finfo.type != FunctionHookType::Invalid)
		Logging::Log("[LoadingScreenPatch::Install] Successfully installed patch!\n");
}