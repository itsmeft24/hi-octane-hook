#pragma once
#include <Windows.h>
#include <algorithm>
#include <iostream>
#include <utility>

namespace utils {
    const inline void make_lowercase(std::string& str) {
        std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
    }
    const inline void make_uppercase(std::string& str) {
        std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::toupper(c); });
    }
    inline size_t curl_string_write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
        reinterpret_cast<std::string*>(userp)->append(reinterpret_cast<char*>(contents), size * nmemb);
        return size * nmemb;
    }
    inline size_t curl_vec_write_callback(void* contents, size_t _size, size_t nmemb, void* userp) {
        size_t size = _size * nmemb;

        std::vector<uint8_t>& vec = *reinterpret_cast<std::vector<uint8_t>*>(userp);

        size_t old_size = vec.size();

        vec.resize(vec.size() + size);

        memcpy(&vec[old_size], contents, size);

        return size;
    }
    inline std::pair<int, int> resolve_game_window_dimensions(int screen_width, int screen_height, bool widescreen) {
        int window_width = screen_width;
        int window_height = screen_height;

        double screen_ratio = screen_width / screen_height;
        if (widescreen) {
            if (screen_ratio > 16.0 / 9.0) {
                window_width = static_cast<int>(screen_height * 16.0 / 9.0);
            }
            else {
                window_height = static_cast<int>(screen_width * 9.0 / 16.0);
            }
        }
        else {
            if (screen_ratio > 4.0 / 3.0) {
                window_width = static_cast<int>(screen_height * 4.0 / 3.0);
            }
            else {
                window_height = static_cast<int>(screen_width * 3.0 / 4.0);
            }
        }

        return std::make_pair(window_width, window_height);
    }
};
