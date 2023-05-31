#pragma once
#include "Globals.h"
#include <format>

namespace logging {

bool init();

HIOCTANE_API void _API_Logging_Log(const char* format, ...);

template<typename... Args>
auto log(std::format_string<Args...> fmt, Args&&... args) {
	auto str = std::vformat(fmt.get(), std::make_format_args(args...)) + '\n';
	_API_Logging_Log(str.c_str());
}

void deinit();
}; // namespace Logging