#include "cars2_story_event.hpp"

int Cars2StoryEvent::Serialize(void* buffer, int len) {
	*reinterpret_cast<int*>(buffer) = static_cast<int>(status);
	*reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(buffer) + 4) = bolt_banners[0].earned;
	*reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(buffer) + 8) = bolt_banners[1].earned;
	*reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(buffer) + 12) = bolt_banners[2].earned;
	return 16;
}

int Cars2StoryEvent::DeSerialize(void* buffer, int len) {
	status = static_cast<StoryEventStatusType>(*reinterpret_cast<int*>(buffer));
	bolt_banners[0].earned = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(buffer) + 4);
	bolt_banners[1].earned = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(buffer) + 8);
	bolt_banners[2].earned = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(buffer) + 12);
	return 16;
}

int Cars2StoryEvent::GetSerializedDataSize() {
	return 0;
}