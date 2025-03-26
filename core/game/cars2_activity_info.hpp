#pragma once
#include <cstdint>

enum class ActivityType : std::uint32_t {
	Invalid = 0,
	RaceRoad = 0x1,
	RaceRelay = 0x2,
	RaceWaypoint = 0x4,
	RaceFuelFrenzy = 0x8,
	RaceTimeTrial = 0x10,
	GhostingMater = 0x20,
	RythmicRumble = 0x40,
	RaceAndChase = 0x80,
	TractorTipping = 0x100,
	EXplorer = 0x200,
	CutScene = 0x400,
	LastLaugh = 0x800,
	UI = 0x1000,
	RaceBase = 0x2000,
	RaceRustbucket = 0x4000,
	Unused = 0x8000,
};

class Cars2ActivityInfo {
public:
	ActivityType type;
	char* directory;
	char* activity_name;
	char* scene;
	int time_of_day;
	int can_run_during_day_or_night;
	int unk;
public:
	Cars2ActivityInfo();
	~Cars2ActivityInfo();
	Cars2ActivityInfo(const Cars2ActivityInfo&) = delete;
	Cars2ActivityInfo& operator=(const Cars2ActivityInfo&) = delete;
};

static_assert(sizeof(Cars2ActivityInfo) == 28);