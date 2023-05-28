
#include <Windows.h>
#include <filesystem>
#include <fstream>

#include "../ConfigManager.h"
#include "../HookFunction.h"
#include "../Logging.h"
#include "../Utils.h"
#include "../FileSystem.h"

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

BOOL __stdcall GetDimensionsFromSelectedResolutionHook(WideScreenPatch::SDResolution selected, unsigned int* width, unsigned int* height) {
    if (ConfigManager::IsWidescreenEnabled) {
        const auto& [selected_width, selected_height] =  resolve_hd(selected);
        *width = selected_width;
        *height = selected_height;
    }
    else {
        const auto& [selected_width, selected_height] = resolve_sd(selected);
        *width = selected_width;
        *height = selected_height;
    }
    return TRUE;
}

void WideScreenPatch::install() {
  if (ConfigManager::IsWidescreenEnabled) {
    Logging::log(
        "[WideScreenPatches::Install] Setting screen mode to widescreen...");

    HookFunction(0x00421E22, &CMPPatch, 6, FunctionHookType::InlineReplacement);
    HookFunction((void*&)GetDimensionsFromSelectedResolution, &GetDimensionsFromSelectedResolutionHook, 7, FunctionHookType::EntireReplacement);

    std::filesystem::path save = FileSystem::save_dir();
    save.concat("GlobalData");
    std::ifstream save_file(save, std::ios::in | std::ios::binary);
    save_file.seekg(0x18);
    std::uint8_t i = 0;
    save_file.read((char*)&i, 1);
    GetDimensionsFromSelectedResolutionHook((SDResolution)i, (unsigned int*)pWindowWidth, (unsigned int*)pWindowHeight);
    save_file.close();

    *pScreenMode = 2;
    SetExecuteReadWritePermission((DWORD *)(0x00421DEF), 4);
    *(DWORD *)(0x00421DEF) = 2; // (mov edi, 2) instead of (mov edi, 1)
  }
}