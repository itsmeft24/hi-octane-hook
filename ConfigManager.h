#pragma once
#include <fstream>

//#include <filesystem>
namespace ConfigManager {

extern bool EnableConsoleWindow;
extern bool EnableDebugTxtConfig;
extern bool IsWidescreenEnabled;
extern unsigned int DesiredWindowWidth;
extern unsigned int DesiredWindowHeight;

void read();

}; // namespace ConfigManager