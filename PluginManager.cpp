#pragma once
#include <Windows.h>
#include <filesystem>
#include <iostream>
#include <vector>

#include "Globals.h"
#include "Logging.h"
#include "PluginManager.h"
#include "utils.hpp"

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

  for (const auto &entry : std::filesystem::directory_iterator(c_modules)) {
    if (entry.is_regular_file()) {

      auto base_name = entry.path().filename().string();
      utils::make_lowercase(base_name);

      if (base_name == "cars-hi-octane.dll")
        continue;

      if (entry.path().extension().string() == ".dll") {
        loaded_plugins.push_back(
            Plugin{base_name, LoadLibraryA(entry.path().string().c_str())});
        logging::log("[PluginManager::LoadAllPlugins] Loading plugin: {}...",
                     base_name);
      }
    }
  }
}

void plugin_manager::start_plugins() {

  for (auto &plugin : loaded_plugins) {
    logging::log("[PluginManager::StartAllPlugins] Running HiOctaneEntry for "
                 "plugin: {}...",
                 plugin.name.c_str());
    plugin.start();
    logging::log("[PluginManager::StartAllPlugins] Finished running "
                 "HiOctaneEntry for plugin: {}.",
                 plugin.name);
  }
}

void plugin_manager::exit_plugins() {

  for (auto &plugin : loaded_plugins) {
    logging::log("[PluginManager::ExitAllPlugins] Running HiOctaneExit for "
                 "plugin: {}...",
                 plugin.name);
    plugin.exit();
    logging::log("[PluginManager::ExitAllPlugins] Finished running "
                 "HiOctaneExit for plugin: {}.",
                 plugin.name);
    FreeLibrary(plugin.handle);
  }

  loaded_plugins.clear();
}
