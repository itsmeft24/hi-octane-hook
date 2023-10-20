#pragma once
#include <fstream>

namespace config {

	extern bool g_ConsoleWindowEnabled;
	extern bool g_DebugTxtConfigEnabled;
	extern bool g_WidescreenEnabled;
	extern bool g_AutomaticUpdatesEnabled;

	void read();

};