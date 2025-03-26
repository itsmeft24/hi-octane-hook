#pragma once
#include "game_object.hpp"

class CarsDialogue : public GameObject, StreamingListener {
public:
    struct StreamingListener listener;
    struct StringBlockAllocator* allocator;
    struct X360SoundGroup* sound_group;
    int character_count;
    // Time required to pass before a line can repeat.
    float pass_line_buffer;
    float passed_line_buffer;
    float excited_line_buffer;
    float surprise_line_buffer;
    float idle_line_buffer;
    float greet_line_buffer;
    float prompt_line_buffer;
    float bump_line_buffer;
    float hit_line_buffer;
    float wreck_line_buffer;
    float out_of_control_line_buffer;
    float hang_time_line_buffer;
    float ouch_line_buffer;
    // Time since each type of line was played.
    float pass_line_timer;
    float passed_line_timer;
    float excited_line_timer;
    float surprise_line_timer;
    float idle_line_timer;
    float greet_line_timer;
    float prompt_line_timer;
    float bump_line_timer;
    float hit_line_timer;
    float wreck_line_timer;
    float out_of_control_line_timer;
    float hang_time_line_timer;
    float ouch_line_timer;
    // Chance that each type of line will be played.
    float play_pass_dialogue_chance;
    float play_pass_car_specific_dialogue_chance;
    float play_passed_dialogue_chance;
    float play_passed_car_specific_dialogue_chance;
    float play_wreck_dialogue_chance;
    float play_bump_dialogue_chance;
    float play_surprise_dialogue_chance;
    float play_hit_dialogue_chance;
    float play_excited_dialogue_chance;
    float play_greet_dialogue_chance;
    float play_hang_time_dialogue_chance;
    float play_ouch_dialogue_chance;
    float play_prompt_dialogue_chance;
    float unk_extra_excited_dialogue_chance;
    float play_rev_announcer_dialogue_chance;
    float play_race_announcer_dialogue_chance;
    float play_guido_announcer_dialogue_chance;
    float play_ramone_announcer_dialogue_chance;
    float min_wreck_impulse;
    float max_wreck_impulse;
    float min_bump_impulse;
    float max_bump_impulse;
    float min_out_of_control_impulse;
    float max_out_of_control_impulse;
    float dialogue_volume_min_distance;
    float dialogue_volume_max_distance;
    float reletive_velocity_threshold_for_passing;
    int should_stop_emitter;
    int can_stop_emitter;
    struct CarsDialogueSoundEmitterInfo* sound_emitter_info;
    struct CarsCharacterDialogue characters[80];
    int streaming;
    char in_dialogue_motion[64];
    struct CarsDialogueInfo info;
	struct CarsCharacterDialogue characters_patched[127];
};


struct CarsCharacterDialogue {
    struct CarsDialogueGroup* dialogue_groups;
    char* name;
    char* aud_name;
    int requested_component;
    struct StreamingListener** listener;
    struct StringBlockAllocator* allocator;
    virtual ~CarsCharacterDialogue();
};

struct CarsDialogueInfo {
    struct CarsDialogueInterface* interface;
    struct CarsDialogueInterface* field2_0x8;
    struct CarsDialogueLine* current_line;
    int field4_0x10;
    int field5_0x14;
    int field6_0x18;
    void* field7_0x1c;
    int field8_0x20;
    int unk_buffer;
    virtual ~CarsDialogueInfo();
};