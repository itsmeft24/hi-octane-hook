#pragma once
#include "serializable_interface.hpp"

class Cars2BumperSticker : public SerializableInterface {
public:
	int index;
	int material_index;
	int unlocked;
public:
	inline virtual int Serialize(void* buffer, int len) override {
		*reinterpret_cast<int*>(buffer) = unlocked;
		return 4;
	}
	inline virtual int DeSerialize(void* buffer, int len) override {
		unlocked = *reinterpret_cast<int*>(buffer);
		return 4;
	}
	inline virtual int GetSerializedDataSize() override {
		return 0;
	}
};

static_assert(sizeof(Cars2BumperSticker) == 16);