#pragma once

#include <iostream>

extern std::string CURRENT_DIRECTORY;

extern std::string DATA_DIR_PATH;

#define AsVoidPtr(x) (void*)(x)

#define DeclareFunction(ret_type, call_conv, name, offset, ...) auto name = (ret_type(call_conv*)(##__VA_ARGS__))(offset)

#define HIOCTANE_API extern "C" __declspec(dllexport)