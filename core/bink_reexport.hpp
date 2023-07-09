#pragma once
struct BINK;
extern void* BINKCLOSE;
extern void* BINKCOPYTOBUFFERRECT;
extern void* BINKDOFRAME;
extern void* BINKGETRECTS;
extern void* BINKNEXTFRAME;
extern void* BINKOPEN;
extern void* BINKOPENDIRECTSOUND;
extern void* BINKPAUSE;
extern void* BINKSETSOUNDSYSTEM;
extern void* BINKSETSOUNDTRACK;
extern void* BINKSETVOLUME;
extern void* BINKSHOULDSKIP;
extern void* BINKWAIT;
namespace bink_reexport {
	void load();
	void unload();
};