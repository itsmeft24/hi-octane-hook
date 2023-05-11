#pragma once
#include <Windows.h>

namespace PluginManager {

struct Plugin {
  std::string name;
  HMODULE handle;

  void start();

  void exit();
};

void load_plugins();

void start_plugins();

void exit_plugins();

}; // namespace PluginManager