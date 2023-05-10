#pragma once
#include <windows.h>
#include <fstream>
#include <shlobj.h>
#undef GetObject

//#include <filesystem>
namespace ConfigManager {

extern bool EnableConsoleWindow;
extern bool EnableDebugTxtConfig;
extern bool IsWidescreenEnabled;
extern unsigned int DesiredWindowWidth;
extern unsigned int DesiredWindowHeight;

void ReadConfigFile();

inline std::wstring GetSaveDir(){
	wchar_t documents[1024];
	SHGetFolderPathW(0, CSIDL_MYDOCUMENTS, 0, 0, documents);
	return std::wstring(documents) + L"\\THQ\\Cars2\\";
}

}; // namespace ConfigManager