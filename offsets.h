#pragma once
// Here is where function addresses will be stored. We will add more as more functions in the game are identified.
DWORD* CarsActivity_AddNameToDialogueList = (DWORD*)0x004B68B0;
DWORD* CarsActivityUI_RequestDialogue = (DWORD*)0x004BD360;
DWORD* CarsDialogue_Create = (DWORD*)0x004C1A20;
DWORD* CarsDialogue_Constructor = (DWORD*)0x00482B30;
DWORD* CarsDialogue_Destructor = (DWORD*)0x00482C50;
DWORD* ParameterBlock_OpenFile = (DWORD*)0x005D87C0;
DWORD* MotionLibrary_LoadMotionFromMOT = (DWORD*)0x005D6CF0;
DWORD* DataAccess_FCLOSE = (DWORD*)0x005A9AF0;
DWORD* DataAccess_FOPEN = (DWORD*)0x005D34F0;
DWORD* DataAccess_FREAD = (DWORD*)0x005C0E90;

#define DECL_FUNCTION(ret_type, call_conv, name, offset, ...) auto name = (ret_type(call_conv*)(##__VA_ARGS__))(offset)

