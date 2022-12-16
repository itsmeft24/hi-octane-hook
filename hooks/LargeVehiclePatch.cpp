#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../FileSystem.h"
#include "../HookFunction.h"
#include "../Logging.h"
#include "../Utils.h"

#include "LargeVehiclePatch.h"

std::vector<std::string> large_vehicles;

bool LV_CollectCharactersToPatch() {
  std::ifstream file(
      FileSystem::GetPathForFile("c\\global\\chars\\largevehicles.ini"),
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

BOOL __stdcall IsNotLargeVehicle(char *character) {
  auto string = std::string(character);
  make_lowercase(string);
  auto ret = std::find(large_vehicles.begin(), large_vehicles.end(), string) ==
             large_vehicles.end();
  return ret;
}

void LargeVehiclePatch::Install() {
  if (LV_CollectCharactersToPatch()) {
      HookedFunctionInfo info = HookFunction(0x0050FB1F, &IsNotLargeVehicle, 0x3B,
                                      FunctionHookType::InlineReplacement);
      if (info.type != FunctionHookType::Invalid) {
          Logging::Log(
              "[LargeVehiclePatch::Install] Successfully installed patch!\n");
      }
}
};
