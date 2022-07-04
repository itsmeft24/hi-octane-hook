#pragma once

#include <filesystem>

#include "HookFunction.h"

namespace FileSystem {
	
    struct CallbackContext {
        const char* FilePath;
        void* DataPtr;
        unsigned long MaxFileSize;
        unsigned long FileSize;
    };

    struct CallbackInfo {
        unsigned long MaxFileSize;
        void* CallBackFunction;
    };

    typedef int (*CarsFileCallback)(CallbackContext*);

    std::filesystem::path GetPathForFile(std::string path);
	
    void Init();

    void Deinit();
};