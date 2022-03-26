#pragma once
#include <unordered_map>
#include <Windows.h>
#include <random>

namespace CarsActivityUI_RequestDialogueHook {

    std::vector<std::string> dialogue_list = {
    "mcq",
    "mat",
    "hud",
    "ram",
    "flo", // Added from MN Wii
    "sar",
    "fil",
    "lui", // Added from MN Wii
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

    DECL_FUNCTION(void, __thiscall, AddNameToDialogueList, CarsActivity_AddNameToDialogueList_Address, void*, const char*);

    void __fastcall RequestDialogue(void* this_ptr) {
        for (int x = 0; x < dialogue_list.size(); x++) {
            AddNameToDialogueList(this_ptr, dialogue_list[x].c_str());
            Logging::Log("[CarsActivityUI::RequestDialogue] Added %s to the Dialogue List!\n", dialogue_list[x].c_str());
        }
    }

    bool CollectCharactersToPatch() {
        std::ifstream file(FileSystem::GetPathForFile("c\\global\\chars\\dialoguelist.ini"), std::ios::in);
        if (!file)
            return false;

        std::string line;
        while (std::getline(file, line)) {
            dialogue_list.push_back(line);
        }
        file.close();
        return true;
    }

    bool install() {
        is_installed = CollectCharactersToPatch();
        if (is_installed)
            InstallReplacementHook(CarsActivityUI_RequestDialogue_Address, &RequestDialogue, 0x110, "CarsActivityUI::RequestDialogue");
        return is_installed;
    };

    void uninstall() {
        if (is_installed) {
            UninstallReplacementHook("CarsActivityUI::RequestDialogue");
            is_installed = false;
        }
    };
};

namespace DataAccessLogging {

    DECL_FUNCTION(int, __thiscall, DataAccess_FOpen, DataAccess_FOPEN_Address, void*, char*, char*);

    DECL_FUNCTION(int, __thiscall, DataAccess_LoadResourceFile, 0x005D2FC0, void*, char*, int, int, int, int, int, int, int, int, int);

    DECL_FUNCTION(int, __thiscall, DataAccess_FindVirtualFile, 0x00580FD0, void*, char*);

    int __fastcall DataAccess_FOpenHook(void* this_ptr, void* DUMMY, char* FileName, char* Access) {

        if (DataAccess_FindVirtualFile(this_ptr, FileName) == -1)
            Logging::Log("[DataAccess::FOpen] Attempting to open file from disk: %s...\n", FileName);
        else
            Logging::Log("[DataAccess::FOpen] Attempting to open virtual file: %s...\n", FileName);

        GetFunctionInfo("DataAccess::FOpen").RestoreOriginalFunction();
        auto ret = DataAccess_FOpen(this_ptr, FileName, Access);
        GetFunctionInfo("DataAccess::FOpen").ReinstallReplacementHook();

        return ret;
    }

    int __fastcall DataAccess_LoadResourceFileHook(void* this_ptr, void* DUMMY, char* filename, int encryptedOnly, int userDataType, int userData, int userDataBufferSizeInBytes, int bytesOfUserDataRead, int forceLoad, int forceRead, int param_10, int param_11) {

        Logging::Log("[DataAccess::LoadResourceFile] Attempting to load ResourceFile: %s\n...", filename);

        GetFunctionInfo("DataAccess::LoadResourceFile").RestoreOriginalFunction();
        auto ret = DataAccess_LoadResourceFile(this_ptr, filename, encryptedOnly, userDataType, userData, userDataBufferSizeInBytes, bytesOfUserDataRead, forceLoad, forceRead, param_10, param_11);
        GetFunctionInfo("DataAccess::LoadResourceFile").ReinstallReplacementHook();

        return ret;
    }

    void install() {
        InstallReplacementHook(DataAccess_FOPEN_Address, &DataAccess_FOpenHook, 6, "DataAccess::FOpen");
        InstallReplacementHook(0x005D2FC0, &DataAccess_LoadResourceFileHook, 5, "DataAccess::LoadResourceFile");
    }

