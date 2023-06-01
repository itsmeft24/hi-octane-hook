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

DefineInlineHook(SetExplorePlaylist) {
    static void __cdecl callback(hooking::InlineContext ctx) {
        Music* music = (*lpCarsGame)->carsAudioManager->music;
        Music_SetCurrentPlaylist(music, 13);
        music->vtbl->Shuffle(music, 1);
    }
};

void explore_music::install() {
	SetExplorePlaylist::install_at_ptr(0x004eb861);
	logging::log("[explore_music::install] Successfully installed patch!");
};