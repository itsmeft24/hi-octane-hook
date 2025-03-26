#pragma once
#include "cars2_activity_info.hpp"

struct VehicleInformation {
	char vehicle_name[32];
	char character_paint_job[32];
	char character_wheel_set[32];
};

enum class UnkType : std::uint32_t {
	FrontEndUI = 0,
	StoryMode,
	Unk,
	Race,
	VersusRace
};

enum class SelectedEventType : std::uint32_t {
	Race = 0,
	Minigame,
	RamoneShopOrLuigiGarage
};

class Cars2Launcher {
public:
	UnkType incoming_unk_type;
	SelectedEventType incoming_selected_event_type;
	ActivityType activity_type;
	char activity_name[64];
	int player_count;
	VehicleInformation vehicle_infos[2];
	VehicleInformation vehicle_infos2[6];
	int needs_character_select_screen;
	char* current_event_in_explore_hub;
public:
	static inline Cars2Launcher* GetInstance() {
		return *reinterpret_cast<Cars2Launcher**>(0x006fe480);
	}
};

static_assert(sizeof(Cars2Launcher) == 856);