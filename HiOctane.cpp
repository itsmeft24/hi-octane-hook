
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
#include "Hooks.h"
#include "Logging.h"
#include "PluginManager.h"
#include "Utils.h"

std::filesystem::path g_DataDir;
std::filesystem::path g_InstallDir;

constexpr std::string_view kDataDirName = "hi-octane";

// This string needs to have a static lifetime as a pointer to it will be given to the game.
static std::string data_dir_fmt = std::format("%s\\{}\\", kDataDirName);

extern "C" __declspec(dllexport) const char *VERSION = "1.9.2.3";

void HiOctaneEntry() {
  WritePUSH(AsVoidPtr(0x00619929), AsVoidPtr(data_dir_fmt.c_str()));
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
  
  ConfigManager::read();
  
  Logging::init();

  Logging::log("[HiOctaneEntry] Installing hooks...");
  
  FileSystem::init();

  WideScreenPatch::install();
  
  DialogueListEx::install();
  
  DataAccessLogging::install();
  
  LargeVehiclePatch::install();
  
  LoadingScreenPatch::install();
  
  GameTextJSON::install();
  
  EnableDebugConfig::install();
  
  MiscUIFixes::install();
  
  ExploreMusic::install();
  
  PluginManager::load_plugins();
  
  PluginManager::start_plugins();
}

void HiOctaneExit() {

  Logging::log("[HiOctaneExit] Exiting...");

  PluginManager::exit_plugins();

  Logging::deinit();
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

HookedFunctionInfo winmain_info;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {

  if (reason == DLL_PROCESS_ATTACH) {
      winmain_info = HookFunction((void*&)_WinMain, WinMainHook, 6, FunctionHookType::EntireReplacement);
  } else if (reason == DLL_PROCESS_DETACH) {
      UninstallFunctionHook(winmain_info);
  }
  return TRUE;
}
