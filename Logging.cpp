#pragma once
#include <Windows.h>
#include <cstdarg>
#include <cstdio>

#include "Globals.h"
#include "Logging.h"

FILE *log_file = nullptr;
FILE *console_handle = nullptr;
bool log_file_initialized = false;

bool Logging::Init() {
  if (ConfigManager::EnableConsoleWindow) {
    AllocConsole();
    freopen_s(&console_handle, "CONOUT$", "w", stdout);
    SetConsoleTitleA("Cars: Hi-Octane Console");
  }
  std::string logfilepath = CURRENT_DIRECTORY + "\\hi-octane.log";
  log_file = fopen(logfilepath.c_str(), "w");
  if (!log_file) {
    printf("[Logging::Init] Failed to open log file: hi-octane.log\n");
  }
  log_file_initialized = log_file != nullptr;
  return log_file_initialized && console_handle != nullptr;
}
void Logging::Log(const char *format, ...) {
  if (ConfigManager::EnableConsoleWindow) {
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

void Logging::Deinit() {
  if (log_file_initialized) {
    fclose(log_file);
  }
  if (ConfigManager::EnableConsoleWindow) {
    fclose(console_handle);
  }
}

HIOCTANE_API void _API_Logging_Log(const char *format, ...) {
  if (ConfigManager::EnableConsoleWindow) {
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