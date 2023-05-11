#pragma once

#include <fstream>
#include <iostream>
#include <optional>
#include <string>

#include "ConfigManager.h"
#include "Globals.h"
#include "Logging.h"

bool ConfigManager::EnableConsoleWindow = true;
bool ConfigManager::EnableDebugTxtConfig = false;
bool ConfigManager::IsWidescreenEnabled = false;
unsigned int ConfigManager::DesiredWindowWidth = 1920;
unsigned int ConfigManager::DesiredWindowHeight = 1080;

std::optional<bool> read_bool(const std::string &str) {
  size_t equal = str.find('=');
  if (equal != std::string::npos && equal != str.size() - 1) {
    std::string stripped = str.substr(equal + 1);
    std::erase(stripped, ' ');
    int bool_char = std::tolower(stripped[0]);
    return bool_char == 't' || bool_char == 'y' || bool_char == '1';
  }
  return std::nullopt;
}

std::optional<int> read_int(const std::string &str) {
  size_t equal = str.find('=');
  if (equal != std::string::npos && equal != str.size() - 1) {
    std::string stripped = str.substr(equal + 1);
    std::erase(stripped, ' ');
    try {
      int num = std::stoi(stripped);
    } catch (std::invalid_argument &error) {
      Logging::log(
          "[ConfigManager::ReadInt] Failed to parse string: {}. Error: {}",
          str, error.what());
      return std::nullopt;
    } catch (std::out_of_range &error) {
      Logging::log(
          "[ConfigManager::ReadInt] Failed to parse string: {}. Error: {}",
          str, error.what());
      return std::nullopt;
    }
    return std::stoi(stripped);
  }
  return std::nullopt;
}

void ConfigManager::read() {

  std::ifstream conf_file(g_DataDir / "config.ini", std::ios::in);
  std::string line;
  while (std::getline(conf_file, line)) {
    if (line.find("EnableConsoleWindow") != std::string::npos) {
      EnableConsoleWindow = read_bool(line).value_or(true);
    }
    if (line.find("EnableWidescreen") != std::string::npos) {
      IsWidescreenEnabled = read_bool(line).value_or(false);
    }
    if (line.find("EnableDebugTxtConfig") != std::string::npos) {
      EnableDebugTxtConfig = read_bool(line).value_or(false);
    }
    if (line.find("DesiredWindowWidth") != std::string::npos) {
      DesiredWindowWidth = read_int(line).value_or(1920);
    }
    if (line.find("DesiredWindowHeight") != std::string::npos) {
      DesiredWindowHeight = read_int(line).value_or(1080);
    }
  }
  conf_file.close();
}
