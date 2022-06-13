#pragma once
#include <cstdio>
#include <cstdarg>

namespace Logging {
	FILE* logfile;
	FILE* CONBUF;
	bool is_init;

	bool Init() {
		AllocConsole();
		freopen_s(&CONBUF, "CONOUT$", "w", stdout);
		SetConsoleTitleA(WindowTitle);

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
			fflush(logfile);
		}
	}
	void Deinit() {
		if (is_init)
			fclose(logfile);
	}
}

HIOCTANE_API void _API_Logging_Log(const char* format, ...) {
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
	if (Logging::is_init) {
		va_list args;
		va_start(args, format);
		vfprintf(Logging::logfile, format, args);
		va_end(args);
	}
}