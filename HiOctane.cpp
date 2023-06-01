
#include <Shlwapi.h>
#include <Windows.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <format>

#include "ConfigManager.h"
#include "FileSystem.h"
#include "Globals.h"
#include "patches.h"
#include "Logging.h"
#include "PluginManager.h"
#include "framework.hpp"

std::filesystem::path g_DataDir;
std::filesystem::path g_InstallDir;

constexpr std::string_view kDataDirName = "hi-octane";

// This string needs to have a static lifetime as a pointer to it will be given to the game.
static std::string data_dir_fmt = std::format("%s\\{}\\", kDataDirName);

extern "C" __declspec(dllexport) const char *VERSION = "1.9.2.3";

void init() {
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
  
    config::read();
  
    logging::init();

    logging::log("[hi-octane::init] Installing hooks...");
  
    fs::init();

    widescreen::install();
  
    dialogue_list::install();
  
    data_access_logging::install();
  
    large_vehicles::install();
  
    loading_screen_fix::install();
  
    game_text_json::install();
  
    debug_txt_support::install();
  
    fix_ui_sounds::install();
  
    explore_music::install();

    // HDRPatch::install();
  
    plugin_manager::load_plugins();
  
    plugin_manager::start_plugins();
}

void deinit() {
    logging::log("[hi-octane::deinit] Exiting...");

    plugin_manager::exit_plugins();

    logging::deinit();
}

// Initialization and de-initialization is now placed inside this thin WinMain wrapper.
// This enables us to call MN's std library functions on HiOctaneEntry.
DefineReplacementHook(WinMainHook) {
    static int __stdcall callback(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
        init();
        int result = original(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
        deinit();
        return result;
    }
};

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        WinMainHook::install_at_ptr(0x006196a0);
    }
    return TRUE;
}
