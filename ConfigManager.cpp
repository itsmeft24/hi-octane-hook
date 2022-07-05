#pragma once

#include <fstream>
#include <iostream>
#include <string>

#include "ConfigManager.h"
#include "Globals.h"

bool ConfigManager::IsWidescreenEnabled = false;
unsigned int ConfigManager::DesiredWindowWidth = 0;
unsigned int ConfigManager::DesiredWindowHeight = 0;

void ConfigManager::ReadConfigFile() {

  std::ifstream conf_file(DATA_DIR_PATH + "\\config.ini", std::ios::in);
  std::string line;
  while (std::getline(conf_file, line)) {
    if (line.find("EnableWidescreen") != -1) {
      IsWidescreenEnabled = line[17] == 'T' || line[17] == 't' ||
                            line[17] == 'Y' || line[17] == 'y' ||
                            line[17] == '1';
    }
    if (line.find("DesiredWindowWidth") != -1) {
      DesiredWindowWidth = std::stoi(line.substr(19));
    }
    if (line.find("DesiredWindowHeight") != -1) {
      DesiredWindowHeight = std::stoi(line.substr(20));
    }
  }
  conf_file.close();
}
