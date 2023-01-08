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
unsigned int ConfigManager::DesiredWindowWidth = 1920;
unsigned int ConfigManager::DesiredWindowHeight = 1080;

std::optional<bool> ReadBool(const std::string &str) {
  size_t equal = str.find('=');
  if (equal != std::string::npos && equal != str.size() - 1) {
    std::string stripped = str.substr(equal + 1);
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
  if (equal != std::string::npos && equal != str.size() - 1) {
    std::string stripped = str.substr(equal + 1);
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
    if (line.find("EnableWidescreen") != std::string::npos) {
      IsWidescreenEnabled = ReadBool(line).value_or(false);
    }
    if (line.find("EnableDebugTxtConfig") != std::string::npos) {
      EnableDebugTxtConfig = ReadBool(line).value_or(false);
    }
    if (line.find("DesiredWindowWidth") != std::string::npos) {
      DesiredWindowWidth = ReadInt(line).value_or(1920);
    }
    if (line.find("DesiredWindowHeight") != std::string::npos) {
      DesiredWindowHeight = ReadInt(line).value_or(1080);
    }
  }
  conf_file.close();
}
