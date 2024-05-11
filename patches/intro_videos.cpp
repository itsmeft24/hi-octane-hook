#include "core/config.hpp"
#include "core/globals.hpp"
#include "core/logging.hpp"
#include "core/hooking/framework.hpp"
#include "intro_videos.hpp"

DefineInlineHook(CarsUIVideoLogoPage_StartPage) {
	static void __cdecl callback(hooking::InlineCtx& ctx) {
		ctx.edx.pointer = config::g_IntroVideos[ctx.ecx.unsigned_integer].data();
	}
};

DeclareFunction(void, __thiscall, CarsUIMenuPage_HandleOK, 0x00516840, uintptr_t, int);
DeclareFunction(void, __thiscall, Unk_PlayMovie, 0x0049f8e0, uintptr_t, char*, char*);


DefineReplacementHook(CarsUIVideoLogoPage_Unk) {
    static void __fastcall callback(struct CarsUIVideoLogoPage* this_) {
        int& movie_index = *reinterpret_cast<int*>(uintptr_t(this_) + 0xb8);
        movie_index++;
        if (movie_index < config::g_IntroVideos.size()) {
            if (*reinterpret_cast<int*>(uintptr_t(this_) + 0xb0) == 0) {
                *reinterpret_cast<int*>(uintptr_t(this_) + 0xb0) = 1;
            }
            auto unk = *reinterpret_cast<uintptr_t*>(uintptr_t(this_) + 0xb4);
            Unk_PlayMovie(unk, config::g_IntroVideos[movie_index].data(), reinterpret_cast<char*>(0x006f6eb0));
            *reinterpret_cast<int*>(uintptr_t(this_) + 0xbc) = 0;
            return;
        }
        CarsUIMenuPage_HandleOK(uintptr_t(this_), 1);
        *reinterpret_cast<int*>(uintptr_t(this_) + 0xbc) = 0;
        return;
    }
};

void intro_videos::install()
{
    CarsUIVideoLogoPage_StartPage::install_at_ptr(0x0049facc);
    CarsUIVideoLogoPage_Unk::install_at_ptr(0x0049fb50);
    logging::log("[intro_videos::install] Successfully installed patch!");
}

