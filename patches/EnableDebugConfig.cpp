
#include "../ConfigManager.h"
#include "../Globals.h"
#include "../framework.hpp"
#include "../Logging.h"

#include "EnableDebugConfig.h"

struct ParameterBlock;
struct CarsGame;

DeclareFunction(void, __thiscall, CarsGame_LoadConfigFile, 0x00440360, CarsGame*, void *);
DeclareFunction(void, __thiscall, ParameterBlock_Ctor, 0x00587E60, ParameterBlock*);
DeclareFunction(void, __thiscall, ParameterBlock_Dtor, 0x005c4440, ParameterBlock*);
DeclareFunction(void, __thiscall, ParameterBlock_OpenFile, 0x005d87c0, ParameterBlock*, const char *, int, int, size_t *, size_t);

DefineReplacementHook(CarsGameSetConfigArguments) {
    static void __fastcall callback(CarsGame* this_ptr) {
        // Call original function to set the config members to their default values.
        original(this_ptr);

        // Instantiate a ParameterBlock for the config.txt file, and pass it to
        // LoadConfigFile.
        auto* pBlock = reinterpret_cast<ParameterBlock*>(malloc(2124));
        ParameterBlock_Ctor(pBlock);
        ParameterBlock_OpenFile(pBlock, "c\\debug\\config.txt", 0, -1, nullptr, -1);

        logging::log("[CarsGame::SetConfigArguments] Loading configuration "
            "data from C\\Debug\\config.txt...");

        CarsGame_LoadConfigFile(this_ptr, pBlock);
        // Destroy the ParameterBlock and free the memory.
        ParameterBlock_Dtor(pBlock);
        free(pBlock);
    }
};

void debug_txt_support::install() {
  if (config::g_DebugTxtConfigEnabled) {
      CarsGameSetConfigArguments::install_at_ptr(0x004416b0);
      logging::log("[debug_txt_support::install] Successfully installed patch!");
  }
}