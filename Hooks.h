#pragma once
#include <unordered_map>
#include <Windows.h>
#include <random>

#include <rapidjson/document.h>

namespace DialogueListEx {

    HookedFunctionInfo request_dialogue_finfo;

    std::vector<std::string> dialogue_list = {
    "mcq",
    "mat",
    "hud",
    "ram",
    "flo",
    "sar",
    "fil",
    "lui",
    "sven",
    "otto",
    "hiro",
    "gio",
    "emma",
    "mcqm",
    "matm",
    "sulm",
    "mike"
    };

    void __fastcall RequestDialogue(void* this_ptr) {
        for (int x = 0; x < dialogue_list.size(); x++) {
            CarsActivity_AddNameToDialogueList(this_ptr, dialogue_list[x].c_str());
            Logging::Log("[DialogueListEx::CarsActivityUI::RequestDialogue] Added %s to the Dialogue List!\n", dialogue_list[x].c_str());
        }
    }

    bool CollectCharactersToPatch() {
        std::ifstream file(FileSystem::GetPathForFile("c\\global\\chars\\dialoguelist.ini"), std::ios::in);
        if (!file)
            return false;

        std::string line;
        while (std::getline(file, line)) {
            if (std::find(dialogue_list.begin(), dialogue_list.end(), line) == dialogue_list.end())
                dialogue_list.push_back(line);
        }
        file.close();
        return true;
    }

    HIOCTANE_API bool RegisterDialogueListAddition(const char* character) {
        if (std::find(dialogue_list.begin(), dialogue_list.end(), character) == dialogue_list.end()) {
            dialogue_list.push_back(character);
            return true;
        }
        return false;
    }

    void Install() {
        request_dialogue_finfo = HookFunction((void*&)CarsActivityUI_RequestDialogue, &RequestDialogue, 0x110, FunctionHookType::EntireReplacement);
        if (request_dialogue_finfo.type != FunctionHookType::Invalid)
            Logging::Log("[DialogueListEx::Install] Successfully installed patch!\n");
        CollectCharactersToPatch();
    };

    void Uninstall() {
        UninstallFunctionHook(request_dialogue_finfo);
    };
};

namespace DataAccessLogging {

    HookedFunctionInfo fopen_finfo;
    HookedFunctionInfo loadresourcefile_finfo;

    // Prevent compiler from removing the in_EDX arguments.
#pragma optimize( "", off )
    int __fastcall DataAccess_FOpenHook(void* this_ptr, void* in_EDX, char* FileName, char* Access) {

        if (DataAccess_FindVirtualFile(this_ptr, FileName) == -1)
            Logging::Log("[DataAccess::FOpen] Attempting to open file from disk: %s...\n", FileName);
        else
            Logging::Log("[DataAccess::FOpen] Attempting to open virtual file: %s...\n", FileName);

        return DataAccess_FOpen(this_ptr, FileName, Access);
    }

    int __fastcall DataAccess_LoadResourceFileHook(void* this_ptr, void* in_EDX, char* filename, int encryptedOnly, int userDataType, int userData, int userDataBufferSizeInBytes, int bytesOfUserDataRead, int forceLoad, int forceRead, int param_10, int param_11) {
        
        Logging::Log("[DataAccess::LoadResourceFile] Attempting to load ResourceFile: %s...\n", filename);
        
        return DataAccess_LoadResourceFile(this_ptr, filename, encryptedOnly, userDataType, userData, userDataBufferSizeInBytes, bytesOfUserDataRead, forceLoad, forceRead, param_10, param_11);
    }
#pragma optimize( "", on )

    void Install() {
        fopen_finfo = HookFunction((void*&)DataAccess_FOpen, &DataAccess_FOpenHook, 6, FunctionHookType::EntireReplacement);

        loadresourcefile_finfo = HookFunction((void*&)DataAccess_LoadResourceFile, &DataAccess_LoadResourceFileHook, 5, FunctionHookType::EntireReplacement);

        if (fopen_finfo.type != FunctionHookType::Invalid && loadresourcefile_finfo.type != FunctionHookType::Invalid)
            Logging::Log("[DataAccessLogging::Install] Successfully installed patch!\n");
    }

