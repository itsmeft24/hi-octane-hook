#pragma once
#include <Windows.h>
#include <cstdarg>
#include <cstdio>

#include "Globals.h"
#include "Logging.h"
#include "ConfigManager.h"

FILE *log_file = nullptr;
FILE *console_handle = nullptr;
bool log_file_initialized = false;

bool logging::init() {
  if (config::g_ConsoleWindowEnabled) {
    AllocConsole();
    freopen_s(&console_handle, "CONOUT$", "w", stdout);
    SetConsoleTitleA("Cars: Hi-Octane Console");
  }
  auto logfilepath = g_InstallDir / "hi-octane.log";
  log_file = fopen(logfilepath.string().c_str(), "w");
  if (!log_file) {
    printf("[Logging::Init] Failed to open log file: hi-octane.log\n");
  }
  log_file_initialized = log_file != nullptr;
  return log_file_initialized && console_handle != nullptr;
}

void logging::deinit() {
  if (log_file_initialized) {
    fclose(log_file);
  }
  if (config::g_ConsoleWindowEnabled) {
    fclose(console_handle);
  }
}

HIOCTANE_API void _API_Logging_Log(const char *format, ...) {
  if (config::g_ConsoleWindowEnabled) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
  }
  if (log_file_initialized) {
    va_list args;
    va_start(args, format);
    vfprintf(log_file, format, args);
    va_end(args);
    fflush(log_file);
  }
}