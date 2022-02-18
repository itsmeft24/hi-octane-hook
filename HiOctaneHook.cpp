#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <Windows.h>
#include <Shlwapi.h>

static std::string CURRENT_DIRECTORY;

static std::string DATA_DIR_PATH;

constexpr auto WindowTitle = "Cars: Hi-Octane Helper Plugin";

constexpr auto DATA_DIR = "datapc"; // Can be swapped out with an alternate string. (unimplemented lol)

#include "Utils.h"
#include "InlineHook32.h"
#include "ReplaceHook.h"
#include "InlineContext.h"
#include "Offsets.h"
#include "Logging.h"
#include "Hooks.h"

extern "C" __declspec(dllexport) const char* VERSION = "0.1.0"; // Will be used for updating 
extern "C" __declspec(dllexport) const char* REQUIRE_HI_OCTANE_VERSION = "1.9.2.3"; // Will be used for updating 

extern "C" __declspec(dllexport) void HiOctaneEntry()
{
    char CURR_DIR_BUF[260];
    GetModuleFileNameA(NULL, CURR_DIR_BUF, 260);
    PathRemoveFileSpecA(CURR_DIR_BUF);
    CharLowerA(CURR_DIR_BUF);
    CURRENT_DIRECTORY = CURR_DIR_BUF;
    DATA_DIR_PATH = CURRENT_DIRECTORY + "\\" + DATA_DIR;
    // Get current directory and store it in a global variable. (Used for File IO stuff.)

    Logging::Init();

    Logging::Log("[HiOctaneEntry] Installing hooks...\n");

    CarsActivityUI_RequestDialogueHook::install();

    ModSupport::install();

    DataAccessLogging::install();
}

extern "C" __declspec(dllexport) void HiOctaneExit() {

    Logging::Log("[HiOctaneExit] Exiting...\n");
    
    DataAccessLogging::uninstall();
    
    ModSupport::uninstall();

    CarsActivityUI_RequestDialogueHook::uninstall();

    Logging::Deinit();
    
    FreeLibraryAndExitThread(GetModuleHandleA(NULL), 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {

    return TRUE;

}