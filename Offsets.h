#pragma once
// Here is where some function addresses and some of the games' globals will be stored.
// Other times, they will be defined in their hook-specific namespaces.
// We will add more as more functions in the game are identified.

DWORD* lpCarsGame = (DWORD*)0x00718A74;
DWORD* lpDataAccess = (DWORD*)0x0071561C;

DWORD* CarsActivity_AddNameToDialogueList_Address = (DWORD*)0x004B68B0;
DWORD* CarsActivityUI_RequestDialogue_Address = (DWORD*)0x004BD360;
DWORD* CarsDialogue_Create_Address = (DWORD*)0x004C1A20;
DWORD* CarsDialogue_Constructor_Address = (DWORD*)0x00482B30;
DWORD* CarsDialogue_Destructor_Address = (DWORD*)0x00482C50;
DWORD* ParameterBlock_OpenFile_Address = (DWORD*)0x005D87C0;
DWORD* ParameterBlock_Constructor_Address = (DWORD*)0x00587E60;
DWORD* MotionLibrary_LoadMotionFromMOT_Address = (DWORD*)0x005D6CF0;
DWORD* DataAccess_FClose_Address = (DWORD*)0x005A9AF0;
DWORD* DataAccess_FOpen_Address = (DWORD*)0x005D34F0;
DWORD* DataAccess_FRead_Address = (DWORD*)0x005C0E90;
DWORD* DataAccess_LoadResourceFile_Address = (DWORD*)0x005D2FC0;
DWORD* DataAccess_FindVirtualFile_Address = (DWORD*)0x00580FD0;
DWORD* GameText_Create_Address = (DWORD*)0x006074E0;
DWORD* GameText_LoadGameText_Address = (DWORD*)0x00607820;

DWORD* _fopen_Address = (DWORD*)0x0063FCBF;
DWORD* _fclose_Address = (DWORD*)0x0063FD94;

#define DeclareFunction(ret_type, call_conv, name, offset, ...) auto name = (ret_type(call_conv*)(##__VA_ARGS__))(offset)

#define SetFunctionOffset(ptr, offset) ptr = (decltype(ptr))(offset)
