#pragma once
#include <Windows.h>

namespace PluginManager {

	struct Plugin {
		std::string Name;
		HMODULE ModuleHandle;

		void ExecuteEntryPoint();
		
		void ExecuteExitPoint();
	};
	
    void LoadAllPlugins();

    void StartAllPlugins();

    void ExitAllPlugins();

};