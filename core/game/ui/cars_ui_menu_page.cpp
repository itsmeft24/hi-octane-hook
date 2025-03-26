#include "cars_ui_menu_page.hpp"

CarsUIMenuPage::CarsUIMenuPage() : CarsUIPage(), UIInputEventListener()
{
	title_text_id_index = -1;
	nav_text_id_indices[0] = -1;
	nav_text_id_indices[1] = -1;
	nav_text_id_indices[2] = -1;
	nav_text_id_indices[3] = -1;
	deferred_load = -1;
	pause_when_up = 0;
	pause_music_on_start = 0;
	visible_if_no_animation_present = 0;
	fade_tick_timer = 0;
	key_up = new UIInputEvent();
	key_down = new UIInputEvent();
	is_pages_popup = 0;
}

CarsUIMenuPage::~CarsUIMenuPage()
{
	if (key_down != nullptr) {
		delete key_down;
	}
	if (key_up != nullptr) {
		delete key_up;
	}
}

__declspec(naked) void CarsUIMenuPage::CreateFromFile(char*, ParameterBlock*) {
	UNIMPLEMENTED(0x00496180);
}

__declspec(naked) void CarsUIMenuPage::Render()
{
	UNIMPLEMENTED(0x00454190);
}

__declspec(naked) void CarsUIMenuPage::Tick(float dt)
{
	UNIMPLEMENTED(0x00454200);
}

__declspec(naked) void CarsUIMenuPage::StartPage()
{
	UNIMPLEMENTED(0x00496340);
}

__declspec(naked) void CarsUIMenuPage::EndPage()
{
	UNIMPLEMENTED(0x004cee60);
}

__declspec(naked) void CarsUIMenuPage::RegisterEventListeners()
{
	UNIMPLEMENTED(0x00454470);
}

__declspec(naked) void CarsUIMenuPage::DeRegisterEventListeners()
{
	UNIMPLEMENTED(0x004544e0);
}

__declspec(naked) void CarsUIMenuPage::Unk_PlayAnim(int is_full_on_animation)
{
	UNIMPLEMENTED(0x004542b0);
}

__declspec(naked) void CarsUIMenuPage::Unk_SetVisible()
{
	UNIMPLEMENTED(0x00454300);
}

__declspec(naked) void CarsUIMenuPage::Unk_EndLayer()
{
	UNIMPLEMENTED(0x00454340);
}

__declspec(naked) void CarsUIMenuPage::Unk_DisableMenu(int)
{
	UNIMPLEMENTED(0x00454370);
}

void CarsUIMenuPage::Unk_EnableMenu()
{
}

__declspec(naked) void CarsUIMenuPage::Unk_ExitMenu(char*, int)
{
	UNIMPLEMENTED(0x004543d0);
}

void CarsUIMenuPage::Unk2(int unk)
{
}

void CarsUIMenuPage::Unk3(int unk)
{
}

__declspec(naked) void CarsUIMenuPage::Unk_UpdateText()
{
	UNIMPLEMENTED(0x004cee10);
}

__declspec(naked) void CarsUIMenuPage::HandleOk(int _advanced_or_retracted)
{
	UNIMPLEMENTED(0x00516840);
}

__declspec(naked) void CarsUIMenuPage::HandleBack(int _advanced_or_retracted)
{
	UNIMPLEMENTED(0x004545e0);
}

__declspec(naked) void CarsUIMenuPage::ProcessKeydown(UIInputEvent*)
{
	UNIMPLEMENTED(0x00463030);
}

__declspec(naked) void CarsUIMenuPage::ProcessKeyup(UIInputEvent*)
{
	UNIMPLEMENTED(0x004545b0);
}
