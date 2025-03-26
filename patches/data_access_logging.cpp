
#include "core/globals.hpp"
#include "sunset/sunset.hpp"
#include "core/logging.hpp"
#include "core/config.hpp"
#include "core/game/data_access.hpp"

#include "data_access_logging.hpp"

DefineReplacementHook(LogDataAccessFOpen) {
    static int __fastcall callback(DataAccess* _this, uintptr_t edx, char* file_name, char* access) {
        if (_this->FindVirtualFile(file_name) == -1) {
            logging::log("[DataAccess::FOpen] Attempting to open file from disk: {}...", file_name);
        }
        else {
            logging::log("[DataAccess::FOpen] Attempting to open virtual file: {}...", file_name);
        }
        return original(_this, edx, file_name, access);
    }
};

DefineReplacementHook(LogDataAccessLoadResFile) {
    static int __fastcall callback(DataAccess* _this, uintptr_t edx, char* file_name, int encrypted_only, int user_data_type, int user_data, int user_data_buffer_size, int bytes_of_user_data_read, int force_load, int force_read, int param_10, int param_11) {
        logging::log("[DataAccess::LoadResourceFile] Attempting to load ResourceFile: {}...", file_name);

        return original(_this, edx, file_name, encrypted_only, user_data_type, user_data, user_data_buffer_size, bytes_of_user_data_read, force_load, force_read, param_10, param_11);
    }
};


void data_access_logging::install() {
    if (config::g_DataAccessLoggingEnabled) {
        LogDataAccessFOpen::install_at_ptr(0x005D34F0);
        LogDataAccessLoadResFile::install_at_ptr(0x005D2FC0);
        logging::log("[data_access_logging::install] Successfully installed patch!");
    }
}
