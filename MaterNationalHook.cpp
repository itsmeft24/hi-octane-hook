#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <Windows.h>

#include "InlineHook32.h"
#include "InlineContext.h"
#include "offsets.h"
#include "Hooks.h"

void ParameterBlockOpenFileHook() {
    
    GetInlineContext();

    char* FileName = (char*)ctx.ESI; // Create variable and assign it to the ESI value retreived from GetInlineContext.

    printf("[Logging::ParameterBlock::OpenFile] Opened file: %s\n", FileName);

    // SetInlineContext();
}

// Unfinished!
void MOT_LoaderHook() {

    GetInlineContext();

    char* FileName = (char*)ctx.EDI; // Create variable and assign it to the EDI value retreived from GetInlineContext.

    std::string FileName_STR(FileName);
    FileName_STR += ".got";

    std::ifstream MOTFile(FileName_STR, std::ios::in | std::ios::binary);

    int MOT_Version = 0;

    MOTFile.read((char*)&MOT_Version, 4);

    MOTFile.close();

    if (MOT_Version == -117440513) { // Version 8
        printf("[Patches::MotionLibrary::LoadMotionFromMOT] Converting %s to Version -7...\n", FileName);
        // Convert animation to version 7
        std::string cmd = "python anim_backporter.py " + FileName_STR + " " + FileName_STR + ".backported";
        system(cmd.c_str());
        strcat(FileName, ".backported");
        // Concatenate extension directly to old pointer so that LoadMotionFromMOT loads the correct file.
        // This way, we dont need to modify the registers!
    }
    else if (MOT_Version == -100663297) { // Version 7
        // Do nothing
    }
    else if (MOT_Version == -7) { // Version 7 MOT (Little Endian)
        printf("[Patches::MotionLibrary::LoadMotionFromMOT] Byteswapping Animation %s...\n", FileName);
        // Byteswap animation file
        std::string cmd = "python byteswap_animation.py " + FileName_STR + " " + FileName_STR + ".byteswapped";
        system(cmd.c_str());
        strcat(FileName, ".byteswapped");
        // Concatenate extension directly to old pointer so that LoadMotionFromMOT loads the correct file.
        // This way, we dont need to modify the registers!
    }
}

DWORD WINAPI main(HMODULE hModule)
{
    // Create Console
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    SetConsoleTitleA("cars mater nation for wii u");

    InlineHook32(ParameterBlock_OpenFile + 0x16, 7, &ParameterBlockOpenFileHook); // Some simple hooks will be installed in our main thread like so.
    
    // CarsActivityUI_RequestDialogueHook::install(); // Others will be held in their own namespace, and their code will be in Hooks.h.

    while (true)
    {
        // MainLoop

        if (GetAsyncKeyState(VK_END) & 1)
        {
            break;
        }
    }

    // Unhook our hooks. (UNIMPLEMENTED!)

	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
    return 0;

}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {

    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 4096, (LPTHREAD_START_ROUTINE)main, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;

}