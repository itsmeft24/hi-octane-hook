#pragma once
#include <string>
#include <Windows.h>
#include <unordered_map>
#include <filesystem>

struct Plugin {
	std::string Name;
	HMODULE ModuleHandle;

    void ExecuteEntryPoint() {
        auto EntryPoint = (void(__stdcall*)())(GetProcAddress(ModuleHandle, "HiOctaneEntry"));
        EntryPoint();
    }

    void ExecuteExitPoint() {
        auto EntryPoint = (void(__stdcall*)())(GetProcAddress(ModuleHandle, "HiOctaneExit"));
        EntryPoint();
    }
};

namespace PluginManager {

	std::vector<Plugin> loaded_plugins;

    void LoadPluginForMod(std::string mod) {

        std::string libpath = CURRENT_DIRECTORY + '\\' + mod + "\\Plugin.dll";
        loaded_plugins.push_back(Plugin{ mod, LoadLibraryA(libpath.c_str()) });
        Logging::Log("[PluginManager::LoadPluginForMod] Loading plugin DLL from mod: %s...", mod.c_str());
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