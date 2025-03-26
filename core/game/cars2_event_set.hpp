#pragma once
#include "serializable_interface.hpp"
#include "cars2_event_info.hpp"

class Cars2EventSet : public SerializableInterface {
public:
	Cars2EventInfo* events;
	int events_len;
	char* event_set_name;
	ActivityType activity_type;
	char* name_string_id;
	int unk;
	char** playable_vehicles;
	int playable_vehicles_len;
	char** computer_vehicles;
	int computer_vehicles_len;
public:
	Cars2EventSet();
	~Cars2EventSet();
	Cars2EventSet(const Cars2EventSet&) = delete;
	Cars2EventSet& operator=(const Cars2EventSet&) = delete;

	virtual int Serialize(void* buffer, int len) override;
	virtual int DeSerialize(void* buffer, int len) override;
	virtual int GetSerializedDataSize() override;
};

static_assert(sizeof(Cars2EventSet) == 0x2c);