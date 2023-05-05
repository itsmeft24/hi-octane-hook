
#include <Shlwapi.h>
#include <Windows.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#include "ConfigManager.h"
#include "FileSystem.h"
#include "Globals.h"
#include "Hooks.h"
#include "Logging.h"
#include "PluginManager.h"
#include "Utils.h"

std::string DATA_DIR_PATH;// = "";
std::string CURRENT_DIRECTORY;// = "";

constexpr auto DATA_DIR = "hi-octane";

const char *DATA_DIR_PATCH = "%s\\Hi-Octane\\"; // Replaces DataPC.

extern "C" __declspec(dllexport) const char *VERSION = "1.9.2.3";

void HiOctaneEntry() {
  WritePUSH(AsVoidPtr(0x00619929), AsVoidPtr(DATA_DIR_PATCH));
  // Set the games' DataPC path to our own.

  char CURR_DIR_BUF[260];
  GetModuleFileNameA(NULL, CURR_DIR_BUF, 260);
  PathRemoveFileSpecA(CURR_DIR_BUF);
  CharLowerA(CURR_DIR_BUF);
  CURRENT_DIRECTORY = CURR_DIR_BUF;
  DATA_DIR_PATH = CURRENT_DIRECTORY + "\\" + DATA_DIR;
  // Get current directory and store it in a global variable. (Used for File IO
  // stuff.)
  
  ConfigManager::ReadConfigFile();
  
  Logging::Init();

  Logging::Log("[HiOctaneEntry] Installing hooks...\n");
  
  FileSystem::Init();

  WideScreenPatch::Install();

  DialogueListEx::Install();

  DataAccessLogging::Install();

  LargeVehiclePatch::Install();

  LoadingScreenPatch::Install();

  GameTextJSON::Install();

  EnableDebugConfig::Install();

  MiscUIFixes::Install();

  ExploreMusic::Install();

  PluginManager::LoadAllPlugins();

  PluginManager::StartAllPlugins();
}

void HiOctaneExit() {

  Logging::Log("[HiOctaneExit] Exiting...\n");

  PluginManager::ExitAllPlugins();

  Logging::Deinit();

  FreeLibraryAndExitThread(GetModuleHandleA(NULL), 0);
}

// Initialization and de-initialization is now placed inside this thin WinMain wrapper.
// This enables us to call MN's std library functions on HiOctaneEntry.
DeclareFunction(int, __stdcall, _WinMain, 0x006196a0, HINSTANCE, HINSTANCE, PSTR, int);

int __stdcall WinMainHook(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
    HiOctaneEntry();
    int result = _WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    HiOctaneExit();
    return result;
}

HookedFunctionInfo mainInfo;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {

  if (reason == DLL_PROCESS_ATTACH) {
      mainInfo = HookFunction((void*&)_WinMain, WinMainHook, 6, FunctionHookType::EntireReplacement);
  } else if (reason == DLL_PROCESS_DETACH) {
      UninstallFunctionHook(mainInfo);
  }
  return TRUE;
}
