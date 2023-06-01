
#include "../Globals.h"
#include "../framework.hpp"
#include "../Logging.h"

#include "DataAccessLogging.h"

struct DataAccess;

DeclareFunction(int, __thiscall, DataAccess_FindVirtualFile, 0x00580FD0, DataAccess*, char *);

DefineReplacementHook(LogDataAccessFOpen) {
    static int __fastcall callback(DataAccess* _this, uintptr_t edx, char* file_name, char* access) {
        if (DataAccess_FindVirtualFile(_this, file_name) == -1) {
            logging::log("[DataAccess::FOpen] Attempting to open file from disk: {}...", file_name);
        }
        else {
            logging::log("[DataAccess::FOpen] Attempting to open virtual file: {}...", file_name);
        }
        return original(_this, edx, file_name, access);
    }
};

DefineReplacementHook(LogDataAccessLoadResFile) {
    static int __fastcall callback(DataAccess* _this, uintptr_t edx, char* file_name, int encryptedOnly, int userDataType, int userData, int userDataBufferSizeInBytes, int bytesOfUserDataRead, int forceLoad, int forceRead, int param_10, int param_11) {
        logging::log("[DataAccess::LoadResourceFile] Attempting to load ResourceFile: {}...", file_name);

        return original(_this, edx, file_name, encryptedOnly, userDataType, userData, userDataBufferSizeInBytes, bytesOfUserDataRead, forceLoad, forceRead, param_10, param_11);
    }
};


void data_access_logging::install() {
    LogDataAccessFOpen::install_at_ptr(0x005D34F0);
    LogDataAccessLoadResFile::install_at_ptr(0x005D2FC0);
    logging::log("[data_access_logging::install] Successfully installed patch!");
}
