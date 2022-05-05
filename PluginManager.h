#pragma once
#include <string>
#include <Windows.h>
#include <unordered_map>
#include <filesystem>

namespace PluginManager {

	struct Plugin {
		std::string Name;
		HMODULE ModuleHandle;

		void ExecuteEntryPoint() {
			auto pEntryPoint = GetProcAddress(ModuleHandle, "Plugin_HiOctaneEntry");
			if (pEntryPoint != nullptr) {
				auto EntryPoint = (void(__stdcall*)())(pEntryPoint);
				EntryPoint();
			}
		}

		void ExecuteExitPoint() {
			auto pExitPoint = GetProcAddress(ModuleHandle, "Plugin_HiOctaneExit");
			if (pExitPoint != nullptr) {
				auto ExitPoint = (void(__stdcall*)())(pExitPoint);
				ExitPoint();
			}
		}
	};

	std::vector<Plugin> loaded_plugins;

    void LoadAllPlugins() {

        std::filesystem::path c_modules = DATA_DIR_PATH + "\\C\\Modules";
		
		for (const auto& entry : std::filesystem::directory_iterator(c_modules)) {
			if (entry.is_regular_file()) {
				
				auto base_name = entry.path().filename().string();
				make_lowercase(base_name);
				
				if (base_name == "cars-hi-octane.dll")
					continue;

				if (entry.path().extension().string() == ".dll") {
					loaded_plugins.push_back(Plugin{ base_name, LoadLibraryA(entry.path().string().c_str()) });
					Logging::Log("[PluginManager::LoadAllPlugins] Loading plugin: %s...", base_name.c_str());
				}
			}
		}
	}

    void StartAllPlugins() {
    
        for (auto& plugin : loaded_plugins) {
            Logging::Log("[PluginManager::StartAllPlugins] Running HiOctaneEntry for plugin: %s...", plugin.Name.c_str());
            plugin.ExecuteEntryPoint();
            Logging::Log("[PluginManager::StartAllPlugins] Finished running HiOctaneEntry for plugin: %s.", plugin.Name.c_str());
        }
    }

    void ExitAllPlugins() {

        for (auto& plugin : loaded_plugins) {
            Logging::Log("[PluginManager::ExitAllPlugins] Running HiOctaneExit for plugin: %s...", plugin.Name.c_str());
            plugin.ExecuteExitPoint();
            Logging::Log("[PluginManager::ExitAllPlugins] Finished running HiOctaneExit for plugin: %s.", plugin.Name.c_str());
            FreeLibrary(plugin.ModuleHandle);
        }

        loaded_plugins.clear();
    }

};