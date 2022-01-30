#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <Windows.h>

#include "InlineHook32.h"
#include "InlineContext.h"
#include "offsets.h"
#include "Hooks.h"
#include "ParameterBlockClass.h"

constexpr auto WindowTitle = "Cars: Hi-Octane Helper Plugin";

__declspec(naked) void ParameterBlockOpenFileHook() {

    GetInlineContext();

    __asm
    {
        push ebp
        mov ebp, esp
    }
    // Create stack frame.

    char* FileName;
    FileName = (char*)ctx.EBP;
    // Create variable and assign it to the EBP value retreived from GetInlineContext.
    // This has to take 2 lines due to the way naked functions are compiled.

    printf("[Logging::ParameterBlock::OpenFile] Opened file: %s\n", FileName);

    __asm
    {
        mov esp, ebp
        pop ebp
    }
    // Destroy stack frame.

    RETURN;
}

void FileDiscovery() {
    // Will be used for adding native mod loading support.
}


DWORD WINAPI main(HMODULE hModule)
{
    // Create Console
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    SetConsoleTitleA(WindowTitle);

    InlineHook32(ParameterBlock_OpenFile + 0x34, 5, &ParameterBlockOpenFileHook); // Some simple hooks will be installed in our main thread like so.
    
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
