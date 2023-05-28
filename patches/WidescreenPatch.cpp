
#include <Windows.h>
#include <filesystem>
#include <fstream>

#include "../ConfigManager.h"
//#include "../HookFunction.h"
#include "../Logging.h"
#include "../Utils.h"
#include "../FileSystem.h"

#include "WidescreenPatch.h"
#include "../framework.hpp"


DWORD *pWindowWidth = (DWORD *)0x006FAA54;
DWORD *pWindowHeight = (DWORD *)0x006FAA58;
DWORD *pScreenMode = (DWORD *)0x006F72CC;

//__declspec(naked) void CMPPatch() { // Force the game to compare against an
//                                    // intermediate 1, rather than the contents
//                                    // of register EDI.
//  __asm {
//		cmp [pScreenMode], 1
//		ret
//  }
//}

DefineInlineHook(CMPPatch) {
    static void __cdecl callback(hooking::InlineContext ctx) {
        ctx.edi.unsigned_integer = 1;
    }
};

DefineReplacementHook(SelectDimensionsFromSaveIndex) {
    static BOOL __stdcall callback(WideScreenPatch::SDResolution selected, unsigned int* width, unsigned int* height) {
        if (ConfigManager::IsWidescreenEnabled) {
            const auto& [selected_width, selected_height] = resolve_hd(selected);
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
};

void WideScreenPatch::install() {
  if (ConfigManager::IsWidescreenEnabled) {
    Logging::log("[WideScreenPatches::Install] Setting screen mode to widescreen...");


    SelectDimensionsFromSaveIndex::install_at_ptr(0x00414ce0);
    CMPPatch::install_at_ptr(0x00421e22);

    std::filesystem::path save = FileSystem::save_dir();
    save.concat("GlobalData");
    std::ifstream save_file(save, std::ios::in | std::ios::binary);
    save_file.seekg(0x18);
    std::uint8_t i = 0;
    save_file.read((char*)&i, 1);
    save_file.close();

    const auto& [selected_width, selected_height] = resolve_hd((SDResolution)i);
    *pWindowWidth = selected_width;
    *pWindowHeight = selected_height;

    *pScreenMode = 2;
    
    winapi::set_permission(0x00421DEF, 4, winapi::Perm::ExecuteReadWrite);
    *reinterpret_cast<std::uint32_t*>(0x00421DEF) = 2; // (mov edi, 2) instead of (mov edi, 1)
  }
}