
#include <filesystem>
#include <fstream>
#include <Windows.h>

#include "core/config.hpp"
#include "core/fs.hpp"
#include "core/logging.hpp"

#include "sunset/sunset.hpp"
#include "widescreen.hpp"

widescreen::SDResolution g_BootResolution = widescreen::SDResolution::_800x600;
const double WidescreenUIScaleW = 1280.0;
const double WidescreenUIScaleH = 720.0;

// Undoes a compiler optimization by the game that always resulted in g_ScreenMode being compared against itself.
DefineInlineHook(CMPPatch) {
    static void __cdecl callback(sunset::InlineCtx& ctx) {
        ctx.edi.unsigned_integer = 1;
    }
};

// Sets the boot resolution (held inside the CarsSettings class) to the user-selected one, instead of the default.
DefineInlineHook(BootResolution) {
    static void __cdecl callback(sunset::InlineCtx& ctx) {
        *reinterpret_cast<std::int32_t*>(ctx.esi.unsigned_integer + 0x2c) = std::to_underlying(g_BootResolution);
    }
};

DefineReplacementHook(SelectDimensionsFromSaveIndex) {
    static BOOL __stdcall callback(widescreen::SDResolution selected, unsigned int* width, unsigned int* height) {
        if (*g_ScreenMode == 2) {
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
        BootResolution::install_at_ptr(0x0048f22f);

        // Set the resolution to the default for widescreen, (1280x720)
        std::uint32_t* g_WindowWidth = reinterpret_cast<std::uint32_t*>(0x006FAA54);
        std::uint32_t* g_WindowHeight = reinterpret_cast<std::uint32_t*>(0x006FAA58);
        
        *g_WindowWidth = 1280;
        *g_WindowHeight = 720;
        *g_ScreenMode = 2;

        // Set the boot resolution to fall back to 1280x720 (corresponds to SDResolution::_1024x768) if no save files are found.
        g_BootResolution = widescreen::SDResolution::_1024x768;

        // Attempt to find the first save file to override the default with a user-selected resolution.
        std::filesystem::path save = fs::save_dir();
        for (std::size_t index = 0; index < 5; index++) {
            std::ifstream save_file(save / std::format("PSLOT{}.dat", index), std::ios::in | std::ios::binary);
            if (save_file) {
                std::uint8_t raw_resolution_index = 0;
                save_file.seekg(0x70);
                save_file.read(reinterpret_cast<char*>(&raw_resolution_index), 1);

                g_BootResolution = static_cast<SDResolution>(raw_resolution_index);
                
                const auto& [selected_width, selected_height] = resolve_hd(g_BootResolution);
                *g_WindowWidth = selected_width;
                *g_WindowHeight = selected_height;

                break;
            }
        }
    
        logging::log("[widescreen::install] Setting screen mode to widescreen, selected resolution is: {}x{}...", *g_WindowWidth, *g_WindowHeight);

        sunset::utils::set_permission(reinterpret_cast<void*>(0x00421DEF), 4, sunset::utils::Perm::ExecuteReadWrite);
        *reinterpret_cast<std::uint32_t*>(0x00421DEF) = 2; // (mov edi, 2) instead of (mov edi, 1)

        // Fixes scaling issues with mouse naviagation.
        sunset::utils::set_permission(reinterpret_cast<void*>(0x00455bfd), 6, sunset::utils::Perm::ExecuteReadWrite);
        sunset::utils::set_permission(reinterpret_cast<void*>(0x00455c20), 6, sunset::utils::Perm::ExecuteReadWrite);
        *reinterpret_cast<const double**>(0x00455bfd + 2) = &WidescreenUIScaleW;
        *reinterpret_cast<const double**>(0x00455c20 + 2) = &WidescreenUIScaleH;

        sunset::utils::set_permission(reinterpret_cast<void*>(0x00454ddd), 6, sunset::utils::Perm::ExecuteReadWrite);
        sunset::utils::set_permission(reinterpret_cast<void*>(0x00454e00), 6, sunset::utils::Perm::ExecuteReadWrite);
        *reinterpret_cast<const double**>(0x00454ddd + 2) = &WidescreenUIScaleW;
        *reinterpret_cast<const double**>(0x00454e00 + 2) = &WidescreenUIScaleH;

        sunset::utils::set_permission(reinterpret_cast<void*>(0x00464070), 6, sunset::utils::Perm::ExecuteReadWrite);
        sunset::utils::set_permission(reinterpret_cast<void*>(0x00464093), 6, sunset::utils::Perm::ExecuteReadWrite);
        *reinterpret_cast<const double**>(0x00464070 + 2) = &WidescreenUIScaleW;
        *reinterpret_cast<const double**>(0x00464093 + 2) = &WidescreenUIScaleH;

        sunset::utils::set_permission(reinterpret_cast<void*>(0x004609fd), 6, sunset::utils::Perm::ExecuteReadWrite);
        sunset::utils::set_permission(reinterpret_cast<void*>(0x00460a20), 6, sunset::utils::Perm::ExecuteReadWrite);
        *reinterpret_cast<const double**>(0x004609fd + 2) = &WidescreenUIScaleW;
        *reinterpret_cast<const double**>(0x00460a20 + 2) = &WidescreenUIScaleH;

        sunset::utils::set_permission(reinterpret_cast<void*>(0x004659dd), 6, sunset::utils::Perm::ExecuteReadWrite);
        sunset::utils::set_permission(reinterpret_cast<void*>(0x00465a00), 6, sunset::utils::Perm::ExecuteReadWrite);
        *reinterpret_cast<const double**>(0x004659dd + 2) = &WidescreenUIScaleW;
        *reinterpret_cast<const double**>(0x00465a00 + 2) = &WidescreenUIScaleH;

        // These offsets have something to do with 3D UI components... which the original game never uses. So I guess it's fine to leave this here?
        sunset::utils::set_permission(reinterpret_cast<void*>(0x0062e5f0), 6, sunset::utils::Perm::ExecuteReadWrite);
        sunset::utils::set_permission(reinterpret_cast<void*>(0x0062e616), 6, sunset::utils::Perm::ExecuteReadWrite);
        *reinterpret_cast<const double**>(0x0062e5f0 + 2) = &WidescreenUIScaleW;
        *reinterpret_cast<const double**>(0x0062e616 + 2) = &WidescreenUIScaleH;
    }
}