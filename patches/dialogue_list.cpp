
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "core/globals.hpp"
#include "core/fs.hpp"
#include "core/config.hpp"
#include "core/hooking/framework.hpp"
#include "core/logging.hpp"

#include "dialogue_list.hpp"

struct CarsActivity;
struct CarsActivityUI;

DeclareFunction(void, __thiscall, CarsActivity_AddNameToDialogueList, 0x004B68B0, CarsActivity*, const char *);

DefineReplacementHook(CarsActivityUIRequestDialogue) {
    static void __fastcall callback(CarsActivityUI* _this) {
        for (size_t x = 0; x < config::g_DialogueList.size(); x++) {
            CarsActivity_AddNameToDialogueList(reinterpret_cast<CarsActivity*>(_this), config::g_DialogueList[x].c_str());
            logging::log("[CarsActivityUI::RequestDialogue] Added {} to the Dialogue List!", config::g_DialogueList[x]);
        }
    }
};

void dialogue_list::install() {
    CarsActivityUIRequestDialogue::install_at_ptr(0x004BD360);
    logging::log("[dialogue_list::install] Successfully installed patch!");
};
