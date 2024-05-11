#pragma once

#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <rapidjson/document.h>

#include "core/config.hpp"
#include "core/globals.hpp"
#include "core/logging.hpp"

bool config::g_ConsoleWindowEnabled = true;
bool config::g_DebugTxtConfigEnabled = false;
bool config::g_WidescreenEnabled = false;
bool config::g_AutomaticUpdatesEnabled = true;
bool config::g_SaveRedirectionEnabled = false;
bool config::g_ExploreMusicEnabled = false;
bool config::g_DataAccessLoggingEnabled = true;
char config::g_LangPrefix = 'E';
std::unordered_map<std::string, std::string> config::g_RaceTypeToPlaylist = {};
std::vector<std::string> config::g_DialogueList = {};
std::vector<std::string> config::g_LargeVehicles = {};
std::vector<std::string> config::g_IntroVideos = {};

std::unordered_map<std::string, char> kLanguageToPrefix = {
    {"English", 'E'},
    {"Spanish", 'S'},
    {"French", 'F'},
    {"Italian", 'I'},
    {"German", 'G'},
    {"Swedish", 'W'},
    {"Norwegian", 'N'},
    {"Danish", 'D'},
    {"Dutch", 'U'}
};

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

std::optional<std::string> read_string(const std::string& str) {
    size_t equal = str.find('=');
    if (equal != std::string::npos && equal != str.size() - 1) {
        std::string stripped = str.substr(equal + 1);
        return stripped;
    }
    return std::nullopt;
}

#define READ_BOOL_AND_LOG_ON_FAIL(name, var) \
if (root.HasMember(#name)) { \
    if (root[#name].IsBool()) { \
        var = root[#name].GetBool(); \
    } else { \
        logging::log("[config::read] Variable: "#name" in settings.json is not a boolean! Defaulting to: {}...", var); \
    } \
} else { \
    logging::log("[config::read] Variable: "#name" is not present in settings.json! Defaulting to: {}...", var); \
} \

void config::read() {
    std::string line;
    std::ifstream conf_file(g_InstallDir / "LocalStorage\\settings.json", std::ios::in);
    if (conf_file.is_open()) {
        conf_file.seekg(0, std::ios::end);
        unsigned int json_size = conf_file.tellg();

        char* json_buffer = new char[json_size + 1];
        json_buffer[json_size] = 0;
        conf_file.seekg(0, std::ios::beg);
        conf_file.read(json_buffer, json_size);
        conf_file.close();

        rapidjson::Document doc;
        doc.Parse<rapidjson::kParseStopWhenDoneFlag>(json_buffer);

        delete[] json_buffer;

        const auto& root = doc.GetObject();

        READ_BOOL_AND_LOG_ON_FAIL(enable_console_window, g_ConsoleWindowEnabled);
        READ_BOOL_AND_LOG_ON_FAIL(enable_widescreen, g_WidescreenEnabled);
        READ_BOOL_AND_LOG_ON_FAIL(enable_debug_config, g_DebugTxtConfigEnabled);
        READ_BOOL_AND_LOG_ON_FAIL(enable_automatic_updates, g_AutomaticUpdatesEnabled);
        READ_BOOL_AND_LOG_ON_FAIL(enable_save_redirection, g_SaveRedirectionEnabled);
        READ_BOOL_AND_LOG_ON_FAIL(enable_explore_music, g_ExploreMusicEnabled);
        READ_BOOL_AND_LOG_ON_FAIL(enable_data_access_logging, g_DataAccessLoggingEnabled);

        if (root.HasMember("force_windowed_mode")) {
            if (root["force_windowed_mode"].IsBool()) {
                if (root["force_windowed_mode"].GetBool()) {
                    *reinterpret_cast<std::uint32_t*>(0x0071b224) = 1;
                }
            }
            else {
                logging::log("[config::read] Variable: force_windowed_mode in settings.json is not a boolean! Defaulting to: false...");
            }
        }
        else {
            logging::log("[config::read] Variable: force_windowed_mode is not present in settings.json! Defaulting to: false...");
        }

        if (root.HasMember("race_type_playlist_map")) {
            if (root["race_type_playlist_map"].IsObject()) {
                const auto& dict = root["race_type_playlist_map"].GetObject();
                for (const auto& member : dict) {
                    g_RaceTypeToPlaylist.insert({ member.name.GetString(), member.value.GetString() });
                }
            }
            else {
                logging::log("[config::read] Variable: race_type_playlist_map in settings.json is not a dictionary!");
            }
        }
        else {
            logging::log("[config::read] Variable: race_type_playlist_map is not present in settings.json!");
        }

        if (root.HasMember("dialogue_list")) {
            if (root["dialogue_list"].IsArray()) {
                const auto& list = root["dialogue_list"].GetArray();
                for (const auto& character : list) {
                    g_DialogueList.push_back(character.GetString());
                }
            }
            else {
                logging::log("[config::read] Variable: dialogue_list in settings.json is not a list!");
            }
        }
        else {
            logging::log("[config::read] Variable: dialogue_list is not present in settings.json!");
        }

        if (root.HasMember("large_vehicles")) {
            if (root["large_vehicles"].IsArray()) {
                const auto& list = root["large_vehicles"].GetArray();
                for (const auto& character : list) {
                    g_LargeVehicles.push_back(character.GetString());
                }
            }
            else {
                logging::log("[config::read] Variable: large_vehicles in settings.json is not a list!");
            }
        }
        else {
            logging::log("[config::read] Variable: large_vehicles is not present in settings.json!");
        }

        if (root.HasMember("intro_videos")) {
            if (root["intro_videos"].IsArray()) {
                const auto& list = root["intro_videos"].GetArray();
                for (const auto& video : list) {
                    g_IntroVideos.push_back(video.GetString());
                }
            }
            else {
                logging::log("[config::read] Variable: intro_videos in settings.json is not a list!");
            }
        }
        else {
            logging::log("[config::read] Variable: intro_videos is not present in settings.json!");
        }
    }

    std::ifstream language_file(g_DataDir / "language.ini", std::ios::in);
    if (language_file.is_open()) {
        while (std::getline(language_file, line)) {
            if (line.find("Name") != std::string::npos) {
                std::string language_name = read_string(line).value_or("English");
                if (kLanguageToPrefix.contains(language_name)) {
                    g_LangPrefix = kLanguageToPrefix[language_name];
                }
            }
        }
    }
}
