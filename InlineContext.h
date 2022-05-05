#pragma once

struct InlineContext // Inspired by skyline. This is the interface for which inline detour hooks may interact with/modify the registers with.
{
	DWORD EAX;
	DWORD EBX;
	DWORD ECX;
	DWORD EDX;
	DWORD ESI;
	DWORD EDI;
	DWORD EBP;
	DWORD ESP;
};
