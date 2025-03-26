#include "cars2_story_database.hpp"

int Cars2StoryDatabase::Serialize(void* buffer, int len) {
	int written = 0;
	for (int i = 0; i < story_events_len; i++) {
		written += story_events[i].Serialize(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(buffer) + written), len - written);
	}
	return written;
}

int Cars2StoryDatabase::DeSerialize(void* buffer, int len) {
	int read = 0;
	for (int i = 0; i < story_events_len; i++) {
		read += story_events[i].DeSerialize(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(buffer) + read), len - read);
	}
	return read;
}

int Cars2StoryDatabase::GetSerializedDataSize() {
	return 0;
}
