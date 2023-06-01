#pragma once

#include <fstream>
#include <iostream>
#include <optional>
#include <string>

#include "ConfigManager.h"
#include "Globals.h"
#include "Logging.h"

bool config::g_ConsoleWindowEnabled = true;
bool config::g_DebugTxtConfigEnabled = false;
bool config::g_WidescreenEnabled = false;

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
            logging::log("[config::read_int] Failed to parse string: {}. Error: {}", str, error.what());
            return std::nullopt;
        } catch (std::out_of_range &error) {
            logging::log("[config::read_int] Failed to parse string: {}. Error: {}", str, error.what());
            return std::nullopt;
        }
        return std::stoi(stripped);
    }
    return std::nullopt;
}

void config::read() {
    std::ifstream conf_file(g_DataDir / "config.ini", std::ios::in);
    std::string line;
    while (std::getline(conf_file, line)) {
        if (line.find("EnableConsoleWindow") != std::string::npos) {
            g_ConsoleWindowEnabled = read_bool(line).value_or(true);
        }
        if (line.find("EnableWidescreen") != std::string::npos) {
            g_WidescreenEnabled = read_bool(line).value_or(false);
        }
        if (line.find("EnableDebugTxtConfig") != std::string::npos) {
            g_DebugTxtConfigEnabled = read_bool(line).value_or(false);
        }
    }
    conf_file.close();
}
