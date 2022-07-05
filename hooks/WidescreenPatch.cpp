
#include <Windows.h>

#include "../ConfigManager.h"
#include "../HookFunction.h"
#include "../Logging.h"
#include "../Utils.h"

#include "WidescreenPatch.h"

DWORD *pWindowWidth = (DWORD *)0x006FAA54;
DWORD *pWindowHeight = (DWORD *)0x006FAA58;
DWORD *pScreenMode = (DWORD *)0x006F72CC;

__declspec(naked) void CMPPatch() { // Force the game to compare against an
                                    // intermediate 1, rather than the contents
                                    // of register EDI.
  __asm {
		cmp [pScreenMode], 1
		ret
  }
}

void WideScreenPatch::Install() {
  if (ConfigManager::IsWidescreenEnabled) {
    Logging::Log(
        "[WideScreenPatches::Install] Setting screen mode to widescreen...\n");

    HookFunction(0x00421E22, &CMPPatch, 6, FunctionHookType::InlineReplacement);

    *pScreenMode = 2;

    *pWindowWidth = ConfigManager::DesiredWindowWidth;
    *pWindowHeight = ConfigManager::DesiredWindowHeight;

    SetExecuteReadWritePermission((DWORD *)(0x00421DEF), 4);
    *(DWORD *)(0x00421DEF) = 2; // (mov edi, 2) instead of (mov edi, 1)

    // Replace 800x600 resolution case in switch-case with desired resolution.
    // (This case is almost always hit by default!)
    SetExecuteReadWritePermission((DWORD *)(0x00414D17), 4);
    *(DWORD *)(0x00414D17) =
        ConfigManager::DesiredWindowWidth; // mov [edx], DesiredWindowWidth
    SetExecuteReadWritePermission((DWORD *)(0x00414D1D), 4);
    *(DWORD *)(0x00414D1D) =
        ConfigManager::DesiredWindowHeight; // mov [eax], DesiredWindowHeight
  }
}