    void Uninstall() {
        UninstallFunctionHook(fopen_finfo);
        UninstallFunctionHook(loadresourcefile_finfo);
    }
};

namespace LargeVehiclePatch {
    // Allows the user to specify which characters should use the Monster Truck CSS Camera animation instead of the default one.

    HookedFunctionInfo largevehicle_finfo;

    std::vector<std::string> large_vehicles;

    bool is_installed = false;

    bool CollectCharactersToPatch() {
        std::ifstream file(FileSystem::GetPathForFile("c\\global\\chars\\largevehicles.ini"), std::ios::in);
        if (!file)
            return false;

        std::string line;
        while (std::getline(file, line)) {
            make_lowercase(line);
            large_vehicles.push_back(line);
        }
        file.close();
        return true;
    }

    BOOL __stdcall IsNotLargeVehicle(char* character) {
        auto string = std::string(character);
        make_lowercase(string);
        auto ret = std::find(large_vehicles.begin(), large_vehicles.end(), string) == large_vehicles.end();
        return ret;
    }

    void Install() {
        if (CollectCharactersToPatch()) {
            largevehicle_finfo = HookFunction(0x0050FB1F, &IsNotLargeVehicle, 0x3B, FunctionHookType::InlineReplacement);
            Logging::Log("[LargeVehiclePatch::Install] Successfully installed patch!\n");
            is_installed = true;
        }
    };

    void Uninstall() {
        if (is_installed) {
            UninstallFunctionHook(largevehicle_finfo);
            is_installed = false;
        }
    };
};

namespace CarsDialogueEX {

    __declspec(naked) void CarsDialogue_CarsDialogue_LEA_PATCH() {
        __asm {
            // lea eax, [esi + 0x114]
            lea eax, [esi + 0xA40]
            ret
        }
    }

    __declspec(naked) void CarsDialogue_Create_LEA_PATCH() {
        __asm {
            // lea edi, [esi + 0x120]
            lea edi, [esi + 0xA4C]
            ret
        }
    }

    __declspec(naked) void CarsDialogue_LoadDialogue_LEA_PATCH() {
        __asm {
            // lea ecx, [eax * 0x8]
            // sub ecx, eax
            // lea esi, [edi + ecx * 4 + 0x114]
            imul ecx, eax, 0x1C
            lea esi, [edi + ecx + 0xA40]
            ret
        }
    }

    __declspec(naked) void CarsDialogue_Reset_LEA_PATCH() {
        __asm {
            // lea ebx, [esi + 0x114]
            lea ebx, [esi + 0xA40]
            ret
        }
    }

    __declspec(naked) void CarsDialogue_BumpWreckOutOfControl_LEA_PATCH() {
        __asm {
            // lea ecx, [eax * 0x8]
            // sub ecx, eax
            // cmp [ebx + ecx * 0x4 + 0x118], 0x0
            imul ecx, eax, 0x1C
            cmp [ebx + ecx + 0xA44], 0x0
            ret
        }
    }

    __declspec(naked) void CarsDialogue_BumpWreckOutOfControl_PART2_LEA_PATCH() {
        __asm {
            // lea ebp, [ebx + ecx * 0x4 + 0x114]
            lea ebp, [ebx + ecx + 0xA44]
            ret
        }
    }

    __declspec(naked) void CarsDialogue_UNK_LEA_PATCH() {
        __asm {
            // lea eax, [edi + 0x11C]
            lea ebx, [edi + 0xA48]
            ret
        }
    }

    __declspec(naked) void CarsDialogue_UNK2_LEA_PATCH() {
        __asm {
            // lea edi, [esi + 0x11C]
            lea edi, [esi + 0xA48]
            ret
        }
    }

    __declspec(naked) void CarsDialogue_UNK3_LEA_PATCH() {
        __asm {
            // lea ecx, [esi * 0x8]
            // sub ecx, esi
            // lea esi, [ebx + ecx * 4 + 0x114]
            imul ecx, esi, 0x1C
            lea esi, [ebx + ecx + 0xA40]
            ret
        }
    }

