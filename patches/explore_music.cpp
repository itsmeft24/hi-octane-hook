#include "core/globals.hpp"
#include "core/hooking/framework.hpp"
#include "core/logging.hpp"

#include "explore_music.hpp"

struct Music {
    struct {
        void* f[41];
        std::uint32_t(__thiscall* Shuffle)(Music*, int);
    } *vtbl;
};
struct CarsAudioManager {
    char padding[0x40];
    Music* music;
};

struct X360Game {
    char padding[0x440];
    CarsAudioManager* carsAudioManager;
};

DeclareFunction(void, __thiscall, Music_SetCurrentPlaylist, 0x00574af0, Music*, int);

X360Game** lpCarsGame = (X360Game**)0x00718a74;
DWORD* musicfix = (DWORD*)0x004978ee;
DWORD* onlymusicexception = (DWORD*)0x004979e7;

DefineInlineHook(SetExplorePlaylist) {
    static void __cdecl callback(hooking::InlineContext ctx) {
        Music* music = (*lpCarsGame)->carsAudioManager->music;
        Music_SetCurrentPlaylist(music, 13);
        music->vtbl->Shuffle(music, 1);
    }
};

__declspec(naked) void MusicPauseFix() {

    __asm {
        mov eax, [lpCarsGame]
        mov eax, [eax]
        mov ecx, dword ptr[eax + 0x444]
        mov edx, dword ptr[ecx + 0x60]
        mov eax, [edx]
        cmp eax, 0x40
        jz byebyetest
        jmp musicfix
        byebyetest :
        jmp onlymusicexception
    }
}

void explore_music::install() {
	SetExplorePlaylist::install_at_ptr(0x004eb861);
 	hooking::write_nop(0x004978ca, 0x24);
 	hooking::write_jmp(0x004978ca, MusicPauseFix);
	logging::log("[explore_music::install] Successfully installed patch!");
};
