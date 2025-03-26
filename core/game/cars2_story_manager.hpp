#pragma once
#include "virtual_network.hpp"
#include "serializable_interface.hpp"
#include "cars2_story_event.hpp"
#include "cars2_bumper_sticker.hpp"
#include "cars2_story_database.hpp"

class Cars2StoryManager : public VNObject, SerializableInterface {
public:
	class VehicleInfo : public SerializableInterface {
	public:
		char vehicle_name[40];
		char character_paint_job[40];
		char character_wheel_set[40];
	public:
		inline virtual int Serialize(void* buffer, int len) override {
			std::memcpy(buffer, &vehicle_name, 40);
			std::memcpy(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(buffer) + 40), &character_paint_job, 40);
			std::memcpy(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(buffer) + 80), &character_wheel_set, 40);
			return 120;
		}

		inline virtual int DeSerialize(void* buffer, int len) override {
			std::memcpy(&vehicle_name, buffer, 40);
			std::memcpy(&character_paint_job, reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(buffer) + 40), 40);
			std::memcpy(&character_wheel_set, reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(buffer) + 80), 40);
			return 120;
		}

		inline virtual int GetSerializedDataSize() override {
			return 0;
		}
	};
	class ExploreHub : public SerializableInterface {
	public:
		char explore_hub_name[40];
		char current_event[40];
		char ambient_vehicle_group[40];
	public:
		inline virtual int Serialize(void* buffer, int len) override {
			std::memcpy(buffer, &current_event, 40);
			std::memcpy(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(buffer) + 40), &ambient_vehicle_group, 40);
			return 80;
		}

		inline virtual int DeSerialize(void* buffer, int len) override {
			std::memcpy(&current_event, buffer, 40);
			std::memcpy(&ambient_vehicle_group, reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(buffer) + 40), 40);
			return 80;
		}

		inline virtual int GetSerializedDataSize() override {
			return 0;
		}
	};
private:
	Cars2StoryDatabase* story_database;
	VehicleInfo vehicles[2];
	int current_vehicle;
	ExploreHub explore_hubs[4];
	int current_explore_hub;
	ContainerList<Cars2BumperSticker*> bumper_stickers;
	int time_of_day;
	char current_story_event_name[40];
	int enable_mcq_horn;
	int mcq_hq_stage;
	Cars2StoryEvent* current_story_event;
public:
};

static_assert(sizeof(Cars2StoryManager::VehicleInfo) == 124);
static_assert(sizeof(Cars2StoryManager::ExploreHub) == 124);
static_assert(sizeof(Cars2StoryManager) == 852);