    __declspec(naked) void CarsDialogue_UNK4_LEA_PATCH() {
        __asm {
            // lea ecx, [eax * 0x8]
            // sub ecx, eax
            // lea eax, [esi + ecx * 4 + 0x114]
            imul ecx, eax, 0x1C
            lea eax, [esi + ecx + 0xA40]
            ret
        }
    }

    __declspec(naked) void CarsDialogue_UNK5_LEA_PATCH() {
        __asm {
            // lea ecx, [eax * 0x8]
            // sub ecx, eax
            // lea eax, [edi + ecx * 4 + 0x114]
            imul ecx, eax, 0x1C
            lea eax, [edi + ecx + 0xA40]
            ret
        }
    }

    __declspec(naked) void CarsDialogue_UNK6_LEA_PATCH() {
        __asm {
            // lea ecx, [eax * 0x8]
            // sub ecx, eax
            // lea eax, [ebx + ecx * 4 + 0x114]
            imul ecx, eax, 0x1C
            lea eax, [ebx + ecx + 0xA40]
            ret
        }
    }

    void Install() {
        Logging::Log("[CarsDialogueEX] Patching CarsDialogue class...\n");
        WritePUSH(0x004f32c8, 0xA40 + 0x1BE4); // patch class size

        HookFunction(0x004C2965, &CarsDialogue_BumpWreckOutOfControl_LEA_PATCH, 0x11, FunctionHookType::InlineReplacement); // PlayBumpDialogue
        HookFunction(0x004c2977, &CarsDialogue_BumpWreckOutOfControl_PART2_LEA_PATCH, 0x7, FunctionHookType::InlineReplacement); // PlayBumpDialogue
        
        HookFunction(0x004C2A95, &CarsDialogue_BumpWreckOutOfControl_LEA_PATCH, 0x11, FunctionHookType::InlineReplacement); // PlayWreckDialogue
        HookFunction(0x004C2AA7, &CarsDialogue_BumpWreckOutOfControl_PART2_LEA_PATCH, 0x7, FunctionHookType::InlineReplacement); // PlayWreckDialogue
        
        HookFunction(0x004C2C95, &CarsDialogue_BumpWreckOutOfControl_LEA_PATCH, 0x11, FunctionHookType::InlineReplacement); // PlayOutOfControlDialogue
        HookFunction(0x004C2CA7, &CarsDialogue_BumpWreckOutOfControl_PART2_LEA_PATCH, 0x7, FunctionHookType::InlineReplacement); // PlayOutOfControlDialogue
        
        HookFunction(0x00482B72, &CarsDialogue_CarsDialogue_LEA_PATCH, 0x6, FunctionHookType::InlineReplacement); // CarsDialogue
        HookFunction(0x00482CD0, &CarsDialogue_CarsDialogue_LEA_PATCH, 0x6, FunctionHookType::InlineReplacement); // ~CarsDialogue
        
        HookFunction(0x0043E251, &CarsDialogue_UNK_LEA_PATCH, 0x6, FunctionHookType::InlineReplacement); // UNK
        
        HookFunction(0x004C2161, &CarsDialogue_UNK2_LEA_PATCH, 0x6, FunctionHookType::InlineReplacement); // UNK2
        
        HookFunction(0x004C1FF1, &CarsDialogue_Create_LEA_PATCH, 0x6, FunctionHookType::InlineReplacement); // Create
        
        HookFunction(0x004C2194, &CarsDialogue_UNK3_LEA_PATCH, 16, FunctionHookType::InlineReplacement); // UNK3
        
        HookFunction(0x00482D61, &CarsDialogue_LoadDialogue_LEA_PATCH, 0x7, FunctionHookType::InlineReplacement); // LoadDialogue
        HookFunction(0x00482EA8, &CarsDialogue_Reset_LEA_PATCH, 0x6, FunctionHookType::InlineReplacement); // Reset
        
        HookFunction(0x0048319E, &CarsDialogue_UNK4_LEA_PATCH, 16, FunctionHookType::InlineReplacement); // UNK4
        HookFunction(0x004C2BB3, &CarsDialogue_UNK4_LEA_PATCH, 16, FunctionHookType::InlineReplacement); // PlayOuchDialogue
        HookFunction(0x004C2BB3, &CarsDialogue_UNK5_LEA_PATCH, 16, FunctionHookType::InlineReplacement); // PlayPassedDialogue
        HookFunction(0x004C3026, &CarsDialogue_UNK5_LEA_PATCH, 16, FunctionHookType::InlineReplacement); // UNK5
        
        HookFunction(0x004C310E, &CarsDialogue_UNK4_LEA_PATCH, 16, FunctionHookType::InlineReplacement); // UNK6
        HookFunction(0x004EECBB, &CarsDialogue_UNK4_LEA_PATCH, 16, FunctionHookType::InlineReplacement); // UNK7
        HookFunction(0x004EED4A, &CarsDialogue_UNK4_LEA_PATCH, 16, FunctionHookType::InlineReplacement); // UNK8
        
        HookFunction(0x004EEF15, &CarsDialogue_UNK6_LEA_PATCH, 16, FunctionHookType::InlineReplacement); // UNK9
        
        HookFunction(0x004F4C51, &CarsDialogue_UNK4_LEA_PATCH, 16, FunctionHookType::InlineReplacement); // UNK10

        HookFunction(0x004F4CAC, &CarsDialogue_UNK4_LEA_PATCH, 16, FunctionHookType::InlineReplacement); // UNK11

        HookFunction(0x004C2DCD, &CarsDialogue_UNK5_LEA_PATCH, 16, FunctionHookType::InlineReplacement); // PlayPassDialogue

        Logging::Log("[CarsDialogueEX] Succesfully moved CarsCharacterDialogue array!\n");

        auto lpCarsDialogue = *(DWORD*)(*(DWORD*)(lpCarsGame + 1088) + 68);
    }

};

