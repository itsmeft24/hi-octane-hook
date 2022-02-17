#pragma once
// Here is where some function addresses and some of the games' globals will be stored.
// Other times, they will be defined in their hook-specific namespaces.
// We will add more as more functions in the game are identified.

DWORD* lpDataAccess = (DWORD*)0x0071561C;

DWORD* CarsActivity_AddNameToDialogueList_Address = (DWORD*)0x004B68B0;
DWORD* CarsActivityUI_RequestDialogue_Address = (DWORD*)0x004BD360;
DWORD* CarsDialogue_Create_Address = (DWORD*)0x004C1A20;
DWORD* CarsDialogue_Constructor_Address = (DWORD*)0x00482B30;
DWORD* CarsDialogue_Destructor_Address = (DWORD*)0x00482C50;
DWORD* ParameterBlock_OpenFile_Address = (DWORD*)0x005D87C0;
DWORD* MotionLibrary_LoadMotionFromMOT_Address = (DWORD*)0x005D6CF0;
DWORD* DataAccess_FCLOSE_Address = (DWORD*)0x005A9AF0;
DWORD* DataAccess_FOPEN_Address = (DWORD*)0x005D34F0;
DWORD* DataAccess_FREAD_Address = (DWORD*)0x005C0E90;

DWORD* _fopen = (DWORD*)0x0063FCBF;

#define DECL_FUNCTION(ret_type, call_conv, name, offset, ...) auto name = (ret_type(call_conv*)(##__VA_ARGS__))(offset)
