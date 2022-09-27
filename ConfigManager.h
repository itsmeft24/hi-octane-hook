#pragma once

namespace ConfigManager {

extern bool EnableDebugTxtConfig;
extern bool IsWidescreenEnabled;
extern unsigned int DesiredWindowWidth;
extern unsigned int DesiredWindowHeight;

void ReadConfigFile();

}; // namespace ConfigManager