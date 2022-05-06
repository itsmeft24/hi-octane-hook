#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>

const char DLL_PATH[MAX_PATH] = "hi-octane\\c\\modules\\cars-hi-octane.dll";

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

int main(int argc, char*argv[]) {

	// Here, we load our DLL with DONT_RESOLVE_DLL_REFERENCES so we can grab the exported version string.
	HMODULE hMod = LoadLibraryExA(DLL_PATH, NULL, DONT_RESOLVE_DLL_REFERENCES);
	char* version_string_imported = *(char**)GetProcAddress(hMod, "VERSION");
	char VERSION_STRING_CPY[MAX_PATH];
	strcpy(VERSION_STRING_CPY, version_string_imported);
	
	// get latest HO version from github api
	// check if we need to update and let the user know they are running an outdated version
	
	// Free our DLL.
	FreeLibrary(hMod);
	
	// Start the game and get a handle to the process.
	STARTUPINFOA sInfoA{};
	sInfoA.cb = sizeof(STARTUPINFOA);
	PROCESS_INFORMATION pInfo{};
	
	if (argc > 1)
		CreateProcessA("cars_mater.exe", "cars_mater.exe -windowed", nullptr, nullptr, TRUE, CREATE_SUSPENDED, nullptr, nullptr, &sInfoA, &pInfo);
	else
		CreateProcessA("cars_mater.exe", "cars_mater.exe", nullptr, nullptr, TRUE, CREATE_SUSPENDED, nullptr, nullptr, &sInfoA, &pInfo);
	
	// Allocate memory in the game for the name of our dll.
	void* allocated_str = VirtualAllocEx(pInfo.hProcess, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	WriteProcessMemory(pInfo.hProcess, allocated_str, DLL_PATH, MAX_PATH, 0);
	
	// Force the game to spawn a thread with LoadLibraryA as an entry point, passing our allocated string pointer.
	HANDLE dll_loader_thread = CreateRemoteThread(pInfo.hProcess, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, allocated_str, 0, 0);

	WaitForSingleObject(dll_loader_thread, INFINITE);
	VirtualFreeEx(pInfo.hProcess, allocated_str, NULL, MEM_RELEASE);
	CloseHandle(dll_loader_thread);
	// Wait for DLL to be injected before resuming the game's main thread.

	ResumeThread(pInfo.hThread); // Resume game's main thread.
	CloseHandle(pInfo.hThread);
	CloseHandle(pInfo.hProcess); // Close the handles to the game.
	return 0;
}


