
#include <Windows.h>
#include <filesystem>
#include <fstream>

#include "core/config.hpp"
#include "core/logging.hpp"
#include "core/fs.hpp"

#include "widescreen.hpp"
#include "core/hooking/framework.hpp"

DWORD *pWindowWidth = (DWORD *)0x006FAA54;
DWORD *pWindowHeight = (DWORD *)0x006FAA58;
DWORD *pScreenMode = (DWORD *)0x006F72CC;

// Undoes a compiler optimization by the game that always resulted in pScreenMode being compared against itself.
DefineInlineHook(CMPPatch) {
    static void __cdecl callback(hooking::InlineContext ctx) {
        ctx.edi.unsigned_integer = 1;
    }
};

DefineReplacementHook(SelectDimensionsFromSaveIndex) {
    static BOOL __stdcall callback(widescreen::SDResolution selected, unsigned int* width, unsigned int* height) {
        if (config::g_WidescreenEnabled) {
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

void widescreen::install() {
    if (config::g_WidescreenEnabled) {

        SelectDimensionsFromSaveIndex::install_at_ptr(0x00414ce0);
        CMPPatch::install_at_ptr(0x00421e22);


        // Set the resolution to the default for widescreen, (1280x720)
        *pWindowWidth = 1280;
        *pWindowHeight = 720;
        *pScreenMode = 2;

        // Attempt to find the first save file to override the default with a user-selected resolution.
        std::filesystem::path save = fs::save_dir();
        for (std::size_t index = 0; index < 5; index++) {
            std::ifstream save_file(save / std::format("PSLOT{}.dat", index), std::ios::in | std::ios::binary);
            if (save_file) {
                std::uint8_t raw_resolution_index = 0;
                save_file.seekg(0x70);
                save_file.read(reinterpret_cast<char*>(&raw_resolution_index), 1);

                const auto& [selected_width, selected_height] = resolve_hd(static_cast<SDResolution>(raw_resolution_index));
                *pWindowWidth = selected_width;
                *pWindowHeight = selected_height;

                break;
            }
        }
    
        logging::log("[widescreen::install] Setting screen mode to widescreen, selected resolution is: {}x{}...", *pWindowWidth, *pWindowHeight);

        winapi::set_permission(0x00421DEF, 4, winapi::Perm::ExecuteReadWrite);
        *reinterpret_cast<std::uint32_t*>(0x00421DEF) = 2; // (mov edi, 2) instead of (mov edi, 1)
    }
}