    void uninstall() {
        UninstallReplacementHook("DataAccess::FOpen");
        UninstallReplacementHook("DataAccess::LoadResourceFile");
    }
};

namespace RemoveMipMapping {
    DECL_FUNCTION(HRESULT, __stdcall, D3DXCreateTextureFromFileInMemoryEx, GetProcAddress(GetModuleHandleA("d3dx9_34.dll"), "D3DXCreateTextureFromFileInMemoryEx"), void*, LPCVOID, UINT, UINT, UINT, UINT, DWORD, UINT, UINT, DWORD, DWORD, DWORD, void*, PALETTEENTRY*, void*);
    
    DECL_FUNCTION(HRESULT, __stdcall, D3DXCreateCubeTextureFromFileInMemoryEx, GetProcAddress(GetModuleHandleA("d3dx9_34.dll"), "D3DXCreateCubeTextureFromFileInMemoryEx"), void*, LPCVOID, UINT, UINT, UINT, DWORD, UINT, UINT, DWORD, DWORD, DWORD, void*, PALETTEENTRY*, void*);

    HRESULT __stdcall D3DXCreateTextureFromFileInMemoryExHook(void* pDevice, LPCVOID pSrcData, UINT SrcDataSize, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, UINT Format, UINT Pool, DWORD Filter, DWORD MipFilter, DWORD ColorKey, void* pSrcInfo, PALETTEENTRY* pPalette, void* ppTexture) {
        GetFunctionInfo("D3DXCreateTextureFromFileInMemoryEx").RestoreOriginalFunction();
        auto ret = D3DXCreateTextureFromFileInMemoryEx(pDevice, pSrcData, SrcDataSize, Width, Height, 1, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppTexture);
        GetFunctionInfo("D3DXCreateTextureFromFileInMemoryEx").ReinstallReplacementHook();
        return ret;
    }

    HRESULT __stdcall D3DXCreateCubeTextureFromFileInMemoryExHook(void* pDevice, LPCVOID pSrcData, UINT SrcDataSize, UINT Size, UINT MipLevels, DWORD Usage, UINT Format, UINT Pool, DWORD Filter, DWORD MipFilter, DWORD ColorKey, void* pSrcInfo, PALETTEENTRY* pPalette, void* ppCubeTexture) {
        GetFunctionInfo("D3DXCreateCubeTextureFromFileInMemoryEx").RestoreOriginalFunction();
        auto ret = D3DXCreateCubeTextureFromFileInMemoryEx(pDevice, pSrcData, SrcDataSize, Size, 1, Usage, Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppCubeTexture);
        GetFunctionInfo("D3DXCreateCubeTextureFromFileInMemoryEx").ReinstallReplacementHook();
        return ret;
    }

    void install() {
        InstallReplacementHook(GetProcAddress(GetModuleHandleA("d3dx9_34.dll"), "D3DXCreateTextureFromFileInMemoryEx"), &D3DXCreateTextureFromFileInMemoryExHook, 6, "D3DXCreateTextureFromFileInMemoryEx");
        InstallReplacementHook(GetProcAddress(GetModuleHandleA("d3dx9_34.dll"), "D3DXCreateCubeTextureFromFileInMemoryEx"), &D3DXCreateCubeTextureFromFileInMemoryExHook, 6, "D3DXCreateCubeTextureFromFileInMemoryEx");
    }

    void uninstall() {
        UninstallReplacementHook("D3DXCreateTextureFromFileInMemoryEx");
        UninstallReplacementHook("D3DXCreateCubeTextureFromFileInMemoryEx");
    }
}

namespace LargeVehiclePatch {
    // Allows the user to specify which characters should use the Monster Truck CSS Camera animation instead of the default one.

    /*
    BOOL __stdcall IsNotLargeVehicle(char* character) {
        return (strcmp(character, "mcqm") != 0 && strcmp(character, "matm") != 0 && strcmp(character, "sulm") != 0);
    }*/

    BOOL __stdcall IsNotLargeVehicle(char* character) {
        return 0;
    }

    void install() {
        SetExecuteReadWritePermission((void*)0x0050FB1F, 78);
        WriteCALL(0x0050FB1F, &IsNotLargeVehicle); // Here, we artificially offload the large vehicle detection to its own function.
        memset((char*)0x0050FB24, 0x90, 0x36); // NOP out the original code to complete the process.
    }
};


