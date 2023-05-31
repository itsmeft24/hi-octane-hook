#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../ConfigManager.h"
#include "../FileSystem.h"
#include "../Logging.h"
#include "../framework.hpp"
#include "../utils.hpp"

#include "LargeVehiclePatch.h"

DeclareFunction(void, __thiscall, PlayCameraAnimByIndex, 0x004bd1f0, uintptr_t,
    int);

std::vector<std::string> large_vehicles_vec = { "mcqm", "matm", "sulm" };

int LastSetCameraAnimIndex = 0;

bool LV_CollectCharactersToPatch() {
    std::ifstream file(
        fs::resolve_path("c\\global\\chars\\largevehicles.ini"),
        std::ios::in);
    if (!file)
        return false;

    std::string line;
    while (std::getline(file, line)) {
        utils::make_lowercase(line);
        large_vehicles_vec.push_back(line);
    }
    file.close();
    return true;
}

BOOL __stdcall GetCameraAnimIndex(char* character) {
    auto string = std::string(character);
    utils::make_lowercase(string);
    auto is_large_vehicle =
        std::find(large_vehicles_vec.begin(), large_vehicles_vec.end(), string) !=
        large_vehicles_vec.end();

    if (!is_large_vehicle && !ConfigManager::IsWidescreenEnabled) {
        return 0;
    }
    else if (!is_large_vehicle && ConfigManager::IsWidescreenEnabled) {
        return 2;
    }
    else if (is_large_vehicle && !ConfigManager::IsWidescreenEnabled) {
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
        call PlayCameraAnimByIndex
        return_part_ :
        pop edi
            pop esi
            ret 0x8
    }
}

void large_vehicles::install() {
    LV_CollectCharactersToPatch();
    hooking::write_nop(0x0050FB1F, 0x3B);
    hooking::write_jmp(0x0050FB1F, HandleCharacter);
    logging::log("[LargeVehiclePatch::Install] Successfully installed patch!");
};
