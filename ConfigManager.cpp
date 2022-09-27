#pragma once

#include <fstream>
#include <iostream>
#include <optional>
#include <string>

#include "ConfigManager.h"
#include "Globals.h"
#include "Logging.h"

bool ConfigManager::EnableDebugTxtConfig = false;
bool ConfigManager::IsWidescreenEnabled = false;
unsigned int ConfigManager::DesiredWindowWidth = 0;
unsigned int ConfigManager::DesiredWindowHeight = 0;

std::optional<bool> ReadBool(const std::string &str) {
  size_t equal = str.find('=');
  if (equal != -1 && equal != str.size() - 1) {
    std::string stripped = str.substr(equal);
    stripped.erase(
        std::remove_if(stripped.begin(), stripped.end(), std::isspace),
        stripped.end());
    int bool_char = std::tolower(stripped[0]);
    return bool_char == 't' || bool_char == 'y' || bool_char == '1';
  }
  return std::nullopt;
}

std::optional<int> ReadInt(const std::string &str) {
  size_t equal = str.find('=');
  if (equal != -1 && equal != str.size() - 1) {
    std::string stripped = str.substr(equal);
    stripped.erase(
        std::remove_if(stripped.begin(), stripped.end(), std::isspace),
        stripped.end());
    try {
      int num = std::stoi(stripped);
    } catch (std::invalid_argument &error) {
      Logging::Log(
          "[ConfigManager::ReadInt] Failed to parse string: %s. Error: %s",
          str.c_str(), error.what());
      return std::nullopt;
    } catch (std::out_of_range &error) {
      Logging::Log(
          "[ConfigManager::ReadInt] Failed to parse string: %s. Error: %s",
          str.c_str(), error.what());
      return std::nullopt;
    }
    return std::stoi(stripped);
  }
  return std::nullopt;
}

void ConfigManager::ReadConfigFile() {

  std::ifstream conf_file(DATA_DIR_PATH + "\\config.ini", std::ios::in);
  std::string line;
  while (std::getline(conf_file, line)) {
    if (line.find("EnableWidescreen") != -1) {
      IsWidescreenEnabled = ReadBool(line).value_or(false);
    }
    if (line.find("EnableDebugTxtConfig") != -1) {
      EnableDebugTxtConfig = ReadBool(line).value_or(false);
    }
    if (line.find("DesiredWindowWidth") != -1) {
      DesiredWindowWidth = ReadInt(line).value_or(0);
    }
    if (line.find("DesiredWindowHeight") != -1) {
      DesiredWindowHeight = ReadInt(line).value_or(0);
    }
  }
  conf_file.close();
}
