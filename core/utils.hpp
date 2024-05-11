#pragma once
#include <Windows.h>
#include <algorithm>
#include <iostream>

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
    template <typename T> inline void wrap_constructor(std::uintptr_t ptr) {
        new (ptr) T();
    }
};
