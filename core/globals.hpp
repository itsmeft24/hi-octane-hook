#pragma once

#include <iostream>
#include <filesystem>

extern std::filesystem::path g_InstallDir;

extern std::filesystem::path g_DataDir;

extern "C" __declspec(dllexport) const char* VERSION;

#define DeclareFunction(ret_type, call_conv, name, offset, ...) auto name = (ret_type(call_conv *)(##__VA_ARGS__))(offset)

#define HIOCTANE_API extern "C" __declspec(dllexport)

#undef GetObject