namespace AnimationPatches {

    struct Motion {
        int among_usl;

    };

    void Motion_Library_LoadMotionFromMOTHook(const char* filename, Motion* motion) {
    
    }

};

namespace GameTextPatches {

    struct StringInfo {
        unsigned short MaxSize;
        char unk[2];
        char* ValuePTR;
        int IsDynamic;
        bool IsDynamic_;
        char has_unk[3]; // might actually be padding
        int** unk_arr;
        int has_unk2;
        byte** unk_arr2;
        //char reserved[0x1C];

    }; // sizeof == 0x1C

    struct GameText {
        char Name[16];
        int NumberOfStrings;
        char** TextIDPointers;
        void* CHTMap; // ContainerHashTable<char*, int>*
        StringInfo* StringInfos;
        int unused;
        char* TextIDBuffer;
        char* ValueBuffer;
    };

    // Prevent compiler from removing the in_EDX arguments.
#pragma optimize( "", off )
    GameText* __fastcall GameText_Create_Hook(GameText* this_ptr, void* in_EDX, char* name) {

        Logging::Log("[GameText::CreateFromJSON] Reading %s strings from JSON...\n", name);

        strcpy(this_ptr->Name, name);

        this_ptr->unused = 0;

        std::ifstream json_file(FileSystem::GetPathForFile("c\\loc\\" + std::string(name) + ".json"), std::ios::in | std::ios::binary);
        json_file.seekg(0, std::ios::end);
        unsigned int json_size = json_file.tellg();
        char* json_buffer = new char[json_size];
        json_file.seekg(0, std::ios::beg);
        json_file.read(json_buffer, json_size);
        json_file.close();
        rapidjson::Document doc;
        doc.Parse<rapidjson::kParseStopWhenDoneFlag>(json_buffer);
        delete[] json_buffer;

        this_ptr->TextIDBuffer = (char*)calloc(1, json_size); // technically this is larger than we need but its fine
        size_t textid_buf_written = 0;
        this_ptr->ValueBuffer = (char*)calloc(1, json_size); // technically this is larger than we need but its fine
        size_t value_buf_written = 0;

        this_ptr->NumberOfStrings = doc.GetArray().Size();

        this_ptr->TextIDPointers = new char* [this_ptr->NumberOfStrings];

        this_ptr->StringInfos = new StringInfo[this_ptr->NumberOfStrings];

        for (int x = 0; x < this_ptr->NumberOfStrings; x++) {

            const auto& elem = doc.GetArray()[x];

            auto textid = elem.GetObjectW()["TextID"].GetString();                
            strcpy(this_ptr->TextIDBuffer + textid_buf_written, textid);
            this_ptr->TextIDPointers[x] = this_ptr->TextIDBuffer + textid_buf_written;
            textid_buf_written += strlen(textid) + 1;

            

            auto value = elem.GetObjectW()["Value"].GetString();
            strcpy(this_ptr->ValueBuffer + value_buf_written, value);

            this_ptr->StringInfos[x] = {
                (unsigned short)strlen(textid),
                {0, 0},
                this_ptr->ValueBuffer + value_buf_written,
                1,
                1,
                {0, 0, 0},
                nullptr,
                false,
                nullptr
            };

            value_buf_written += strlen(textid) + 1;
        }
        Logging::Log("[GameText::CreateFromJSON] Returning...\n", name);
        return this_ptr;

    }

