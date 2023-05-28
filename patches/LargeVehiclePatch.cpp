#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../ConfigManager.h"
#include "../FileSystem.h"
#include "../HookFunction.h"
#include "../Logging.h"
#include "../Utils.h"

#include "LargeVehiclePatch.h"

DeclareFunction(void, __thiscall, PlayCameraAnimByIndex, 0x004bd1f0, uintptr_t,
    int);

std::vector<std::string> large_vehicles = { "mcqm", "matm", "sulm" };

int LastSetCameraAnimIndex = 0;

bool LV_CollectCharactersToPatch() {
    std::ifstream file(
        FileSystem::resolve_path("c\\global\\chars\\largevehicles.ini"),
        std::ios::in);
    if (!file)
        return false;

    std::string line;
    while (std::getline(file, line)) {
        make_lowercase(line);
        large_vehicles.push_back(line);
    }
    file.close();
    return true;
}

BOOL __stdcall GetCameraAnimIndex(char* character) {
    auto string = std::string(character);
    make_lowercase(string);
    auto is_large_vehicle =
        std::find(large_vehicles.begin(), large_vehicles.end(), string) !=
        large_vehicles.end();

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

void LargeVehiclePatch::install() {
    LV_CollectCharactersToPatch();
    HookedFunctionInfo info =
        HookFunction(0x0050FB1F, &HandleCharacter, 0x3B,
            FunctionHookType::InlineReplacementJMP);
    if (info.type != FunctionHookType::Invalid) {
        Logging::log(
            "[LargeVehiclePatch::Install] Successfully installed patch!");
    }
};
