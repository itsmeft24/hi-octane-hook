#pragma once
#include "x360_game.hpp"
#include "cars_audio_manager.hpp"
#include "cars2_event_database.hpp"
#include "cars2_story_manager.hpp"

struct CheatCode {
    int index;
    char name[20];
    int unk;
};

class CarsGame : public X360Game {
public:
    char unk[128];
    struct CarsProfile* profile;
    struct CarsGlobalProfile* global_profile;
    struct CarsRecordLibrary* hall_of_fame_library;
    struct CarsRecordLibrary* personal_records_library;
    CarsAudioManager* audio_manager;
    struct CarsActivityManager* activity_manager;
    struct CarsWorld* world;
    struct CarsUI* ui;
    struct VideoFXManager* video_fx_manager;
    struct CarsBonusContentManager* bonus_content_manager;
    struct CarsControllerSettings* controller_settings;
    struct CarsSettings* settings;
    struct CarsBoltManager* bolt_manager;
    struct Cars2PickupManager* pickup_manager;
    struct CarsScavengerHuntManager* scavanger_hunt_manager;
    struct Cars2BonusPointManager* bonus_point_manager;
    struct CarsAchievementManager* achievement_manager;
    struct Cars2SceneDatabase* scene_database;
    struct Cars2ActivityDatabase* activity_database;
    Cars2EventDatabase* event_database;
    struct Cars2VehicleDatabase* vehicle_database;
    Cars2StoryManager* story_manager;
    struct Cars2EventJoinPointManager* event_join_point_manager;
    struct Cars2UIResourceManager* ui_resource_manager;
    GameObject* unused;
    char scene[64];
    char scene_name[64];
    char activity[64];
    char start[64];
    char unused2[64];
    char splash_screen[64];
    char player_start[64];
    char ai_names[64];
    int unused3;
    int unused4;
    int no_321;
    int total_laps;
    int use_vehicle_engine_sound;
    int play_music;
    int build_spline_mode;
    int brake_when_resting;
    int loop_cutscenes;
    int loading_tip_preview;
    float brake_when_resting_threshold;
    char char_name[32];
    char char2_name[32];
    int char_number;
    unsigned int char_paint_type;
    int vehicle_manual_transmission;
    int number_of_players;
    int number_of_ai;
    int unused5;
    int load_ai;
    unsigned int viewport_type;
    int analog_gas;
    int show_coords;
    int dump_results;
    int unused6;
    unsigned int control_type;
    unsigned int camera_type;
    enum DeferredLoadEnum deferred_load;
    int world_paused;
    int unlock_all;
    int force_stub_mode;
    int unlock_all_abilities;
    int unlock_all_bonus_content;
    int unused7;
    int enforce_free_camera_speed;
    int unused8;
    int cinema_mode;
    struct X360LoadingIcon* loading_icon;
    char loading_screen_name[260];
    char loading_icon_name[260];
    float char_color1[4];
    float char_color2[4];
    int game_mode;
    unsigned int difficulty;
    unsigned int time_of_day;
    int starting_bolt_banners;
    int starting_bonus_points;
    int always_show_gps;
    int show_lap_timer;
    CheatCode cheat_codes[11];
    int unused9;
public:
    virtual int InitializeRenderer(const char*) override;
    virtual int Initialize() override;
    virtual int PreGameInitialize(DisplayMode*) override;
    virtual void LoadConfigFile(ParameterBlock*) override;
    virtual void BeginAutoTest() override;
    virtual void EndAutoTest(int) override;
    virtual int Tick() override;
    virtual int KeyUpHandler(ProcessNode*, KeyInfo*) override;
    virtual int KeyDownHandler(ProcessNode*, KeyInfo*) override;
    virtual int Terminate() override;
    virtual void ReportMemoryUsage(char*) override;
    virtual void UpdateLocalizedPaths() override;
    virtual void UpdateTextureContentDirectories() override;
    virtual void ReloadMaterials();
    virtual void ReloadMaterialsAndTextures();

    // void PresentFrame(int);
};

inline CarsGame** lpCarsGame = reinterpret_cast<CarsGame**>(0x00718a74);

static_assert(sizeof(CarsGame) == 0xae0);