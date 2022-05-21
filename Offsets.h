#pragma once
// Here is where some function addresses and some of the games' globals will be stored.
// Other times, they will be defined in their hook-specific namespaces.
// We will add more as more functions in the game are identified.

BYTE* lpCarsGame = (BYTE*)0x00718A74;
BYTE* lpDataAccess = (BYTE*)0x0071561C;

#define DeclareFunction(ret_type, call_conv, name, offset, ...) auto name = (ret_type(call_conv*)(##__VA_ARGS__))(offset)


DeclareFunction(void, __thiscall, CarsActivity_AddNameToDialogueList, 0x004B68B0, void*, const char*);
DeclareFunction(void, __thiscall, CarsActivityUI_RequestDialogue, 0x004BD360, void*);

DeclareFunction(int, __thiscall, DataAccess_FOpen, 0x005D34F0, void*, const char*, const char*);
DeclareFunction(int, __thiscall, DataAccess_LoadResourceFile, 0x005D2FC0, void*, char*, int, int, int, int, int, int, int, int, int);
DeclareFunction(int, __thiscall, DataAccess_FindVirtualFile, 0x00580FD0, void*, char*);

DeclareFunction(void*, __thiscall, GameText_Create, 0x006074E0, void*, char*);
DeclareFunction(void, __thiscall, GameText_LoadGameText, 0x006074E0, void*, int);

DeclareFunction(DWORD*, __stdcall, BinkOpen, GetProcAddress(GetModuleHandleA("binkw32.dll"), "_BinkOpen@8"), char*, DWORD);
DeclareFunction(DWORD, __stdcall, BASS_SampleLoad, GetProcAddress(GetModuleHandleA("bass.dll"), "BASS_SampleLoad"), BOOL, char*, DWORD, DWORD, DWORD, DWORD);
DeclareFunction(DWORD, __stdcall, BASS_StreamCreateFile, GetProcAddress(GetModuleHandleA("bass.dll"), "BASS_StreamCreateFile"), BOOL, char*, DWORD, DWORD, DWORD);

DeclareFunction(void*, __cdecl, __fsopen, 0x0063FBFB, char*, char*, int); // VS2005 CRT function
DeclareFunction(void*, __cdecl, _fopen, 0x0063FCBF, char*, char*); // VS2005 CRT function
DeclareFunction(int, __cdecl, _fclose, 0x0063FD94, void*); // VS2005 CRT function


//DWORD* CarsDialogue_Create_Address = (DWORD*)0x004C1A20;
//DWORD* CarsDialogue_Constructor_Address = (DWORD*)0x00482B30;
//DWORD* CarsDialogue_Destructor_Address = (DWORD*)0x00482C50;
// 
//DWORD* ParameterBlock_OpenFile_Address = (DWORD*)0x005D87C0;
//DWORD* ParameterBlock_Constructor_Address = (DWORD*)0x00587E60;
// 
//DWORD* MotionLibrary_LoadMotionFromMOT_Address = (DWORD*)0x005D6CF0;
// 
//DWORD* DataAccess_FClose_Address = (DWORD*)0x005A9AF0;
//DWORD* DataAccess_FOpen_Address = (DWORD*)0x005D34F0;
//DWORD* DataAccess_FRead_Address = (DWORD*)0x005C0E90;
//DWORD* DataAccess_LoadResourceFile_Address = (DWORD*)0x005D2FC0;
//DWORD* DataAccess_FindVirtualFile_Address = (DWORD*)0x00580FD0;
// 
//DWORD* GameText_Create_Address = (DWORD*)0x006074E0;
//DWORD* GameText_LoadGameText_Address = (DWORD*)0x00607820;
//
//DWORD* _fopen_Address = (DWORD*)0x0063FCBF;
//DWORD* _fclose_Address = (DWORD*)0x0063FD94;


#define SetFunctionOffset(ptr, offset) ptr = (decltype(ptr))(offset)
