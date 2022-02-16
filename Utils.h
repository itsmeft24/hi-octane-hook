#pragma once
#include <iostream>
#include <algorithm>

const inline void make_lowercase(std::string& str) {
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
}
const inline void make_uppercase(std::string& str) {
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::toupper(c); });
}