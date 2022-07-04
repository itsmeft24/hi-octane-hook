#pragma once

namespace ConfigManager {

	extern bool IsWidescreenEnabled;
	extern unsigned int DesiredWindowWidth;
	extern unsigned int DesiredWindowHeight;
	
	void ReadConfigFile();
	
};