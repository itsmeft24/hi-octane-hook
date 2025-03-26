#include <array>
#include <iostream>
#include <unordered_map>

#include "core/config.hpp"
#include "core/game/cars_game.hpp"
#include "core/game/cars2_activity_info.hpp"
#include "core/game/cars2_event_database.hpp"
#include "core/game/music.hpp"
#include "sunset/sunset.hpp"
#include "core/logging.hpp"
#include "patches/playlist_events.hpp"

DeclareFunction(int, __thiscall, X360SoundGroup_StopAllSounds, 0x0041b5b0, std::uintptr_t);

DefineInlineHook(KillMusicBeforeCutScene) {
	static void __cdecl callback(sunset::InlineCtx& ctx) {
		X360SoundGroup_StopAllSounds(std::uintptr_t((*lpCarsGame)->audio_manager->music_group));
	}
};

std::array<std::string_view, 12> original_playlist_order = {
	"Road Races",
	"Stadium Races",
	"Rustbucket Races",
	"Tractor Tipping",
	"GhostLight",
	"Fuel Up Countdown",
	"Relay Races",
	"Time Trial",
	"Race and Chase",
	"Waypoint",
	"Last Laugh",
	"FrontEnd"
};

DefineReplacementHook(X360Music_SetCurrentPlaylist) {
	static void __fastcall callback(X360Music * _this, uintptr_t edx, int playlist_number) {
		// If we end up in this hook, then the caller MUST be the original game and NOT ourselves.
		auto playlist_index = playlist_number - 1;
		// If the requested playlist is one of the playlists part of the original game, then we want to get its name so we can later get its real index.
		if (playlist_index < original_playlist_order.size()) {
			std::string desired_playlist(original_playlist_order[playlist_index]);
			playlist_index = _this->GetPlaylistIndex(desired_playlist);
			if (playlist_index == -1) {
				logging::log("[X360Music::SetCurrentPlaylist] Failed to set playlist to: {} as it was not found in music.pb!", desired_playlist);
				return;
			}
		}

		// If the sanitized playlist index is somehow out of bounds the user's music.pb file is probably cooked...
		if (playlist_index >= _this->GetNumPlaylists()) {
			logging::log("[X360Music::SetCurrentPlaylist] Failed to set playlist index to: {} as it was out of bounds! (Check to make sure your music.pb isnt missing anything!)", playlist_index);
			return;
		}

		logging::log("[X360Music::SetCurrentPlaylist] Setting playlist to: {}.", _this->GetPlaylist(playlist_index)->name);
		_this->SetCurrentPlaylistIndex(playlist_index);
	}
};


DefineInlineHook(SetExplorePlaylist) {
	static void __cdecl callback(sunset::InlineCtx & ctx) {
		X360Music* music = (*lpCarsGame)->audio_manager->music;
		if (!music->IsPlaying()) {
			auto playlist_index = music->GetPlaylistIndex("Explore");
			if (playlist_index == -1) {
				logging::log("[CarsActivityExplore::Start] Failed to set playlist to: Explore as it was not found in music.pb!");
				return;
			}
			logging::log("[CarsActivityExplore::Start] Setting playlist to: Explore.");
			music->SetCurrentPlaylistIndex(playlist_index);
			music->Shuffle(1);
		}
	}
};

DefineInlineHook(SetGhostLightPlaylist) {
	static void __cdecl callback(sunset::InlineCtx & ctx) {
		X360Music* music = (*lpCarsGame)->audio_manager->music;
		if (!music->IsPlaying()) {
			auto playlist_index = music->GetPlaylistIndex("GhostLight");
			if (playlist_index == -1) {
				logging::log("[CarsActivityMinigameGhostLight::Start] Failed to set playlist to: GhostLight as it was not found in music.pb!");
				return;
			}
			logging::log("[CarsActivityMinigameGhostLight::Start] Setting playlist to: GhostLight.");
			music->SetCurrentPlaylistIndex(playlist_index);
			music->Shuffle(1);
		}
	}
};

