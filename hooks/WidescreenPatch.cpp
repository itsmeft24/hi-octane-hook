
#include <Windows.h>

#include "../ConfigManager.h"
#include "../HookFunction.h"
#include "../Logging.h"
#include "../Utils.h"

#include "WidescreenPatch.h"

DWORD *pWindowWidth = (DWORD *)0x006FAA54;
DWORD *pWindowHeight = (DWORD *)0x006FAA58;
DWORD *pScreenMode = (DWORD *)0x006F72CC;

DeclareFunction(BOOL, __stdcall, GetDimensionsFromSelectedResolution, 0x00414ce0, int, unsigned int*, unsigned int*);

__declspec(naked) void CMPPatch() { // Force the game to compare against an
                                    // intermediate 1, rather than the contents
                                    // of register EDI.
  __asm {
		cmp [pScreenMode], 1
		ret
  }
}

enum class SDResolution : int {
    _640x480 = 0,
    _800x600,
    _1024x768,
    _1152x864,
    _1280x768,
    _1280x800,
    _1280x1024,
    _1440x900,
    _1600x1200
};

BOOL __stdcall GetDimensionsFromSelectedResolutionHook(SDResolution selected, unsigned int* width, unsigned int* height) {
    if (ConfigManager::IsWidescreenEnabled) {
        *width = ConfigManager::DesiredWindowWidth;
        *height = ConfigManager::DesiredWindowHeight;
        return TRUE;
    }
    switch (selected) {
    case SDResolution::_640x480:
        *width = 640;
        *height = 480;
        break;
    case SDResolution::_800x600:
        *width = 800;
        *height = 600;
        break;
    case SDResolution::_1024x768:
        *width = 1024;
        *height = 768;
        break;
    case SDResolution::_1152x864:
        *width = 1152;
        *height = 864;
        break;
    case SDResolution::_1280x768:
        *width = 1280;
        *height = 768;
        break;
    case SDResolution::_1280x800:
        *width = 1280;
        *height = 800;
        break;
    case SDResolution::_1280x1024:
        *width = 1280;
        *height = 1024;
        break;
    case SDResolution::_1440x900:
        *width = 1440;
        *height = 900;
        break;
    case SDResolution::_1600x1200:
        *width = 1600;
        *height = 1200;
        break;
    }
    return TRUE;
}

void WideScreenPatch::Install() {
  if (ConfigManager::IsWidescreenEnabled) {
    Logging::Log(
        "[WideScreenPatches::Install] Setting screen mode to widescreen...\n");

    HookFunction(0x00421E22, &CMPPatch, 6, FunctionHookType::InlineReplacement);
    HookFunction((void*&)GetDimensionsFromSelectedResolution, &GetDimensionsFromSelectedResolutionHook, 7, FunctionHookType::EntireReplacement);

    *pScreenMode = 2;

    *pWindowWidth = ConfigManager::DesiredWindowWidth;
    *pWindowHeight = ConfigManager::DesiredWindowHeight;

    SetExecuteReadWritePermission((DWORD *)(0x00421DEF), 4);
    *(DWORD *)(0x00421DEF) = 2; // (mov edi, 2) instead of (mov edi, 1)
  }
}