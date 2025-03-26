#pragma once
#include "music.hpp"

struct CarsAudioManager {
	char padding[0x24];
	struct X360SoundGroup* music_group;
	struct X360SoundGroup* sfx_1_group;
	struct X360SoundGroup* sfx_2_group;
	struct X360SoundGroup* unk;
	struct X360SoundGroup* sfx_3_group;
	struct X360SoundGroup* master_group;
	struct X360SoundGroup* dialogue_group;
	X360Music* music;
};