#pragma once
#include <fstream>

namespace ConfigManager {

extern bool g_ConsoleWindowEnabled;
extern bool g_DebugTxtConfigEnabled;
extern bool g_WidescreenEnabled;

void read();

}; // namespace ConfigManager