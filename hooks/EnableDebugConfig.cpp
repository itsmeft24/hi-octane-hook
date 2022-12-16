
#include "../ConfigManager.h"
#include "../Globals.h"
#include "../HookFunction.h"
#include "../Logging.h"

#include "EnableDebugConfig.h"

DeclareFunction(void, __thiscall, CarsGame_SetConfigArguments, 0x004416b0,
                void *);
DeclareFunction(void, __thiscall, CarsGame_LoadConfigFile, 0x00440360, void *,
                void *);
DeclareFunction(void, __thiscall, ParameterBlock_Ctor, 0x00587E60, void *);
DeclareFunction(void, __thiscall, ParameterBlock_Dtor, 0x005c4440, void *);
DeclareFunction(void, __thiscall, ParameterBlock_OpenFile, 0x005d87c0, void *,
                const char *, int, int, size_t *, size_t);

void __fastcall SetConfigArgumentsHook(void *this_ptr) {
  // Call original function to set the config members to their default values.
  CarsGame_SetConfigArguments(this_ptr);

  // Instantiate a ParameterBlock for the config.txt file, and pass it to
  // LoadConfigFile.
  void *pBlock = malloc(2124);
  ParameterBlock_Ctor(pBlock);
  ParameterBlock_OpenFile(pBlock, "c\\debug\\config.txt", 0, -1, nullptr, -1);

  Logging::Log("[EnableDebugConfig::SetConfigArguments] Loading configuration "
               "data from C\\Debug\\config.txt...\n");

  CarsGame_LoadConfigFile(this_ptr, pBlock);
  // Destroy the ParameterBlock and free the memory.
  ParameterBlock_Dtor(pBlock);
  free(pBlock);
}

void EnableDebugConfig::Install() {
  if (ConfigManager::EnableDebugTxtConfig) {
      HookedFunctionInfo info = HookFunction(
        (void *&)CarsGame_SetConfigArguments, &SetConfigArgumentsHook, 5,
        FunctionHookType::EntireReplacement);
    if (info.type != FunctionHookType::Invalid)
      Logging::Log(
          "[EnableDebugConfig::Install] Successfully installed patch!\n");
  }
}