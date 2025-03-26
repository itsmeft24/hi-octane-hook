#pragma once
#include "container_list.hpp"
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
	virtual ~Music() override;
	virtual int Tick(float dt) override = 0;
	virtual int Pause(int paused) override = 0;
	virtual void CreateNumTracks(int param_1, int track_count) = 0;
	virtual void Play(int song, int param_3) = 0;
	virtual bool IsPlaying() = 0;
	virtual void Repeat(int param_1, int param_2) = 0;
	virtual void Loop(int param_1, int param_2) = 0;
	virtual bool IsPaused() = 0;
	virtual int Stop(float param_1) = 0;
	virtual int SetVolume(int param_1) = 0;
	virtual int RampVolume(int param_1, int param_2, int param_3) = 0;
	virtual int GetCurrentTrack() = 0;
	virtual int Shuffle(int param_1) = 0;
	virtual int ShuffleStartingAt(int param_1, int param_2) = 0;
	virtual float GetCurrentElapsedTime() = 0;
	virtual int GetAdditionalNumberOfJukeboxTracks() = 0;

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
	virtual ~X360Music() override;
	virtual int Tick(float dt) override;
	virtual int Pause(int paused) override;
	virtual void CreateNumTracks(int param_1, int track_count) override;
	virtual void Play(int song, int param_3) override;
	virtual bool IsPlaying() override;
	virtual void Repeat(int param_1, int param_2) override;
	virtual void Loop(int param_1, int param_2) override;
	virtual bool IsPaused() override;
	virtual int Stop(float param_1) override;
	virtual int SetVolume(int param_1) override;
	virtual int RampVolume(int param_1, int param_2, int param_3) override;
	virtual int GetCurrentTrack() override;
	virtual int Shuffle(int param_1) override;
	virtual int ShuffleStartingAt(int param_1, int param_2) override;
	virtual float GetCurrentElapsedTime() override;
	virtual int GetAdditionalNumberOfJukeboxTracks() override;
};