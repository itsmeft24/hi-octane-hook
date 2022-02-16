#pragma once
#include <unordered_map>
#include <algorithm>
#include <Windows.h>
#include <winternl.h>
#include "Utils.h"

namespace CarsActivityUI_RequestDialogueHook { // Not currently working, will fix later

    std::vector<std::string> dialogue_list_append;

    DECL_FUNCTION(void, __thiscall, AddNameToDialogueList, CarsActivity_AddNameToDialogueList, void*, const char*);

    void __fastcall RequestDialogue(DWORD* this_ptr) {

        int iVar1 = *(int*)(this_ptr + 0x504);

        if (iVar1 == 0) {
            AddNameToDialogueList(this_ptr, "mcq");
            AddNameToDialogueList(this_ptr, "mat");
            AddNameToDialogueList(this_ptr, "hud");
            AddNameToDialogueList(this_ptr, "ram");
            AddNameToDialogueList(this_ptr, (const char*)0x00686574);
            AddNameToDialogueList(this_ptr, "sar");
            AddNameToDialogueList(this_ptr, "fil");
            AddNameToDialogueList(this_ptr, (const char*)0x0068EDC0);
            AddNameToDialogueList(this_ptr, "sven");
            AddNameToDialogueList(this_ptr, "otto");
            AddNameToDialogueList(this_ptr, "hiro");
            AddNameToDialogueList(this_ptr, "gio");
            AddNameToDialogueList(this_ptr, "emma");
            AddNameToDialogueList(this_ptr, "mcqm");
            AddNameToDialogueList(this_ptr, "matm");
            AddNameToDialogueList(this_ptr, "sulm");
            AddNameToDialogueList(this_ptr, "mike");

            for (int x = 0; x < dialogue_list_append.size(); x++) {
                AddNameToDialogueList(this_ptr, dialogue_list_append[x].c_str());
                Logging::Log("[Patches::CarsActivityUI::RequestDialogue] Added %s to the Dialogue List!\n", dialogue_list_append[x].c_str());
            }
        }
        else {
            if (iVar1 == 1) {
                AddNameToDialogueList(this_ptr, "mcq");
                return;
            }
            if (iVar1 == 2) {
                AddNameToDialogueList(this_ptr, "mcq");
                AddNameToDialogueList(this_ptr, "mcqm");
                return;
            }
        }

    }

    bool CollectCharactersToPatch() {
        std::string DialogueListFilePath = "c\\global\\chars\\dialoguelist.ini";

        if (ModSupport::MAP.find(DialogueListFilePath) != ModSupport::MAP.end()) {
            DialogueListFilePath = CURRENT_DIRECTORY + "\\mods\\" + ModSupport::MAP.at(DialogueListFilePath) + DialogueListFilePath;
        }
        else {
            if (std::filesystem::exists(CURRENT_DIRECTORY + "\\DataPC\\C\\Global\\Chars\\DialogueList.ini"))
                DialogueListFilePath = CURRENT_DIRECTORY + "\\DataPC\\C\\Global\\Chars\\DialogueList.ini";
            else
                return false;
        }

        std::ifstream file(DialogueListFilePath, std::ios::in);
        if (!file)
            return false;
        
        std::string line;
        while (std::getline(file, line)) {
            dialogue_list_append.push_back(line);
        }
        file.close();
        return true;
    }

    void install() {
        if (CollectCharactersToPatch())
            InstallReplacementHook((char*)CarsActivityUI_RequestDialogue, (char*)&RequestDialogue, 0x110, "CarsActivityUI::RequestDialogue");
    };

    void uninstall() {

    };
};

namespace NumberOfCarsPatcher { // will require a lot more reversing to actually implement, so consider this dead code

    int getNumberOfCars() {
        std::ifstream dialogue_txt(CURRENT_DIRECTORY + "DataPC\\C\\Audio\\Dialogue\\dialogue.txt", std::ios::in);
        std::string line;
        while (std::getline(dialogue_txt, line)) {
            if (line.find("NumberOfCars") != -1) {
                dialogue_txt.close();
                return std::stoi(line.substr(12));
            }
        }
        dialogue_txt.close();
        return 0x50; // Default Value
    }
    /*
    int getNumberOfCars()
    {
        ParameterBlock pBlock;
        pBlock.OpenFile("C\\Audio\\Dialogue\\dialogue.txt"); // allows the user to replace this file if they want to
        int NumberOfCars;
        pBlock.GetParameter("NumberOfCars", 0x50, &NumberOfCars);
        return NumberOfCars;
    }
    */
    void install() {
        BYTE num = getNumberOfCars();
        
        *(BYTE*)(CarsDialogue_Constructor + 0x3F) = num;
        *(BYTE*)(CarsDialogue_Destructor + 0x7D) = num;
        *(BYTE*)(CarsDialogue_Create + 0x5BE) = num;
    
    }

};

namespace ModSupport {

    std::unordered_map<std::string, std::string> MAP;

    DECL_FUNCTION(DWORD, __stdcall, BASS_SampleLoad, GetProcAddress(GetModuleHandleA("bass.dll"), "BASS_SampleLoad"), BOOL, char*, DWORD, DWORD, DWORD, DWORD); // i made this name up
    
    DECL_FUNCTION(DWORD, __stdcall, BASS_StreamCreateFile, GetProcAddress(GetModuleHandleA("bass.dll"), "BASS_StreamCreateFile"), BOOL, char*, DWORD, DWORD, DWORD); // i made this name up

