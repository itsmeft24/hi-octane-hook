
#include "../Globals.h"
#include "../HookFunction.h"
#include "../Logging.h"

#include "DataAccessLogging.h"

DeclareFunction(int, __thiscall, DataAccess_FOpen, 0x005D34F0, void*, const char*, const char*);
DeclareFunction(int, __thiscall, DataAccess_LoadResourceFile, 0x005D2FC0, void*, char*, int, int, int, int, int, int, int, int, int);
DeclareFunction(int, __thiscall, DataAccess_FindVirtualFile, 0x00580FD0, void*, char*);

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

void DataAccessLogging::Install() {
	fopen_finfo = HookFunction((void*&)DataAccess_FOpen, &DataAccess_FOpenHook, 6, FunctionHookType::EntireReplacement);

	loadresourcefile_finfo = HookFunction((void*&)DataAccess_LoadResourceFile, &DataAccess_LoadResourceFileHook, 5, FunctionHookType::EntireReplacement);

	if (fopen_finfo.type != FunctionHookType::Invalid && loadresourcefile_finfo.type != FunctionHookType::Invalid)
		Logging::Log("[DataAccessLogging::Install] Successfully installed patch!\n");
}

void DataAccessLogging::Uninstall() {
	UninstallFunctionHook(fopen_finfo);
	UninstallFunctionHook(loadresourcefile_finfo);
}
