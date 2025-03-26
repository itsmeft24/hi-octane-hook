#pragma once
#include "cars_ui_page.hpp"
#include "ui_event.hpp"
#include "cars_ui_menu_layer.hpp"

enum PageState {
	Created,
	Started,
	PlayingAnim,
	Visible,
	NotVisible,
	NotPlayingAnim
};

class CarsUIMenuPage : public CarsUIPage, UIInputEventListener {
protected:
	int title_text_id_index;
	int nav_text_id_indices[4];
	int nav_ids[4];
	UILayer* navigation_layer;
	UILayer* background_layer;
	CarsUIMenuLayer* associated_layer;
	UIInputEvent* key_down;
	UIInputEvent* key_up;
	int is_pages_popup;
	int fade_tick_timer;
	int advanced_or_retracted; // actually prob an enum but idc
	char* selected_option;
	PageState page_state;
	int allow_game_music;
	int pause_music_on_start;
	UIPage* unk_page;
	int pause_when_up;
	int visible_if_no_animation_present;
	int deferred_load; // fs an enum DeferredLoad but idc
	int is_not_top_of_stack;
public:
	CarsUIMenuPage();
	virtual ~CarsUIMenuPage() override;
	virtual void CreateFromFile(char*, ParameterBlock*) override;
	virtual void Render() override;
	virtual void Tick(float dt) override;
	virtual void StartPage() override;
	virtual void EndPage() override;
	virtual void RegisterEventListeners() override;
	virtual void DeRegisterEventListeners() override;
	
	virtual void Unk_PlayAnim(int is_full_on_animation);
	virtual void Unk_SetVisible();
	virtual void Unk_EndLayer();
	virtual void Unk_DisableMenu(int);
	virtual void Unk_EnableMenu();
	virtual void Unk_ExitMenu(char*, int);
	virtual void Unk2(int);
	virtual void Unk3(int);
	virtual void Unk_UpdateText();
	virtual void HandleOk(int _advanced_or_retracted);
	virtual void HandleBack(int _advanced_or_retracted);

	virtual void ProcessKeydown(UIInputEvent*) override;
	virtual void ProcessKeyup(UIInputEvent*) override;

	REPLACE_OPERATOR_NEW_DELETE
	inline char* GetSelectedOption() {
		return selected_option;
	}
	inline int GetAdvancedOrRetracted() {
		return advanced_or_retracted;
	}
};

static_assert(sizeof(PageState) == 4);
static_assert(sizeof(CarsUIMenuPage) == 0xb4);