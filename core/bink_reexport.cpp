
#include <cstdint>
#include <Windows.h>

#include "core/bink_reexport.hpp"

HMODULE binkw32;

void* BINKCLOSE;
void* BINKCOPYTOBUFFERRECT;
void* BINKDOFRAME;
void* BINKGETRECTS;
void* BINKNEXTFRAME;
void* BINKOPEN;
void* BINKOPENDIRECTSOUND;
void* BINKPAUSE;
void* BINKSETSOUNDSYSTEM;
void* BINKSETSOUNDTRACK;
void* BINKSETVOLUME;
void* BINKSHOULDSKIP;
void* BINKWAIT;

namespace bink_reexport {
	void load() {
		binkw32 = LoadLibraryA("binkw32.dll");
		if (binkw32 != nullptr) {
			BINKCLOSE = GetProcAddress(binkw32, "_BinkClose@4");
			BINKCOPYTOBUFFERRECT = GetProcAddress(binkw32, "_BinkCopyToBufferRect@44");
			BINKDOFRAME = GetProcAddress(binkw32, "_BinkDoFrame@4");
			BINKGETRECTS = GetProcAddress(binkw32, "_BinkGetRects@8");
			BINKNEXTFRAME = GetProcAddress(binkw32, "_BinkNextFrame@4");
			BINKOPEN = GetProcAddress(binkw32, "_BinkOpen@8");
			BINKOPENDIRECTSOUND = GetProcAddress(binkw32, "_BinkOpenDirectSound@4");
			BINKPAUSE = GetProcAddress(binkw32, "_BinkPause@8");
			BINKSETSOUNDSYSTEM = GetProcAddress(binkw32, "_BinkSetSoundSystem@8");
			BINKSETSOUNDTRACK = GetProcAddress(binkw32, "_BinkSetSoundTrack@8");
			BINKSETVOLUME = GetProcAddress(binkw32, "_BinkSetVolume@12");
			BINKSHOULDSKIP = GetProcAddress(binkw32, "_BinkShouldSkip@4");
			BINKWAIT = GetProcAddress(binkw32, "_BinkWait@4");
		}
	}

	void unload() {
		if (binkw32 != nullptr) {
			FreeLibrary(binkw32);
		}
	}
};

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkClose(BINK* bink)
{
	__asm jmp BINKCLOSE;
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkCopyToBufferRect(BINK* bink, void* dst, std::int32_t dst_pitch, std::uint32_t dst_height, std::uint32_t dst_x, std::uint32_t dst_y, std::uint32_t src_x, std::uint32_t src_y, std::uint32_t src_w, std::uint32_t src_h, std::uint32_t flags)
{
	__asm jmp BINKCOPYTOBUFFERRECT;
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkDoFrame(BINK* bink)
{
	__asm jmp BINKDOFRAME;
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkGetRects(BINK* bink, std::uint32_t flags)
{
	__asm jmp BINKGETRECTS;
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkNextFrame(BINK* bink)
{
	__asm jmp BINKNEXTFRAME;
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkOpen(const char* path, std::uint32_t flags)
{
	__asm jmp BINKOPEN;
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkOpenDirectSound(std::uint32_t param)
{
	__asm jmp BINKOPENDIRECTSOUND;
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkPause(BINK* bink, std::int32_t pause)
{
	__asm jmp BINKPAUSE;
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkSetSoundSystem(void* open, std::uint32_t param)
{
	__asm jmp BINKSETSOUNDSYSTEM;
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkSetSoundTrack(std::uint32_t total_tracks, std::uint32_t* tracks)
{
	__asm jmp BINKSETSOUNDTRACK;
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkSetVolume(BINK* bink, std::uint32_t track_id, std::int32_t volume)
{
	__asm jmp BINKSETVOLUME;
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkShouldSkip(BINK* bink)
{
	__asm jmp BINKSHOULDSKIP;
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkWait(BINK* bink)
{
	__asm jmp BINKWAIT;
}