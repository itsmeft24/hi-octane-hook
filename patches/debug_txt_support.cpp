
#include "core/config.hpp"
#include "core/globals.hpp"
#include "sunset/sunset.hpp"
#include "core/logging.hpp"
#include "core/game/parameter_block.hpp"
#include "core/game/cars_game.hpp"

#include "debug_txt_support.hpp"


DeclareFunction(void, __thiscall, CarsGame_LoadConfigFile, 0x00440360, CarsGame*, void *);

DefineReplacementHook(CarsGameSetConfigArguments) {
    static void __fastcall callback(CarsGame* this_ptr) {
        // Call original function to set the config members to their default values.
        original(this_ptr);

        // Instantiate a ParameterBlock for the config.txt file, and pass it to LoadConfigFile.
        ParameterBlock p_block{};
        p_block.OpenFile("c\\debug\\config.txt", 0, -1, nullptr, -1);

        logging::log("[CarsGame::SetConfigArguments] Loading configuration data from C\\Debug\\config.txt...");

        CarsGame_LoadConfigFile(this_ptr, &p_block);
    }
};

void debug_txt_support::install() {
    if (config::g_DebugTxtConfigEnabled) {
        CarsGameSetConfigArguments::install_at_ptr(0x004416b0);
        logging::log("[debug_txt_support::install] Successfully installed patch!");
    }
}