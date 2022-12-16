#pragma once
#include <Windows.h>

struct InlineContext // Inspired by skyline. This is the interface for which
                     // inline detour hooks may interact with/modify the
                     // registers with.
{
	DWORD EDI;
	DWORD ESI;
	DWORD EBP;
	DWORD ESP;
	DWORD EBX;
	DWORD EDX;
	DWORD ECX;
	DWORD EAX;
};
