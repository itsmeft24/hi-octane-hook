#include "cars2_activity_info.hpp"
#include "string_block_allocator.hpp"

Cars2ActivityInfo::Cars2ActivityInfo() {
	type = ActivityType::Invalid;
	directory = nullptr;
	activity_name = nullptr;
	scene = nullptr;
	time_of_day = 0;
	can_run_during_day_or_night = 0;
	unk = 0;
}

Cars2ActivityInfo::~Cars2ActivityInfo() {
	auto global_activity_database = *reinterpret_cast<uintptr_t*>(0x006fe470);
	auto string_block_allocator = *reinterpret_cast<StringBlockAllocator**>(global_activity_database + 8);
	if (directory != nullptr) {
		string_block_allocator->FreeString(directory);
	}
	if (activity_name != nullptr) {
		string_block_allocator->FreeString(activity_name);
	}
	if (scene != nullptr) {
		string_block_allocator->FreeString(scene);
	}
}
