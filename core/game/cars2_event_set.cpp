#include "bind.hpp"
#include "cars2_event_set.hpp"
#include "string_block_allocator.hpp"
#include "cars2_event_database.hpp"

Cars2EventSet::Cars2EventSet()
{
	events = nullptr;
	events_len = 0;
	playable_vehicles = nullptr;
	playable_vehicles_len = 0;
	computer_vehicles = nullptr;
	computer_vehicles_len = 0;
	event_set_name = nullptr;
	activity_type = ActivityType::Invalid;
	name_string_id = nullptr;
	unk = 0;
}

Cars2EventSet::~Cars2EventSet()
{
	if (events != nullptr) {
		for (int i = 0; i < events_len; i++) {
			(&events[i])->~Cars2EventInfo();
		}
		operator_delete(events);
	}

	if (playable_vehicles != nullptr) {
		for (int i = 0; i < playable_vehicles_len; i++) {
			(*lpGlobalEventDatabase)->string_block_allocator->FreeString(playable_vehicles[i]);
		}
		operator_delete(playable_vehicles);
	}
	if (computer_vehicles != nullptr) {
		for (int i = 0; i < computer_vehicles_len; i++) {
			(*lpGlobalEventDatabase)->string_block_allocator->FreeString(computer_vehicles[i]);
		}
		operator_delete(computer_vehicles);
	}

	if (event_set_name != nullptr) {
		(*lpGlobalEventDatabase)->string_block_allocator->FreeString(event_set_name);
	}

	if (name_string_id != nullptr) {
		(*lpGlobalEventDatabase)->string_block_allocator->FreeString(name_string_id);
	}
}

int Cars2EventSet::Serialize(void* buffer, int len) {
	int written = 0;
	for (int i = 0; i < events_len; i++) {
		written += events[i].Serialize(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(buffer) + written), len - written);
	}
	return written;
}

int Cars2EventSet::DeSerialize(void* buffer, int len) {
	int read = 0;
	for (int i = 0; i < events_len; i++) {
		read += events[i].DeSerialize(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(buffer) + read), len - read);
	}
	return read;
}

int Cars2EventSet::GetSerializedDataSize() {
	return 0;
}
