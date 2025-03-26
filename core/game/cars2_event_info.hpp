#pragma once
#include "serializable_interface.hpp"
#include "cars2_activity_info.hpp"

class Cars2EventInfo : public SerializableInterface {
public:
	int status;
	int flags;
	char* activity_name;
	int default_status;
	Cars2ActivityInfo* activity_info;
	char* name_string_id;
	char* frontend_preview_image_material;
	char* ingame_preview_image_material;
	class Cars2EventSet* event_set;
	int unk;
public:
	Cars2EventInfo();
	~Cars2EventInfo();
	Cars2EventInfo(const Cars2EventInfo&) = delete;
	Cars2EventInfo& operator=(const Cars2EventInfo&) = delete;
	
	virtual int Serialize(void* buffer, int len) override;
	virtual int DeSerialize(void* buffer, int len) override;
	virtual int GetSerializedDataSize() override;
};