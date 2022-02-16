#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>

const char DLL_PATH[MAX_PATH] = "hi-octane-hook.dll";

int main(int argc, char*argv[]) {
	// Here, we load our DLL with DONT_RESOLVE_DLL_REFERENCES so we can grab a couple exported variables and functions.
	HMODULE hMod = LoadLibraryExA(DLL_PATH, NULL, DONT_RESOLVE_DLL_REFERENCES);
	void* entry_point = GetProcAddress(hMod, "HiOctaneEntry");
	void* exit_point = GetProcAddress(hMod, "HiOctaneExit");
	char** ver_str_imported = (char**)GetProcAddress(hMod, "VERSION");
	char** required_hi_octane_version = (char**)GetProcAddress(hMod, "REQUIRE_HI_OCTANE_VERSION");
	char VERSION_STRING_CPY[MAX_PATH];
	char REQUIRED_HI_OCTANE_VERSION_CPY[MAX_PATH];
	strcpy(VERSION_STRING_CPY, *ver_str_imported);
	strcpy(REQUIRED_HI_OCTANE_VERSION_CPY, *required_hi_octane_version);
	
	// get HO version and cars hook from github api
	// check if we need to update and if config.ini will let us
	// do that if we can
	
	// Free our DLL.
	FreeLibrary(hMod);
	
	// Start the game and get a handle to the process.
	STARTUPINFOA sInfoA{};
	sInfoA.cb = sizeof(STARTUPINFOA);

	PROCESS_INFORMATION pInfo{};

	CreateProcessA("Cars_Mater.exe", "Cars_Mater.exe -windowed", nullptr, nullptr, FALSE, CREATE_SUSPENDED, nullptr, nullptr, &sInfoA, &pInfo);

	// Allocate memory in the game for the name of our dll.
	void* allocated_str = VirtualAllocEx(pInfo.hProcess, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	WriteProcessMemory(pInfo.hProcess, allocated_str, DLL_PATH, MAX_PATH, 0);
	
	// Force the game to spawn a thread with LoadLibraryA as an entry point, passing our allocated string pointer.
	HANDLE dll_loader_thread = CreateRemoteThread(pInfo.hProcess, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, allocated_str, 0, 0);

	WaitForSingleObject(dll_loader_thread, INFINITE);
	VirtualFreeEx(pInfo.hProcess, allocated_str, NULL, MEM_RELEASE);
	CloseHandle(dll_loader_thread);
	// Wait for DLL to be injected before executing its entry point.

	hMod = GetModuleHandleA(DLL_PATH); // Get new module handle in Mater-National's address space.

	HANDLE ho_thread_handle = CreateRemoteThread(pInfo.hProcess, 0, 0, (LPTHREAD_START_ROUTINE)entry_point, nullptr, 0, 0); // Call HiOctaneEntry
	WaitForSingleObject(ho_thread_handle, INFINITE);
	CloseHandle(ho_thread_handle);
	// Wait for entry function to finish before resuming the game's main thread.

	ResumeThread(pInfo.hThread); // Resume game's main thread.

	WaitForSingleObject(pInfo.hThread, INFINITE); // Wait for game's main thread to finish before executing cleanup.
	CloseHandle(pInfo.hThread);

	HANDLE ho_exit_thread_handle = CreateRemoteThread(pInfo.hProcess, 0, 0, (LPTHREAD_START_ROUTINE)exit_point, nullptr, 0, 0); // Call HiOctaneExit
	WaitForSingleObject(ho_exit_thread_handle, INFINITE);
	CloseHandle(ho_exit_thread_handle);
	// Call exit thread and wait for it to finish before closing the handle.
	
	CloseHandle(hMod); // Close the handle to our DLL.
	
	CloseHandle(pInfo.hProcess); // Close the handle to the game.
	return 0;
}


