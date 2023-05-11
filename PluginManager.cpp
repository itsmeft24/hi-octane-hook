#pragma once
#include <Windows.h>
#include <filesystem>
#include <iostream>
#include <vector>

#include "Globals.h"
#include "Logging.h"
#include "PluginManager.h"
#include "Utils.h"

void PluginManager::Plugin::start() {
  auto pEntryPoint = GetProcAddress(handle, "Plugin_HiOctaneEntry");
  if (pEntryPoint != nullptr) {
    auto EntryPoint = (void(__stdcall *)())(pEntryPoint);
    EntryPoint();
  }
}

void PluginManager::Plugin::exit() {
  auto pExitPoint = GetProcAddress(handle, "Plugin_HiOctaneExit");
  if (pExitPoint != nullptr) {
    auto ExitPoint = (void(__stdcall *)())(pExitPoint);
    ExitPoint();
  }
}

std::vector<PluginManager::Plugin> loaded_plugins;

void PluginManager::load_plugins() {

  std::filesystem::path c_modules = g_DataDir / "C\\Modules";

  for (const auto &entry : std::filesystem::directory_iterator(c_modules)) {
    if (entry.is_regular_file()) {

      auto base_name = entry.path().filename().string();
      make_lowercase(base_name);

      if (base_name == "cars-hi-octane.dll")
        continue;

      if (entry.path().extension().string() == ".dll") {
        loaded_plugins.push_back(
            Plugin{base_name, LoadLibraryA(entry.path().string().c_str())});
        Logging::log("[PluginManager::LoadAllPlugins] Loading plugin: {}...",
                     base_name);
      }
    }
  }
}

void PluginManager::start_plugins() {

  for (auto &plugin : loaded_plugins) {
    Logging::log("[PluginManager::StartAllPlugins] Running HiOctaneEntry for "
                 "plugin: {}...",
                 plugin.name.c_str());
    plugin.start();
    Logging::log("[PluginManager::StartAllPlugins] Finished running "
                 "HiOctaneEntry for plugin: {}.",
                 plugin.name);
  }
}

void PluginManager::exit_plugins() {

  for (auto &plugin : loaded_plugins) {
    Logging::log("[PluginManager::ExitAllPlugins] Running HiOctaneExit for "
                 "plugin: {}...",
                 plugin.name);
    plugin.exit();
    Logging::log("[PluginManager::ExitAllPlugins] Finished running "
                 "HiOctaneExit for plugin: {}.",
                 plugin.name);
    FreeLibrary(plugin.handle);
  }

  loaded_plugins.clear();
}
