#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <Windows.h>
#include <shlwapi.h>

static std::string CURRENT_DIRECTORY;

#include "InlineHook32.h"
#include "InlineContext.h"
#include "Offsets.h"
#include "Logging.h"
#include "Hooks.h"
#include "ParameterBlockClass.h"

extern "C" __declspec(dllexport) const char* VERSION = "0.1.0"; // Will be used for updating 
extern "C" __declspec(dllexport) const char* REQUIRE_HI_OCTANE_VERSION = "1.9.2.3"; // Will be used for updating 

constexpr auto WindowTitle = "Cars: Hi-Octane Helper Plugin";

constexpr auto DATA_DIR = "DataPC"; // Can be swapped out with an alternate string. (unimplemented lol)

DWORD WINAPI HiOctaneMain(HMODULE hModule)
{
    // Create Console
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    SetConsoleTitleA(WindowTitle);

    char CURR_DIR_BUF[260];
    GetModuleFileNameA(NULL, CURR_DIR_BUF, 260);
    PathRemoveFileSpecA(CURR_DIR_BUF);
    CURRENT_DIRECTORY = CURR_DIR_BUF;
    // Get current directory and store it in a global variable. (Used for File IO stuff.)

    Logging::Init();

    DataAccessLogging::install();

    CarsActivityUI_RequestDialogueHook::install(); // Others will be held in their own namespace, and their code will be in Hooks.h.

    while (true)
    {
        // MainLoop

        if (GetAsyncKeyState(VK_END) & 1)
        {
            break;
        }
    }
    
    Logging::Deinit();

    CarsActivityUI_RequestDialogueHook::uninstall();

    DataAccessLogging::uninstall();

	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
    return 0;

}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {

    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 4096, (LPTHREAD_START_ROUTINE)HiOctaneMain, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;

}