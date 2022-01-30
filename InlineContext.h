#pragma once

struct InlineContext // Inspired by skyline. Although not necessary to get information from the registers, it is useful for restoring things. It also looks a little cleaner!
{
	DWORD EAX;
	DWORD EBX;
	DWORD ECX;
	DWORD EDX;
	DWORD ESI;
	DWORD EDI;
	DWORD EBP;
	DWORD ESP;
	// DWORD EIP;
};

InlineContext ctx{}; // Initialize global inline context.

__declspec(naked) void GetInlineContext()
{
	__asm
	{
		mov ctx.EAX, eax
		mov ctx.EBX, ebx
		mov ctx.ECX, ecx
		mov ctx.EDX, edx
		mov ctx.ESI, esi
		mov ctx.EDI, edi
		mov ctx.EBP, ebp
		mov ctx.ESP, esp
		// mov ctx.EIP, eip
		ret
	}
}

__declspec(naked) void SetInlineContext()
{
	__asm
	{
		mov eax, ctx.EAX
		mov ebx, ctx.EBX
		mov ecx, ctx.ECX
		mov edx, ctx.EDX
		mov esi, ctx.ESI
		mov edi, ctx.EDI
		mov ebp, ctx.EBP
		mov esp, ctx.ESP
		// mov eip, ctx.EIP
		ret
	}
}
