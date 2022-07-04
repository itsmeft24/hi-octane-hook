#pragma once
#include <Windows.h>

struct InlineContext // Inspired by skyline. This is the interface for which inline detour hooks may interact with/modify the registers with.
{
	DWORD EAX;
	DWORD ECX;
	DWORD EDX;
	DWORD EBX;
	DWORD ESP;
	DWORD EBP;
	DWORD ESI;
	DWORD EDI;
};
