#pragma once
#include <Windows.h>
#include <cstdarg>
#include <cstdio>

#include "core/globals.hpp"
#include "core/logging.hpp"
#include "core/config.hpp"

FILE *log_file = nullptr;
FILE *console_handle = nullptr;

void logging::setup_log_file() {
    std::filesystem::path logfilepath = g_InstallDir / "hi-octane.log";
    log_file = fopen(logfilepath.string().c_str(), "w");
}

void logging::create_console_window() {
    AllocConsole();
    freopen_s(&console_handle, "CONOUT$", "w", stdout);
    SetConsoleTitleA("Cars: Hi-Octane Console");
}

void logging::cleanup() {
    if (log_file != nullptr) {
        fclose(log_file);
    }
    if (console_handle != nullptr) {
        fclose(console_handle);
    }
}

HIOCTANE_API void _API_Logging_Log(const char *format, ...) {
    if (console_handle != nullptr) {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
    if (log_file != nullptr) {
        va_list args;
        va_start(args, format);
        vfprintf(log_file, format, args);
        va_end(args);
        fflush(log_file);
    }
}