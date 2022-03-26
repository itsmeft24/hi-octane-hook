#pragma once
#include <random>
#include <unordered_map>
#include <Windows.h>

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

    std::unordered_map<std::string, std::string> MAP;

    std::vector<std::string> MARK_AS_DELETED;

    std::unordered_map<std::string, CallbackInfo> InstalledCallbacks;

    std::unordered_map<FILE*, std::filesystem::path> FileBacking;

    DECL_FUNCTION(DWORD*, __stdcall, BinkOpen, GetProcAddress(GetModuleHandleA("binkw32.dll"), "_BinkOpen@8"), char*, DWORD);

    DECL_FUNCTION(DWORD, __stdcall, BASS_SampleLoad, GetProcAddress(GetModuleHandleA("bass.dll"), "BASS_SampleLoad"), BOOL, char*, DWORD, DWORD, DWORD, DWORD);

    DECL_FUNCTION(DWORD, __stdcall, BASS_StreamCreateFile, GetProcAddress(GetModuleHandleA("bass.dll"), "BASS_StreamCreateFile"), BOOL, char*, DWORD, DWORD, DWORD);

    DECL_FUNCTION(FILE*, __cdecl, __fsopen, 0x0063FBFB, char*, char*, int); // VS2005 CRT function

    DECL_FUNCTION(int, __cdecl, _fclose, 0x0063FD94, FILE*); // VS2005 CRT function
    // Note: Since we never dereference these FILE* pointers, it does not matter that our modern definition of FILE wont match up with the 2005 one.

    typedef int (*CarsFileCallback)(CallbackContext*);

    std::filesystem::path GetPathForFile(std::string path) {
        make_lowercase(path);
        if (MAP.find(path) != MAP.end()) { // if we have a modfile for the file
            return CURRENT_DIRECTORY + "\\mods\\" + MAP.at(path) + "\\" + path;
        }
        return CURRENT_DIRECTORY + "\\DataPC\\" + path;
    }

    std::filesystem::path GenerateFileBackingPath() {
        std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

        std::random_device rd;
        std::mt19937 generator(rd());

        std::shuffle(str.begin(), str.end(), generator);

        return CURRENT_DIRECTORY + "\\LocalStorage\\FileBacking\\" + str.substr(0, 16) + ".tmp";    // assumes 32 < number of characters in str         
    }

    CARSHOOK_API void InstallCarsFileCallback(char* filepath, size_t max_size, CarsFileCallback callbackfunc) {
        InstalledCallbacks.insert_or_assign(filepath, CallbackInfo{ max_size, callbackfunc });
    }

    CARSHOOK_API bool LoadOriginalFile(CallbackContext* context) {
        auto p = GetPathForFile(context->FilePath);
        unsigned int size = 0;
        if (context->FileSize == 0)
            return false; // file does not exist in base game OR in installed mods
        std::ifstream f(p, std::ios::in | std::ios::binary);
        f.read((char*)context->DataPtr, context->FileSize);
        f.close();
        return true;
    }

    bool FileDiscovery() {
        std::filesystem::path mods_dir(CURRENT_DIRECTORY + "\\mods\\");
        if (std::filesystem::is_directory(mods_dir)) {
            for (const auto mod : std::filesystem::directory_iterator(mods_dir)) {
                if (mod.is_directory() && mod.path().filename().string()[0] != '.') {
                    for (const auto& entry : std::filesystem::recursive_directory_iterator(mod)) {
                        if (entry.is_regular_file()) {

                            if (entry.path().filename().string() == "mod.info")
                                continue;

                            if (entry.path().filename().string() == "plugin.dll")
                                PluginManager::LoadPluginForMod(mod.path().filename().string());

                            if (entry.path().filename().string() == "mark_as_deleted.txt") {
                                std::ifstream file(entry.path(), std::ios::in);
                                if (!file)
                                    return false;

                                std::string line;
                                while (std::getline(file, line)) {
                                    if (std::find(MARK_AS_DELETED.begin(), MARK_AS_DELETED.end(), line) == MARK_AS_DELETED.end()) {
                                        MARK_AS_DELETED.push_back(line);
                                        Logging::Log("[ModSupport::FileDiscovery] Added %s to the deletion queue.\n", line.c_str());
                                    }
                                }
                                file.close();
                                continue;
                            }

                            if (entry.path().extension().string() == ".ogg") { // the stream file open function doesnt pass a filepath with an extension to ssnprintf, it instead concatenates the extension after. dont ask why, i dont know.

                                auto stripped_filename = entry.path().parent_path().string() + "\\" + entry.path().stem().string();

                                auto base_filename = stripped_filename.substr(mod.path().string().size() + 1);
                                make_lowercase(base_filename);

                                if (MAP.find(base_filename) != MAP.end()) {
                                    MAP.insert_or_assign(base_filename, mod.path().filename().string());
                                }
                                else {
                                    MAP.insert_or_assign(base_filename, mod.path().filename().string());
                                }
                            }

                            auto base_filename = entry.path().string().substr(mod.path().string().size() + 1);
                            make_lowercase(base_filename);

                            if (MAP.find(base_filename) != MAP.end()) {
                                Logging::Log("[ModSupport::FileDiscovery] File: %s in mod: %s overwrites existing mod file in: %s\n", base_filename.c_str(), mod.path().filename().string().c_str(), MAP.at(base_filename).c_str());
                                MAP.insert_or_assign(base_filename, mod.path().filename().string());
                            }
                            else {
                                Logging::Log("[ModSupport::FileDiscovery] Found file: %s in mod: %s\n", base_filename.c_str(), mod.path().filename().string().c_str());
                                MAP.insert_or_assign(base_filename, mod.path().filename().string());
                            }

                        }
                    }
                }
            }
            return !MAP.empty();
        }
        else {
            return false;
        }
    }

    DWORD* __stdcall BinkOpenHook(char* file, DWORD flags) {

        if (_strnicmp(file, DATA_DIR_PATH.c_str(), DATA_DIR_PATH.size()) == 0) { // is a DataPC file

            std::string base_filepath = file;

            base_filepath = base_filepath.substr(DATA_DIR_PATH.size() + 1); // strip DataPC path

            make_lowercase(base_filepath); // force lowercase

            if (MAP.find(base_filepath) != MAP.end()) { // if we have a modfile for the file
                auto out_path = CURRENT_DIRECTORY + "\\mods\\" + MAP.at(base_filepath) + "\\" + base_filepath;

                Logging::Log("[ModSupport::BinkW32::BinkOpen] Loading Bink file: %s from mod: %s...\n", base_filepath.c_str(), MAP.at(base_filepath).c_str());

                // return expected result
                GetFunctionInfo("_BinkOpen@8").RestoreOriginalFunction();
                auto* ret = BinkOpen((char*)out_path.c_str(), flags);
                GetFunctionInfo("_BinkOpen@8").ReinstallReplacementHook();
                return ret;
            }

            if (std::find(MARK_AS_DELETED.begin(), MARK_AS_DELETED.end(), base_filepath) != MARK_AS_DELETED.end()) {
                return 0;
            } // if the file should be marked as deleted, then return 0.
            // this comes after swapping the path for modfiles, so if another mod (or the mod itself) has that file it will still be loaded.
        }

        // return expected result
        GetFunctionInfo("_BinkOpen@8").RestoreOriginalFunction();
        auto* ret = BinkOpen(file, flags);
        GetFunctionInfo("_BinkOpen@8").ReinstallReplacementHook();
        return ret;
    }

    DWORD __stdcall BASS_SampleLoadHook(BOOL mem, char* file, DWORD offset, DWORD length, DWORD max, DWORD flags) {

        if (!mem && _strnicmp(file, DATA_DIR_PATH.c_str(), DATA_DIR_PATH.size()) == 0) { // is a DataPC file

            std::string base_filepath = file;

            base_filepath = base_filepath.substr(DATA_DIR_PATH.size() + 1); // strip DataPC path

            make_lowercase(base_filepath); // force lowercase

            if (MAP.find(base_filepath) != MAP.end()) { // if we have a modfile for the file
                auto out_path = CURRENT_DIRECTORY + "\\mods\\" + MAP.at(base_filepath) + "\\" + base_filepath;

                Logging::Log("[ModSupport::BASS::SampleLoad] Loading stream file: %s from mod: %s...\n", base_filepath.c_str(), MAP.at(base_filepath).c_str());

                // return expected result
                GetFunctionInfo("BASS_SampleLoad").RestoreOriginalFunction();
                auto ret = BASS_SampleLoad(mem, (char*)out_path.c_str(), offset, length, max, flags);
                GetFunctionInfo("BASS_SampleLoad").ReinstallReplacementHook();
                return ret;
            }

            if (std::find(MARK_AS_DELETED.begin(), MARK_AS_DELETED.end(), base_filepath) != MARK_AS_DELETED.end()) {
                return 0;
            } // if the file should be marked as deleted, then return 0.
            // this comes after swapping the path for modfiles, so if another mod (or the mod itself) has that file it will still be loaded.
        }

        // return expected result
        GetFunctionInfo("BASS_SampleLoad").RestoreOriginalFunction();
        auto ret = BASS_SampleLoad(mem, file, offset, length, max, flags);
        GetFunctionInfo("BASS_SampleLoad").ReinstallReplacementHook();
        return ret;
    }

    DWORD __stdcall BASS_StreamCreateFileHook(BOOL mem, char* file, DWORD offset, DWORD length, DWORD flags) {

        if (!mem && _strnicmp(file, DATA_DIR_PATH.c_str(), DATA_DIR_PATH.size()) == 0) { // is a DataPC file

            std::string base_filepath = file;

            base_filepath = base_filepath.substr(DATA_DIR_PATH.size() + 1); // strip DataPC path

            make_lowercase(base_filepath); // force lowercase

            if (MAP.find(base_filepath) != MAP.end()) { // if we have a modfile for the file
                auto out_path = CURRENT_DIRECTORY + "\\mods\\" + MAP.at(base_filepath) + "\\" + base_filepath;

                Logging::Log("[ModSupport::BASS::StreamCreateFile] Loading stream file: %s from mod: %s...\n", base_filepath.c_str(), MAP.at(base_filepath).c_str());

                // return expected result
                GetFunctionInfo("BASS_StreamCreateFile").RestoreOriginalFunction();
                auto ret = BASS_StreamCreateFile(mem, (char*)out_path.c_str(), offset, length, flags);
                GetFunctionInfo("BASS_StreamCreateFile").ReinstallReplacementHook();
                return ret;
            }

            if (std::find(MARK_AS_DELETED.begin(), MARK_AS_DELETED.end(), base_filepath) != MARK_AS_DELETED.end()) {
                return 0;
            } // if the file should be marked as deleted, then return 0.
            // this comes after swapping the path for modfiles, so if another mod (or the mod itself) has that file it will still be loaded.
        }

        // return expected result
        GetFunctionInfo("BASS_StreamCreateFile").RestoreOriginalFunction();
        auto ret = BASS_StreamCreateFile(mem, file, offset, length, flags);
        GetFunctionInfo("BASS_StreamCreateFile").ReinstallReplacementHook();
        return ret;
    }

    FILE* __cdecl _fopenHook(char* _Filename, char* _Mode) { // modified _fopen from VS2005
        if (_strnicmp(_Filename, DATA_DIR_PATH.c_str(), DATA_DIR_PATH.size()) == 0) { // is a DataPC file

            std::string base_filepath = _Filename;
            base_filepath = base_filepath.substr(DATA_DIR_PATH.size() + 1); // strip DataPC path
            make_lowercase(base_filepath); // force lowercase

            if (InstalledCallbacks.find(base_filepath) != InstalledCallbacks.end()) { // if we have an installed callback for the file (callbacks take priority over normal files)

                auto& info = InstalledCallbacks.at(base_filepath);

                void* data = malloc(info.MaxFileSize); // allocate buffer for callback

                unsigned int size;

                try {
                    size = std::filesystem::file_size(GetPathForFile(base_filepath));
                }
                catch (std::filesystem::filesystem_error& e) {
                    size = 0;
                } // get size of file (could be a modfile or it could not be a modfile)

                CallbackContext clbkcontext{ base_filepath.c_str(), data, info.MaxFileSize, size };

                Logging::Log("[Hi-Octane API] Executing file callback for: %s...\n", base_filepath.c_str());

                auto clbk = InstalledCallbacks.at(base_filepath).CallBackFunction; // execute callback function

                ((CarsFileCallback)clbk)(&clbkcontext);

                auto filebackingpath = GenerateFileBackingPath(); // generate random string for filebacking

                std::ofstream f(filebackingpath, std::ios::out | std::ios::binary); // create file and write modified data to it
                f.write((char*)data, clbkcontext.FileSize); // size SHOULD be modified by the callback function
                free(data); // free allocated buffer (plugin SHOULD NOT hold on to this pointer)
                f.close();

                auto ret = __fsopen((char*)filebackingpath.c_str(), _Mode, 0x40); // return expected result

                FileBacking.insert_or_assign(ret, filebackingpath); // keep track of file pointers used in callback handling

                return ret;
            }

            if (MAP.find(base_filepath) != MAP.end()) { // if we have a modfile for the file
                auto out_path = CURRENT_DIRECTORY + "\\mods\\" + MAP.at(base_filepath) + "\\" + base_filepath;

                Logging::Log("[ModSupport::FOpen] Loading file: %s from mod: %s...\n", base_filepath.c_str(), MAP.at(base_filepath).c_str());

                return __fsopen((char*)out_path.c_str(), _Mode, 0x40); // return expected result
            }
        }
        return __fsopen(_Filename, _Mode, 0x40); // return expected result
    }

    int __cdecl _fcloseHook(FILE* fp) {
        if (FileBacking.find(fp) != FileBacking.end()) {

            GetFunctionInfo("_fclose").RestoreOriginalFunction();
            auto ret = _fclose(fp);
            GetFunctionInfo("_fclose").ReinstallReplacementHook();

            std::filesystem::remove(FileBacking.at(fp));

            FileBacking.erase(fp);

            return ret;
        }

        GetFunctionInfo("_fclose").RestoreOriginalFunction();
        auto ret = _fclose(fp);
        GetFunctionInfo("_fclose").ReinstallReplacementHook();
        return ret;
    }

    bool Init() {
        if (FileDiscovery()) {
            WriteCALL(0x0040FA62, &_fopenHook); // undocumented file io function, does not have a name yet
            // here, the call to _fopen is intercepted manually because we do not want to interfere with
            // calls to _fopen outside the games' file opener.

            InstallReplacementHook(0x0063FD94, &_fcloseHook, 7, "_fclose");

            InstallReplacementHook(GetProcAddress(GetModuleHandleA("binkw32.dll"), "_BinkOpen@8"), &BinkOpenHook, 6, "_BinkOpen@8");

            InstallReplacementHook(GetProcAddress(GetModuleHandleA("bass.dll"), "BASS_SampleLoad"), &BASS_SampleLoadHook, 6, "BASS_SampleLoad");

            InstallReplacementHook(GetProcAddress(GetModuleHandleA("bass.dll"), "BASS_StreamCreateFile"), &BASS_StreamCreateFileHook, 9, "BASS_StreamCreateFile");

            return true;
        }
        return false;
    }

    void Deinit() {

        WriteCALL(0x0040FA62, _fopen);

        UninstallReplacementHook("_fclose");
        UninstallReplacementHook("_BinkOpen@8");
        UninstallReplacementHook("BASS_SampleLoad");
        UninstallReplacementHook("BASS_StreamCreateFile");
    }

};