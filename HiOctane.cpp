#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX

#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <Windows.h>
#include <Shlwapi.h>

static std::string CURRENT_DIRECTORY;

static std::string DATA_DIR_PATH;

constexpr auto WindowTitle = "Cars: Hi-Octane Console";

constexpr auto DATA_DIR = "hi-octane";

const char* DATA_DIR_PATCH = "%s\\Hi-Octane\\"; // Replaces DataPC.

extern "C" __declspec(dllexport) const char* VERSION = "1.9.2.3";

#include "Utils.h"
#include "InlineContext.h"
#include "HookFunction.h"

#include "Offsets.h"
#include "Logging.h"
#include "PluginManager.h"
#include "ConfigManager.h"
#include "FileSystem.h"
#include "Hooks.h"

void HiOctaneEntry()
{
    WritePUSH(0x00619929, DATA_DIR_PATCH);
    // Set the games' DataPC path to our own.

    char CURR_DIR_BUF[260];
    GetModuleFileNameA(NULL, CURR_DIR_BUF, 260);
    PathRemoveFileSpecA(CURR_DIR_BUF);
    CharLowerA(CURR_DIR_BUF);
    CURRENT_DIRECTORY = CURR_DIR_BUF;
    DATA_DIR_PATH = CURRENT_DIRECTORY + "\\" + DATA_DIR;
    // Get current directory and store it in a global variable. (Used for File IO stuff.)

    Logging::Init();

    Logging::Log("[HiOctaneEntry] Installing hooks...\n");

    ConfigManager::ReadConfigFile();

    FileSystem::Init();

    WideScreenPatches::Install();

    DialogueListEx::Install();

    DataAccessLogging::Install();

    LargeVehiclePatch::Install();

    LoadingScreenPatch::Install();
	
    AutomaticBoostPatch::Install();

    // CarsDialoguePatches::install();

    // CarsDialogueEX::Install();

    // GameTextPatches::install();

    PluginManager::LoadAllPlugins();

    PluginManager::StartAllPlugins();

#ifdef _DEBUG
    unsigned long long WaitForExternalEdit = 8389773196515696983;
    Logging::Log("[HiOctaneEntry] Waiting for debugger to attach...");
    while (WaitForExternalEdit == 8389773196515696983) {
        Sleep(100);
    }
    Logging::Log("[HiOctaneEntry] Debugger attached!");  
#endif

}

void HiOctaneExit() {
    
    Logging::Log("[HiOctaneExit] Exiting...\n");

    LargeVehiclePatch::Uninstall();

    DataAccessLogging::Uninstall();

    DialogueListEx::Uninstall();
    
    PluginManager::ExitAllPlugins();

    FileSystem::Deinit();

    Logging::Deinit();

    FreeLibraryAndExitThread(GetModuleHandleA(NULL), 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {

    if (reason == DLL_PROCESS_ATTACH) {
        HiOctaneEntry();
    }
    else if (reason == DLL_PROCESS_DETACH) {
        HiOctaneExit();
    }
    return TRUE;
}
