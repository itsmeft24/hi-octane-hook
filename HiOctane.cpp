
#include <Shlwapi.h>
#include <Windows.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <format>

#include "core/bink_reexport.hpp"
#include "core/config.hpp"
#include "core/fs.hpp"
#include "core/globals.hpp"
#include "patches/patches.hpp"
#include "core/logging.hpp"
#include "core/plugin_manager.hpp"
#include "core/hooking/framework.hpp"
#include "core/update.hpp"

std::filesystem::path g_DataDir;
std::filesystem::path g_InstallDir;

constexpr std::string_view kDataDirName = "hi-octane";

// This string needs to have a static lifetime as a pointer to it will be given to the game.
static std::string data_dir_fmt = std::format("%s\\{}\\", kDataDirName);

extern "C" __declspec(dllexport) const char *VERSION = "2.0.0";

bool init() {
    hooking::write_push(0x00619929, data_dir_fmt.c_str());
    // Set the games' DataPC path to our own.

    wchar_t CURR_DIR_BUF[260];
    GetModuleFileNameW(NULL, CURR_DIR_BUF, 260);
    PathRemoveFileSpecW(CURR_DIR_BUF);
    CharLowerW(CURR_DIR_BUF);
    g_InstallDir = CURR_DIR_BUF;
    g_DataDir = g_InstallDir;
    g_DataDir.append(kDataDirName);

    // Get current directory and store it in a global variable. (Used for File IO
    // stuff.)

    std::srand(std::time(0));

    // Set the random seed.

    update::delete_temporary_files();

    logging::setup_log_file();

    config::read();

    if (config::g_ConsoleWindowEnabled) {
        logging::create_console_window();
    }

    logging::log("[hi-octane::init] Installing hooks...");
  
    fs::init();

    widescreen::install();
    
    cars_dialogue_ex::install();
  
    dialogue_list::install();
  
    data_access_logging::install();

    intro_videos::install();
  
    large_vehicles::install();
  
    loading_screen_name::install();
  
    game_text_json::install();
  
    debug_txt_support::install();
  
    ui_sounds_fix::install();

    playlist_events::install();

    misc::install();

    // HDRPatch::install();

#ifndef _DEBUG
    if (config::g_AutomaticUpdatesEnabled) {
        if (update::check_for_updates()) {
            if (update::download_latest_release()) {
                return false;
            }
            else {
                plugin_manager::load_plugins();
                plugin_manager::start_plugins();
                return true;
            }
        }
        plugin_manager::load_plugins();
        plugin_manager::start_plugins();
        return true;
    }
#endif

    plugin_manager::load_plugins();
    plugin_manager::start_plugins();
    return true;
}

void deinit() {
    logging::log("[hi-octane::deinit] Exiting...");

    plugin_manager::exit_plugins();

    logging::cleanup();
}

// Initialization and de-initialization is now placed inside this thin WinMain wrapper.
// This enables us to call MN's std library functions on HiOctaneEntry.
DefineReplacementHook(WinMainHook) {
    static int __stdcall callback(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
        if (init()) {
            int result = original(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
            deinit();
            return result;
        }
        return 0;
    }
};

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        WinMainHook::install_at_ptr(0x006196a0);
        bink_reexport::load();
    }
    if (reason == DLL_PROCESS_DETACH) {
        bink_reexport::unload();
    }
    return TRUE;
}
