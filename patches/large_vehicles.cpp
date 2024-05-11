#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "core/globals.hpp"
#include "core/config.hpp"
#include "core/fs.hpp"
#include "core/logging.hpp"
#include "core/hooking/framework.hpp"
#include "core/utils.hpp"

#include "large_vehicles.hpp"

DeclareFunction(void, __thiscall, CarsActivityUI_SetView, 0x004bd1f0, uintptr_t, int);

int LastSetCameraAnimIndex = 0;

BOOL __stdcall GetCameraAnimIndex(char* character) {
    auto string = std::string(character);
    utils::make_lowercase(string);
    auto is_large_vehicle = std::find(config::g_LargeVehicles.begin(), config::g_LargeVehicles.end(), string) != config::g_LargeVehicles.end();

    if (!is_large_vehicle && !config::g_WidescreenEnabled) {
        return 0;
    }
    else if (!is_large_vehicle && config::g_WidescreenEnabled) {
        return 2;
    }
    else if (is_large_vehicle && !config::g_WidescreenEnabled) {
        return 1;
    }
    else { // is_large_vehicle && ConfigManager::IsWidescreenEnabled
        return 3;
    }
}

// Called every time the game is about to switch camera animations.
void __declspec(naked) HandleCharacter() {
    __asm {
        call GetCameraAnimIndex
        cmp LastSetCameraAnimIndex, eax
        mov LastSetCameraAnimIndex, eax
        // Return early if the index didn't change. Prevents animations from
        // repeating.
        je return_part_
        mov ecx, dword ptr[esi + 0x140]
        push eax
        call CarsActivityUI_SetView
        return_part_ :
        pop edi
            pop esi
            ret 0x8
    }
}

DefineInlineHook(CSSEndLayer) {
    static void __cdecl callback(hooking::InlineCtx & ctx) {
        LastSetCameraAnimIndex = 0;
        if (config::g_WidescreenEnabled) {
            LastSetCameraAnimIndex = 2;
        }
    }
};

void large_vehicles::install() {
    hooking::write_nop(0x0050FB1F, 0x3B);
    hooking::write_jmp(0x0050FB1F, HandleCharacter);
    CSSEndLayer::install_at_ptr(0x004fe65a);
    logging::log("[large_vehicles::install] Successfully installed patch!");
};