DefineInlineHook(SetTractorTippingPlaylist) {
	static void __cdecl callback(sunset::InlineCtx & ctx) {
		X360Music* music = (*lpCarsGame)->audio_manager->music;
		if (!music->IsPlaying()) {
			auto playlist_index = music->GetPlaylistIndex("Tractor Tipping");
			if (playlist_index == -1) {
				logging::log("[CarsActivityMinigameTractorTipping::Start] Failed to set playlist to: Tractor Tipping as it was not found in music.pb!");
				return;
			}
			logging::log("[CarsActivityMinigameTractorTipping::Start] Setting playlist to: Tractor Tipping.");
			music->SetCurrentPlaylistIndex(playlist_index);
			music->Shuffle(1);
		}
	}
};

DefineReplacementHook(CarsActivityRaceRoad_PlayMusic) {
	static void __fastcall callback(struct CarsActivityRaceRoad* _this) {
		uintptr_t cars_activity_manager = reinterpret_cast<uintptr_t>((*lpCarsGame)->activity_manager);
		Cars2ActivityInfo* activity_info = *reinterpret_cast<Cars2ActivityInfo**>(cars_activity_manager + 0x60);
		if (activity_info != nullptr) {
			Cars2EventInfo* event_info = (*lpGlobalEventDatabase)->GetEventInfo(activity_info);
			if (event_info != nullptr) {
				Cars2EventSet* event_set = event_info->event_set;
				std::string race_type = event_set->event_set_name;
				if (config::g_RaceTypeToPlaylist.contains(race_type)) {
					const auto& playlist = config::g_RaceTypeToPlaylist.at(race_type);
					std::size_t playlist_index = (*lpCarsGame)->audio_manager->music->GetPlaylistIndex(playlist);
					if (playlist_index == -1) {
						logging::log("[CarsActivityRaceRoad::PlayMusic] Failed to set playlist to: {} as it was not found in music.pb!", playlist);
						return;
					}

					logging::log("[CarsActivityRaceRoad::PlayMusic] Setting playlist to: {}.", playlist);
					(*lpCarsGame)->audio_manager->music->SetCurrentPlaylistIndex(playlist_index);
					(*lpCarsGame)->audio_manager->music->Shuffle(1);
				}
				else {
					logging::log("[CarsActivityRaceRoad::PlayMusic] Encountered race type: {} which does not have an assigned playlist!", race_type);
				}
			}
		}
	}
};

DefineReplacementHook(CarsActivityUI_PlayMusic) {
	static void __fastcall callback(struct CarsActivityUI* _this) {
		X360Music* music = (*lpCarsGame)->audio_manager->music;
		if (!music->IsPlaying()) {
			std::size_t frontend_index = music->GetPlaylistIndex("FrontEnd");
			Music::Playlist* frontend = music->GetPlaylist(frontend_index);
			int song = frontend->songs.GetItem(std::rand() % frontend->songs.Length());

			logging::log("[CarsActivityUI::PlayMusic] Setting playlist to: FrontEnd.");
			music->SetCurrentPlaylistIndex(frontend_index);
			music->Loop(song, 1);
		}
	}
};

void playlist_events::install() {
	CarsActivityUI_PlayMusic::install_at_ptr(0x00431770);
	CarsActivityRaceRoad_PlayMusic::install_at_ptr(0x00430f90);
	X360Music_SetCurrentPlaylist::install_at_ptr(0x00574af0);

	SetGhostLightPlaylist::install_at_ptr(0x0053550e);
	SetTractorTippingPlaylist::install_at_ptr(0x004ee41c);

	if (config::g_ExploreMusicEnabled) {
		SetExplorePlaylist::install_at_ptr(0x004eb861);
		KillMusicBeforeCutScene::install_at_ptr(0x004ea891);

		// Allows the song selection UI elements to show up during explore activities
		sunset::inst::nop(reinterpret_cast<void*>(0x004978da), 11);
		sunset::inst::nop(reinterpret_cast<void*>(0x00419e8d), 2);
	}
	logging::log("[playlist_events::install] Successfully installed patch!");
}