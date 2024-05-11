#pragma once
#include <fstream>
#include <unordered_map>

namespace config {

	extern bool g_ConsoleWindowEnabled;
	extern bool g_DebugTxtConfigEnabled;
	extern bool g_WidescreenEnabled;
	extern bool g_AutomaticUpdatesEnabled;
	extern bool g_SaveRedirectionEnabled;
	extern bool g_ExploreMusicEnabled;
	extern bool g_DataAccessLoggingEnabled;
	extern char g_LangPrefix;
	extern std::unordered_map<std::string, std::string> g_RaceTypeToPlaylist;
	extern std::vector<std::string> g_DialogueList;
	extern std::vector<std::string> g_LargeVehicles;
	extern std::vector<std::string> g_IntroVideos;

	void read();

};