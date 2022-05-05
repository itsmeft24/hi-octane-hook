#pragma once
#include <unordered_map>
#include <Windows.h>
#include <random>

#include <rapidjson/document.h>

namespace CarsActivityUI_RequestDialogueHook {

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

    bool is_installed = false;

    DeclareFunction(void, __thiscall, CarsActivity_AddNameToDialogueList, CarsActivity_AddNameToDialogueList_Address, void*, const char*);

    void __fastcall RequestDialogue(void* this_ptr) {
        for (int x = 0; x < dialogue_list.size(); x++) {
            CarsActivity_AddNameToDialogueList(this_ptr, dialogue_list[x].c_str());
            Logging::Log("[CarsActivityUI::RequestDialogue] Added %s to the Dialogue List!\n", dialogue_list[x].c_str());
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

    HIOCTANE_API void RequestDialogueListAddition(const char* character) {
        if (std::find(dialogue_list.begin(), dialogue_list.end(), character) == dialogue_list.end())
            dialogue_list.push_back(character);
    }

    bool install() {
        is_installed = CollectCharactersToPatch();
        if (is_installed)
            HookFunction(CarsActivityUI_RequestDialogue_Address, &RequestDialogue, 0x110, FunctionHookType::EntireReplacement);
        return is_installed;
    };

    void uninstall() {
        if (is_installed) {
            UninstallFunctionHook(CarsActivityUI_RequestDialogue_Address);
            is_installed = false;
        }
    };
};

namespace DataAccessLogging {

    DeclareFunction(int, __thiscall, DataAccess_FOpen, DataAccess_FOpen_Address, void*, const char*, const char*);

    DeclareFunction(int, __thiscall, DataAccess_LoadResourceFile, DataAccess_LoadResourceFile_Address, void*, char*, int, int, int, int, int, int, int, int, int);

    DeclareFunction(int, __thiscall, DataAccess_FindVirtualFile, DataAccess_FindVirtualFile_Address, void*, char*);

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

    void install() {
        // HookFunction(DataAccess_FOpen_Address, &DataAccess_FOpenHook, 6, FunctionHookType::EntireReplacement);
        {
            WriteCALL(0x00410d0b, &DataAccess_FOpenHook);
            WriteCALL(0x00415ca2, &DataAccess_FOpenHook);
            WriteCALL(0x00415f22, &DataAccess_FOpenHook);
            WriteCALL(0x005d39b9, &DataAccess_FOpenHook);
            WriteCALL(0x005d5023, &DataAccess_FOpenHook);
            WriteCALL(0x005d5234, &DataAccess_FOpenHook);
            WriteCALL(0x005d57da, &DataAccess_FOpenHook);
            WriteCALL(0x005d64bb, &DataAccess_FOpenHook);
            WriteCALL(0x005d6ae5, &DataAccess_FOpenHook);
            WriteCALL(0x005d6d58, &DataAccess_FOpenHook);
            WriteCALL(0x005d8841, &DataAccess_FOpenHook);
            WriteCALL(0x005d88f4, &DataAccess_FOpenHook);
            WriteCALL(0x005e1567, &DataAccess_FOpenHook);
            WriteCALL(0x005e37d8, &DataAccess_FOpenHook);
            WriteCALL(0x005e3b2f, &DataAccess_FOpenHook);
            WriteCALL(0x005e3e1f, &DataAccess_FOpenHook);
            WriteCALL(0x005e40af, &DataAccess_FOpenHook);
            WriteCALL(0x005e4482, &DataAccess_FOpenHook);
            WriteCALL(0x005ebdc7, &DataAccess_FOpenHook);
            WriteCALL(0x005f25b4, &DataAccess_FOpenHook);
            WriteCALL(0x005f2fdd, &DataAccess_FOpenHook);
            WriteCALL(0x005f46e9, &DataAccess_FOpenHook);
            WriteCALL(0x00607561, &DataAccess_FOpenHook);
            WriteCALL(0x0060789e, &DataAccess_FOpenHook);
            WriteCALL(0x0060a31d, &DataAccess_FOpenHook);
            WriteCALL(0x006168f7, &DataAccess_FOpenHook);
        }
        
        // HookFunction(DataAccess_LoadResourceFile_Address, &DataAccess_LoadResourceFileHook, 5, FunctionHookType::EntireReplacement);
        {
            WriteCALL(0x005ed284, &DataAccess_LoadResourceFileHook);
            WriteCALL(0x005f2f05, &DataAccess_LoadResourceFileHook);
            WriteCALL(0x005f46d2, &DataAccess_LoadResourceFileHook);
        }

        // SetFunctionOffset(DataAccess_FOpen, GetRelocatedCode(DataAccess_FOpen));
        // SetFunctionOffset(DataAccess_LoadResourceFile, GetRelocatedCode(DataAccess_LoadResourceFile));
    }

    void uninstall() {
        // UninstallFunctionHook(DataAccess_FOpen_Address);
        // UninstallFunctionHook(DataAccess_LoadResourceFile_Address);
        // SetFunctionOffset(DataAccess_FOpen, DataAccess_FOpen_Address);
        // SetFunctionOffset(DataAccess_LoadResourceFile, DataAccess_LoadResourceFile_Address);
    }
};

namespace LargeVehiclePatch {
    // Allows the user to specify which characters should use the Monster Truck CSS Camera animation instead of the default one.

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

    bool install() {
        is_installed = CollectCharactersToPatch();
        if (is_installed) {
            HookFunction(0x0050FB1F, &IsNotLargeVehicle, 0x3B, FunctionHookType::InlineReplacement);
        }
        return is_installed;
    };

    void uninstall() {
        if (is_installed) {
            UninstallFunctionHook(0x0050FB1F);
            is_installed = false;
        }
    };
};

namespace CarsDialoguePatches {

    constexpr unsigned char MAX_NUMBER_OF_CARS = 0x50;

    // Populates what would have been an in-place array with just a pointer to an array on heap. Used for relocating the CarsCharacterDialogue array in the CarsDialogue class.
    void __stdcall eh_vector_constructor_iterator_HeapHook(void* cars_character_dialogue_destination, int elemsize, int numelems, void* constructor, void* destructor)
    {
        void** dest_ptr = (void**)cars_character_dialogue_destination;
        memset(dest_ptr, 0x69, elemsize * numelems); // Used for debugging purposes.
        *dest_ptr = calloc(numelems, elemsize);

        char* curr_elem = (char*)*dest_ptr;

        for (int x = 0; x < numelems; x++) {
            ((void(__thiscall*)(void*))constructor)(curr_elem);
            curr_elem += elemsize;
        }

        Logging::Log("[CarsDialogue::CarsDialogue] Created CarsCharacterDialogue array on heap at pointer: 0x%p.\n", dest_ptr);
    }

    // Destroys our custom-ly allocated CarsCharacterDialogue array in the CarsDialogue class's destructor.
    void __stdcall eh_vector_destructor_iterator_HeapHook(void* cars_character_dialogue_array_ptr, int elemsize, int numelems, void* destructor)
    {
        void* actual_start_of_array = *(void**)cars_character_dialogue_array_ptr;

        char* curr_elem = (char*)actual_start_of_array;

        for (int x = 0; x < numelems; x++) {
            ((void(__thiscall*)(void*))destructor)(curr_elem);
            curr_elem += elemsize;
        }

        free(actual_start_of_array);

        Logging::Log("[CarsDialogue::~CarsDialogue] Destroyed heap-allocated CarsCharacterDialogue array.\n");
    }

    __declspec(naked) void CarsDialogue_Create_LEA_PATCH() {
        __asm {
            // lea edi,[esi + 0x120]
            mov edi, [esi + 0x114]
            add edi, 0xC
            ret
        }
    }
    
    __declspec(naked) void CarsDialogue_LoadDialogue_LEA_PATCH() {
        __asm {
            // lea ecx, [eax * 0x8]
            // sub ecx, eax
            // lea esi, [edi + ecx * 4 + 0x114]
            imul ecx, eax, 0x1C
            mov esi, [edi + 0x114]
            add esi, ecx
            ret
        }
    }

    __declspec(naked) void CarsDialogue_Reset_LEA_PATCH() {
        __asm {
            // lea ebx, [esi + 0x114]
            mov ebx, [esi + 0x114]
            ret
        }
    }

    __declspec(naked) void CarsDialogue_unk_LEA_PATCH() {

        __asm {
            // lea ecx, [eax * 0x8]
            // sub ecx, eax
            // lea eax, [esi + ecx * 4 + 0x114]
            imul ecx, eax, 0x1C
            mov eax, [esi + 0x114]
            add eax, ecx
            ret
        }
    }

    __declspec(naked) void CarsDialogue_unk9_LEA_PATCH() {

        __asm {
            // lea ecx, [eax * 0x8]
            // sub ecx, eax
            // lea eax, [edi + ecx * 4 + 0x114]
            imul ecx, eax, 0x1C
            mov eax, [edi + 0x114]
            add eax, ecx
            ret
        }
    }

    __declspec(naked) void CarsDialogue_unk10_LEA_PATCH() {

        __asm {
            // lea ecx, [eax * 0x8]
            // sub ecx, eax
            // lea eax, [ebx + ecx * 4 + 0x114]
            imul ecx, eax, 0x1C
            mov eax, [ebx + 0x114]
            add eax, ecx
            ret
        }
    }

    __declspec(naked) void CarsDialogue_unk2_LEA_PATCH() {
        __asm {
            // lea ebx, [edi + 0x11c]
            mov ebx, [edi + 0x114]
            add ebx, 0x8
            ret
        }
    }

    __declspec(naked) void CarsDialogue_unk3_4_LEA_PATCH() {
        __asm {
            // lea edi, [ebx + 0x114]
            mov edi, [ebx + 0x114]
            ret
        }
    }

    __declspec(naked) void CarsDialogue_unk5_LEA_PATCH() {
        __asm {
            // lea edi, [ebx + 0x118]
            mov edi, [ebx + 0x114]
            add edi, 0x4
            ret
        }
    }

    __declspec(naked) void CarsDialogue_unk6_LEA_PATCH() {

        __asm {
            // lea edi, [ebx + 0x11C]
            mov edi, [ebx + 0x114]
            add edi, 0x8
            ret
        }
    }

    __declspec(naked) void CarsDialogue_unk7_LEA_PATCH() {

        __asm {
            // lea ecx, [esi * 0x8]
            // sub ecx, esi
            // lea esi, [ebx + ecx * 4 + 0x114]
            imul ecx, esi, 0x1C
            mov esi, [ebx + 0x114]
            add esi, ecx
            ret
        }
    }

    __declspec(naked) void CarsDialogue_unk8_LEA_PATCH() {

        __asm {
            // lea ecx, [eax * 0x8]
            // sub ecx, eax
            // cmp [ebx + ecx * 0x4 + 0x118], 0x0
            // push ebp
            // lea ebp, [ebx + ecx * 0x4 + 0x114]

            imul eax, 0x1C
            mov ecx, [ebx + 0x114]
            cmp dword ptr [ecx + eax + 0x4], 0x0
            ret
        }
    }

    int GetNumberOfCars() {
        std::ifstream dialogue_txt(FileSystem::GetPathForFile("c\\audio\\dialogue\\dialogue.txt"), std::ios::in);
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

    void install() {
        // SetExecuteReadWritePermission(CarsDialogue_Constructor_Address + 0x3F, 1);
        // SetExecuteReadWritePermission(CarsDialogue_Destructor_Address + 0x7D, 1);
        // SetExecuteReadWritePermission(CarsDialogue_Create_Address + 0x5BE, 1);

        // *(unsigned char*)(CarsDialogue_Constructor_Address + 0x3F) = MAX_NUMBER_OF_CARS;
        // *(unsigned char*)(CarsDialogue_Destructor_Address + 0x7D) = MAX_NUMBER_OF_CARS;
        // *(unsigned char*)(CarsDialogue_Create_Address + 0x5BE) = MAX_NUMBER_OF_CARS;

        //*(unsigned char*)0x00482B6F = MAX_NUMBER_OF_CARS;

        
        HookFunction(0x00482D61, &CarsDialogue_LoadDialogue_LEA_PATCH, 16, FunctionHookType::InlineReplacement); //
        HookFunction(0x00482EA8, &CarsDialogue_Reset_LEA_PATCH, 6, FunctionHookType::InlineReplacement); //
        HookFunction(0x004C1FF1, &CarsDialogue_Create_LEA_PATCH, 6, FunctionHookType::InlineReplacement);

        HookFunction(0x0048319E, &CarsDialogue_unk_LEA_PATCH, 16, FunctionHookType::InlineReplacement); //
        HookFunction(0x004C2BB3, &CarsDialogue_unk_LEA_PATCH, 16, FunctionHookType::InlineReplacement); // PlayOuchDialogue
        HookFunction(0x004C310E, &CarsDialogue_unk_LEA_PATCH, 16, FunctionHookType::InlineReplacement); //
        HookFunction(0x004EED4A, &CarsDialogue_unk_LEA_PATCH, 16, FunctionHookType::InlineReplacement); //

        HookFunction(0x004EECBB, &CarsDialogue_unk_LEA_PATCH, 16, FunctionHookType::InlineReplacement); //

        HookFunction(0x004F4CAC, &CarsDialogue_unk_LEA_PATCH, 16, FunctionHookType::InlineReplacement); //
        HookFunction(0x004F4C51, &CarsDialogue_unk_LEA_PATCH, 16, FunctionHookType::InlineReplacement); //

        HookFunction(0x0043E251, &CarsDialogue_unk2_LEA_PATCH, 6, FunctionHookType::InlineReplacement); //

        HookFunction(0x004E9D6B, &CarsDialogue_unk3_4_LEA_PATCH, 6, FunctionHookType::InlineReplacement); //
        HookFunction(0x004E9EE3, &CarsDialogue_unk3_4_LEA_PATCH, 6, FunctionHookType::InlineReplacement); //

        HookFunction(0x0043E06C, &CarsDialogue_unk5_LEA_PATCH, 6, FunctionHookType::InlineReplacement); //


        HookFunction(0x004C2161, &CarsDialogue_unk6_LEA_PATCH, 6, FunctionHookType::InlineReplacement); //

        HookFunction(0x004C2194, &CarsDialogue_unk7_LEA_PATCH, 16, FunctionHookType::InlineReplacement); //

        HookFunction(0x004C2965, &CarsDialogue_unk8_LEA_PATCH, 0x19, FunctionHookType::InlineReplacement); // PlayBumpDialogue
        // memset((DWORD*)0x004C2965, 0x90, 0x19);
        // WriteCALL(0x004C2965, &CarsDialogue_unk8_LEA_PATCH);
        *(DWORD*)0x004C296A = 0x012C8D55; // push ebp // lea ebp, [ecx + eax]

        HookFunction(0x004C2A95, &CarsDialogue_unk8_LEA_PATCH, 0x19, FunctionHookType::InlineReplacement); // PlayWreckDialogue
        // memset((DWORD*)0x004C2A95, 0x90, 0x19);
        // WriteCALL(0x004C2A65, &CarsDialogue_unk8_LEA_PATCH);
        *(DWORD*)0x004C2A6A = 0x012C8D55; // push ebp // lea ebp, [ecx + eax]

        HookFunction(0x004C2C95, &CarsDialogue_unk8_LEA_PATCH, 0x19, FunctionHookType::InlineReplacement); // PlayOutOfControlDialogue
        // memset((DWORD*)0x004C2C95, 0x90, 0x19);
        // WriteCALL(0x004C2C65, &CarsDialogue_unk8_LEA_PATCH);
        *(DWORD*)0x004C2C6A = 0x012C8D55; // push ebp // lea ebp, [ecx + eax]

        HookFunction(0x004C2DCD, &CarsDialogue_unk9_LEA_PATCH, 16, FunctionHookType::InlineReplacement); // PlayPassDialogue
        HookFunction(0x004C2F0D, &CarsDialogue_unk9_LEA_PATCH, 16, FunctionHookType::InlineReplacement); // PlayPassedDialogue
        HookFunction(0x004C3026, &CarsDialogue_unk9_LEA_PATCH, 16, FunctionHookType::InlineReplacement); // 

        HookFunction(0x004EEF15, &CarsDialogue_unk10_LEA_PATCH, 16, FunctionHookType::InlineReplacement);
        
        WriteCALL(0x00482B93, &eh_vector_constructor_iterator_HeapHook);
        WriteCALL(0x00482CE6, &eh_vector_destructor_iterator_HeapHook);

        Logging::Log("[CarsDialoguePatches] Succesfully moved CarsCharacterDialogue array!\n");

    }


}

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
    GameText* __fastcall GameText_Create(GameText* this_ptr, void* in_EDX, char* name) {

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

    void __fastcall GameText_LoadGameText(GameText* this_ptr, void* in_EDX, int param_1) {
        return;
    }
#pragma optimize( "", on )

    void install() {
        HookFunction(GameText_Create_Address, &GameText_Create, 7, FunctionHookType::EntireReplacement);
        HookFunction(GameText_LoadGameText_Address, &GameText_LoadGameText, 6, FunctionHookType::EntireReplacement);
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

    void install() {
        if (ConfigManager::IsWidescreenEnabled) {
            Logging::Log("[WideScreenPatches::install] Setting screen mode to widescreen...\n");

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
    // please excuse how awful this is written
    // i didnt have any better ideas at the time

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

    void install() {

        // yeah so instead of using a normal inline hook that CALLs we have to jank together a fake one using a single JMP instruction
        // messy but hey it works
        SetExecuteReadWritePermission((DWORD*)0x0040538e, 0x14);
        memset((DWORD*)0x0040538e, 0x90, 0x14);
        WriteJMP(0x0040538e, &CalculateLoadingScreenPath);
    }

};
