#pragma once
#include "container_list.hpp"
#include "container_linked_list.hpp"
#include "game_object.hpp"

class Music : GameObject {
public:
	struct Playlist {
		char* name;
		ContainerList<int> songs;
	};
private:
	Playlist* selected_playlist;
	ContainerList<Playlist*> playlists;
	// ...
public:
	inline virtual ~Music() override {
		UNIMPLEMENTED(0x005bf160);
	}
	inline virtual int Tick(float dt) override {
	}
	inline virtual int Pause(int paused) override {
	}
	inline virtual void Create(int param_1, int param_2) {}
	inline virtual void Play(int song, int param_3) {}
	inline virtual bool IsPlaying() {}
	inline virtual void Repeat(int param_1, int param_2) {}
	inline virtual void Loop(int param_1, int param_2) {}
	inline virtual bool IsPaused() {}
	inline virtual int Stop(float param_1) {}
	inline virtual int SetVolume(int param_1) {}
	inline virtual int RampVolume(int param_1, int param_2, int param_3) {}
	inline virtual int GetCurrentTrack() {}
	inline virtual int Shuffle(int param_1) {}
	inline virtual int ShuffleStartingAt(int param_1, int param_2) {}
	inline virtual float GetCurrentElapsedTime() {}
	inline virtual int GetAdditionalNumberOfJukeboxTracks() {
		return 0;
	}
	inline void SetCurrentPlaylistIndex(int index) {
		selected_playlist = playlists.GetItem(index);
	}
	inline Playlist* GetPlaylist(int index) {
		return playlists.GetItem(index);
	}
	inline std::size_t GetPlaylistIndex(const std::string& str) {
		for (std::size_t index = 0; index < playlists.Length(); index++) {
			const auto& playlist = *playlists[index];
			if (str == playlist.name) {
				return index;
			}
		}
		return -1;
	}
	inline std::size_t GetNumPlaylists() {
		return playlists.Length();
	}
};

class X360Music : public Music {
public:
	inline virtual ~X360Music() override {
		UNIMPLEMENTED(0x0041b120);
	}
};

struct CarsAudioManager {
	char padding[0x40];
	X360Music* music;
};

struct X360Game {
	char padding[0x440];
	CarsAudioManager* carsAudioManager;
};

inline X360Game** lpCarsGame = reinterpret_cast<X360Game**>(0x00718a74);