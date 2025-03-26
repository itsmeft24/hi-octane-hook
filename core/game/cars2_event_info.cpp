#include "cars2_event_info.hpp"
#include "string_block_allocator.hpp"
#include "cars2_event_database.hpp"

Cars2EventInfo::Cars2EventInfo()
{
	activity_name = nullptr;
	default_status = 0;
	status = 0;
	flags = 0;
	activity_info = nullptr;
	name_string_id = nullptr;
	frontend_preview_image_material = nullptr;
	ingame_preview_image_material = nullptr;
	event_set = nullptr;
	unk = 0;
}

Cars2EventInfo::~Cars2EventInfo() {
	if (activity_name != nullptr) {
		(*lpGlobalEventDatabase)->string_block_allocator->FreeString(activity_name);
	}
	if (this->name_string_id != nullptr) {
		(*lpGlobalEventDatabase)->string_block_allocator->FreeString(name_string_id);
	}
	if (this->frontend_preview_image_material != nullptr) {
		(*lpGlobalEventDatabase)->string_block_allocator->FreeString(frontend_preview_image_material);
	}
	if (this->ingame_preview_image_material != nullptr) {
		(*lpGlobalEventDatabase)->string_block_allocator->FreeString(ingame_preview_image_material);
	}
}

int Cars2EventInfo::Serialize(void* buffer, int len) {
	*reinterpret_cast<int*>(buffer) = status;
	*reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(buffer) + 4) = flags;
	return 8;
}

int Cars2EventInfo::DeSerialize(void* buffer, int len) {
	status = *reinterpret_cast<int*>(buffer);
	flags = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(buffer) + 4);
	return 8;
}

int Cars2EventInfo::GetSerializedDataSize() {
	return 0;
}