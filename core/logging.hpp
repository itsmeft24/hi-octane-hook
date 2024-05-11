#pragma once
#include "core/globals.hpp"
#include <format>

namespace logging {

	void setup_log_file();
	
	void create_console_window();

	HIOCTANE_API void _API_Logging_Log(const char* format, ...);

	template<typename... Args>
	auto log(std::format_string<Args...> fmt, Args&&... args) {
		auto str = std::vformat(fmt.get(), std::make_format_args(args...)) + '\n';
		_API_Logging_Log(str.c_str());
	}

	void cleanup();
};