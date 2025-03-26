#pragma once
#include "cars2_event_set.hpp"
#include "string_block_allocator.hpp"

class Cars2EventDatabase {
public:
	Cars2EventSet* event_sets;
	int event_sets_len;
	StringBlockAllocator* string_block_allocator;
public:
	Cars2EventInfo* GetEventInfo(Cars2ActivityInfo* activity);
};

inline Cars2EventDatabase** lpGlobalEventDatabase = reinterpret_cast<Cars2EventDatabase**>(0x006fe474);