    DECL_FUNCTION(FILE*, __cdecl, __fsopen, 0x0063fbfb, char*, char*, int); // VS2005 CRT function

    bool FileDiscovery() {
        std::filesystem::path mods_dir(CURRENT_DIRECTORY+"\\mods\\");
        if (std::filesystem::exists(mods_dir)) {
            for (const auto mod : std::filesystem::directory_iterator(mods_dir)) {
                if (mod.is_directory()) {
                    for (const auto& entry : std::filesystem::recursive_directory_iterator(mod)) {
                        if (entry.is_regular_file()) {

                            if (entry.path().filename().string() == "mod.info" || entry.path().extension().string() == ".dll")
                                continue;

                            if (entry.path().extension().string() == ".ogg") { // the stream file open function doesnt pass a filepath with an extension to ssnprintf, it instead concatenates the extension after. dont ask why, i dont know.

                                auto stripped_filename = entry.path().parent_path().string() + "\\" + entry.path().stem().string();

                                auto base_filename = stripped_filename.substr(mod.path().string().size() + 1);
                                make_lowercase(base_filename);
                                MAP.insert({ base_filename, mod.path().filename().string() });
                            }

                            auto base_filename = entry.path().string().substr(mod.path().string().size() + 1);
                            make_lowercase(base_filename);
                            Logging::Log("[ModSupport::FileDiscovery] Found file: %s in mod: %s\n", base_filename.c_str(), mod.path().filename().string().c_str());
                            MAP.insert({ base_filename, mod.path().filename().string() });
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

    int __cdecl SsnprintfHook(char* buf, int len, char* format ...) // reimplemented ssnprintf for single-call inside another function
    {
        if (len == 0)
            return 0;

        va_list arg;
        va_start(arg, format);

        std::string path = *(char**)(arg+4); // va_arg stuff

        make_lowercase(path);

        if (MAP.find(path) != MAP.end()) {
            auto out_path = CURRENT_DIRECTORY + "\\mods\\" + MAP.at(path) + "\\" + path;

            strcpy(buf, out_path.c_str());

            Logging::Log("[ModSupport::Ssnprintf] Loading file: %s from mod: %s...\n", path.c_str(), MAP.at(path).c_str());

            return out_path.size();
        }

        else {
            int ret = vsnprintf(buf, len, format, arg);
            va_end(arg);
            return ret;
        }

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
        }

        // return expected result
        GetFunctionInfo("BASS_StreamCreateFile").RestoreOriginalFunction();
        auto ret = BASS_StreamCreateFile(mem, file, offset, length, flags);
        GetFunctionInfo("BASS_StreamCreateFile").ReinstallReplacementHook();
        return ret;
    }

    FILE* __cdecl _fopenHook(char* _Filename, char* _Mode) // modified _fopen from VS2005
    {

        if (_strnicmp(_Filename, DATA_DIR_PATH.c_str(), DATA_DIR_PATH.size()) == 0) { // is a DataPC file

            std::string base_filepath = _Filename;

            base_filepath = base_filepath.substr(DATA_DIR_PATH.size() + 1); // strip DataPC path

            make_lowercase(base_filepath); // force lowercase

            if (MAP.find(base_filepath) != MAP.end()) { // if we have a modfile for the file
                auto out_path = CURRENT_DIRECTORY + "\\mods\\" + MAP.at(base_filepath) + "\\" + base_filepath;

                Logging::Log("[ModSupport::FOpen] Loading file: %s from mod: %s...\n", base_filepath.c_str(), MAP.at(base_filepath).c_str());

                return __fsopen((char*)out_path.c_str(), _Mode, 0x40); // return expected result
            }
        }
        return __fsopen((char*)_Filename, _Mode, 0x40); // return expected result
    }

    bool install() {
        if (FileDiscovery()) {
            DWORD oldProtect;
            VirtualProtect((LPVOID)0x0040FA62, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
            WriteCALL(0x0040FA62, &_fopenHook); // undocumented file io function

            VirtualProtect((LPVOID)0x00417B2D, 5, PAGE_EXECUTE_READWRITE, &oldProtect);
            WriteCALL(0x00417B2D, &SsnprintfHook); // bink opening thing
            
            InstallReplacementHook((char*)GetProcAddress(GetModuleHandleA("bass.dll"), "BASS_SampleLoad"), (char*)&BASS_SampleLoadHook, 6, "BASS_SampleLoad");
            
            InstallReplacementHook((char*)GetProcAddress(GetModuleHandleA("bass.dll"), "BASS_StreamCreateFile"), (char*)&BASS_StreamCreateFileHook, 9, "BASS_StreamCreateFile");
        }
    }

    void uninstall() {

        //...

        UninstallReplacementHook("BASS_SampleLoad");
        UninstallReplacementHook("BASS_StreamCreateFile");
    }

};

namespace DataAccessLogging {

    // Will do more interesting things in the future right now this is all it does

    void DECL_CARSINLINEHOOK DataAccess_FOpenHook() {

        GetInlineContext();

        __asm
        {
            push ebp
            mov ebp, esp
        }

        char* FileName;
        FileName = (char*)ctx.EBP;

        Logging::Log("[Logging::DataAccess::FOpen] Opened file: %s\n", FileName);

        __asm
        {
            mov esp, ebp
            pop ebp
        }

        RETURN;

    }

    void install() {
        InlineHook32(DataAccess_FOPEN + 0xB, 8, &DataAccess_FOpenHook);
    }

    void uninstall() {
        RemoveInlineHook32(DataAccess_FOPEN + 0xB, 8, &DataAccess_FOpenHook);
    }
};
