#pragma once
#include "serializable_interface.hpp"
#include "cars2_event_info.hpp"

class Cars2StoryEvent : public SerializableInterface {
public:
	enum class StoryEventStatusType : std::uint32_t {
		Locked = 1,
		Unlocked,
		Failed,
		Placed,
		First,
		CutsceneFinished
	};

	class ActionScriptInfo {
	public:
		char* name;
		struct ActionScript* script;
	};

	struct BoltBannerStruct {
		int available;
		int earned;
	};

	StoryEventStatusType status;
	char* story_event_name;
	BoltBannerStruct bolt_banners[3];
	Cars2EventInfo* event_info;
	StoryEventStatusType default_status;
	ActionScriptInfo** on_completion_scripts;
	ActionScriptInfo* quit_script;
	ActionScriptInfo* continue_script;
	int on_critical_path;
public:
	virtual int Serialize(void* buffer, int len) override;
	virtual int DeSerialize(void* buffer, int len) override;
	virtual int GetSerializedDataSize() override;
};

static_assert(sizeof(Cars2StoryEvent::StoryEventStatusType) == 4);
static_assert(sizeof(Cars2StoryEvent::ActionScriptInfo) == 8);
static_assert(sizeof(Cars2StoryEvent::BoltBannerStruct) == 8);
static_assert(sizeof(Cars2StoryEvent) == 60);
