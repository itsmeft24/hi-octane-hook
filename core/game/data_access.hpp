#pragma once
#include "base_object.hpp"
#include "container_hash_table.hpp"
#include "string_block_allocator.hpp"

struct VirtualDataDevice {
    int id;
    char* file_name;
    int offset;
    std::uint32_t flag_data;
    void* file_pointer;
    std::uint32_t size;
    int primary_data_offset;
    int sector_list_offset;
    std::uint32_t size_of_sector_list;
    std::uint8_t original_key;
};

struct VirtualDataFile {
    int device_id;
    int cache_id;
    int resource_handle;
    int start_offset;
    int current_offset;
    char* file_name;
    std::uint32_t size;
    std::uint32_t flag_data;
    void* resource_data;
};

struct VirtualFileCache {
    std::uint8_t* cache_data;
    int resource_handle;
    int offset;
};

struct VirtualDeviceCache {
    std::uint8_t** cache_buffer;
    std::uint32_t* page_sizes;
    int device_id;
    std::uint32_t size;
    int first_sector;
    int last_sector;
    std::int16_t* sector_size_list;
};

struct ResMonitorData {
    int device_id;
    std::uint32_t time_start;
    std::uint32_t mem_used_start;
    int num_cache_updates;
    int cache_updates[128][4];
};

class DataAccess : public BaseObject {
private:
	std::uint32_t flags;
	int next_available_device;
    VirtualDataDevice* device_list;
    int number_of_devices_in_list;
    VirtualDataDevice* active_device;
    int device_stow[3];
    int* first_device_stow;
    int* last_device_stow;
    VirtualDeviceCache device_cache;
    int num_device_cache_pages;
    VirtualFileCache file_cache_list[8];
    int next_available_file_cache_id;
    ContainerHashTable<char*, int>* file_list;
    VirtualDataFile** file_lookup_list;
    int file_lookup_list_num_slots;
    int last_get_data_or_file_handle;

    // the game checks if these are != 0, but they never get set to anything other than 0
    int unk1;
    int unk2;
    void* unk_vtable;

    // all of this goes completely unused
    int report_bytes_read_uncached;
    int report_bytes_read_device_cache;
    int report_bytes_copied_to_device_cache;
    int report_bytes_copied_to_user;
    int report_bytes_decompressed;
    ResMonitorData resource_monitor;

    // not this though!!!!!
    StringBlockAllocator* string_block_allocator;
public:
    int FindVirtualFile(char*);
};

static_assert(sizeof(VirtualDataDevice) == 40);
static_assert(sizeof(VirtualDataFile) == 36);
static_assert(sizeof(VirtualFileCache) == 12);
static_assert(sizeof(VirtualDeviceCache) == 28);
static_assert(sizeof(ResMonitorData) == 2064);
static_assert(sizeof(DataAccess) == 2296);