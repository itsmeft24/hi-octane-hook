#pragma once
#include <Windows.h>
#include <cstdarg>
#include <cstdio>

#include "Globals.h"
#include "Logging.h"

FILE *log_file;
FILE *console_handle;
bool is_logger_init;

bool Logging::Init() {
  AllocConsole();
  freopen_s(&console_handle, "CONOUT$", "w", stdout);
  SetConsoleTitleA("Cars: Hi-Octane Console");

  auto logfilepath = CURRENT_DIRECTORY + "\\hi-octane.log";
  log_file = fopen(logfilepath.c_str(), "w");
  if (!log_file)
    printf("[Logging::Init] Failed to open log file: hi-octane.log\n");
  is_logger_init = !(!log_file);
  return is_logger_init;
}
void Logging::Log(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
  if (is_logger_init) {
    va_list args;
    va_start(args, format);
    vfprintf(log_file, format, args);
    va_end(args);
    fflush(log_file);
  }
}
void Logging::Deinit() {
  if (is_logger_init) {
    fclose(log_file);
  }
}

HIOCTANE_API void _API_Logging_Log(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
  if (is_logger_init) {
    va_list args;
    va_start(args, format);
    vfprintf(log_file, format, args);
    va_end(args);
  }
}