#pragma once
#include <string_view>
#include <filesystem>

extern std::filesystem::path g_InstallDir;

extern std::filesystem::path g_DataDir;

constexpr std::string_view kDataDirName = "hi-octane";

extern "C" __declspec(dllexport) const char* VERSION;

#define DeclareFunction(ret_type, call_conv, name, offset, ...) auto name = (ret_type(call_conv *)(##__VA_ARGS__))(offset)

#define HIOCTANE_API extern "C" __declspec(dllexport)

#undef GetObject