    void __fastcall GameText_LoadGameText_Hook(GameText* this_ptr, void* in_EDX, int param_1) {
        return;
    }
#pragma optimize( "", on )

    void install() {
        HookFunction((void*&)GameText_Create, &GameText_Create_Hook, 7, FunctionHookType::EntireReplacement);
        HookFunction((void*&)GameText_LoadGameText, &GameText_LoadGameText_Hook, 6, FunctionHookType::EntireReplacement);
    }

};

namespace WideScreenPatches {

    DWORD* pWindowWidth = (DWORD*)0x006FAA54;
    DWORD* pWindowHeight = (DWORD*)0x006FAA58;
    DWORD* pScreenMode = (DWORD*)0x006F72CC;

    __declspec(naked) void CMPPatch() { // Force the game to compare against an intermediate 1, rather than the contents of register EDI.
        __asm {
            cmp [pScreenMode], 1
            ret
        }
    }

    void Install() {
        if (ConfigManager::IsWidescreenEnabled) {
            Logging::Log("[WideScreenPatches::Install] Setting screen mode to widescreen...\n");

            HookFunction(0x00421E22, &CMPPatch, 6, FunctionHookType::InlineReplacement);

            *pScreenMode = 2;

            *pWindowWidth = ConfigManager::DesiredWindowWidth;
            *pWindowHeight = ConfigManager::DesiredWindowHeight;

            SetExecuteReadWritePermission((DWORD*)(0x00421DEF), 4); *(DWORD*)(0x00421DEF) = 2; // (mov edi, 2) instead of (mov edi, 1)

            // Replace 800x600 resolution case in switch-case with desired resolution. (This case is almost always hit by default!)
            SetExecuteReadWritePermission((DWORD*)(0x00414D17), 4); *(DWORD*)(0x00414D17) = ConfigManager::DesiredWindowWidth; // mov [edx], DesiredWindowWidth
            SetExecuteReadWritePermission((DWORD*)(0x00414D1D), 4); *(DWORD*)(0x00414D1D) = ConfigManager::DesiredWindowHeight; // mov [eax], DesiredWindowHeight

        }
    }
};

namespace LoadingScreenPatch {

    HookedFunctionInfo loading_screen_finfo;

    DWORD* ret_address = (DWORD*)0x004053A2;

    const char E_UI_Tex_[] = "e\\ui\\tex\\%s";

    __declspec(naked) void CalculateLoadingScreenPath() {
        
        __asm {
            mov eax, [lpCarsGame]
            mov eax, [eax]
            add eax, 0x764 // lpCarsGame->LoadingScreenPath
            push eax
            push OFFSET [E_UI_Tex_]
            lea eax, [esp + 0x10]
            push 0x104
            push eax
            jmp ret_address
        }
    }

    void Install() {
        loading_screen_finfo = HookFunction(0x0040538e, &CalculateLoadingScreenPath, 0x14, FunctionHookType::InlineReplacementJMP);
        if (loading_screen_finfo.type != FunctionHookType::Invalid)
            Logging::Log("[LoadingScreenPatch::Install] Successfully installed patch!\n");
    }

};
