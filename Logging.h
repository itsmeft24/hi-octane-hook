#pragma once
#include <cstdio>
#include <cstdarg>

namespace Logging {
	FILE* logfile;
	bool is_init;

	bool Init() {
		auto logfilepath = CURRENT_DIRECTORY + "\\hi-octane.log";
		logfile = fopen(logfilepath.c_str(), "w");
		if (!logfile)
			printf("[Logging::Init] Failed to open log file: hi-octane.log\n");
		is_init = !(!logfile);
		return is_init;
	}
	void Log(const char* format, ...) {
		va_list args;
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
		if (is_init) {
			va_list args;
			va_start(args, format);
			vfprintf(logfile, format, args);
			va_end(args);
		}
	}
	void Deinit() {
		fclose(logfile);
	}
}