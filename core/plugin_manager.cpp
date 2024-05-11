#pragma once
#include <Windows.h>
#include <filesystem>
#include <iostream>
#include <vector>

#include "core/globals.hpp"
#include "core/logging.hpp"
#include "core/plugin_manager.hpp"
#include "core/utils.hpp"

void plugin_manager::Plugin::start() {
    auto pEntryPoint = GetProcAddress(handle, "Plugin_HiOctaneEntry");
    if (pEntryPoint != nullptr) {
        auto EntryPoint = (void(__stdcall *)())(pEntryPoint);
        EntryPoint();
    }
}

void plugin_manager::Plugin::exit() {
    auto pExitPoint = GetProcAddress(handle, "Plugin_HiOctaneExit");
    if (pExitPoint != nullptr) {
        auto ExitPoint = (void(__stdcall *)())(pExitPoint);
        ExitPoint();
    }
}

std::vector<plugin_manager::Plugin> loaded_plugins;

void plugin_manager::load_plugins() {

    std::filesystem::path c_modules = g_DataDir / "C\\Modules";
    if (std::filesystem::is_directory(c_modules)) {
        for (const auto& entry : std::filesystem::directory_iterator(c_modules)) {
            if (entry.is_regular_file()) {
                std::string base_name = entry.path().filename().string();
                utils::make_lowercase(base_name);

                if (base_name == "cars-hi-octane.dll") {
                    continue;
                }

                if (entry.path().extension().string() == ".dll") {
                    loaded_plugins.emplace_back(base_name, LoadLibraryA(entry.path().string().c_str()));
                    logging::log("[plugin_manager::load_plugins] Loading plugin: {}...", base_name);
                }
            }
        }
    }
}

void plugin_manager::start_plugins() {
    for (auto &plugin : loaded_plugins) {
        logging::log("[plugin_manager::start_plugins] Running HiOctaneEntry for plugin: {}...", plugin.name.c_str());
        plugin.start();
        logging::log("[plugin_manager::start_plugins] Finished running HiOctaneEntry for plugin: {}.", plugin.name);
    }
}

void plugin_manager::exit_plugins() {
    for (auto &plugin : loaded_plugins) {
        logging::log("[plugin_manager::exit_plugins] Running HiOctaneExit for plugin: {}...", plugin.name);
        plugin.exit();
        logging::log("[plugin_manager::exit_plugins] Finished running HiOctaneExit for plugin: {}.", plugin.name);
        FreeLibrary(plugin.handle);
    }

    loaded_plugins.clear();
}
