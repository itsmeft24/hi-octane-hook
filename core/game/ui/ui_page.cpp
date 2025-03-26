#include "ui_page.hpp"

#include "core/globals.hpp"

DeclareFunction(void*, __thiscall, UIFocusManager_Destructor, 0x00610470, void*);

UIPage::UIPage() : focus_managers()
{
	name = 0;
	layer_count = 0;
	layer_name_indices = nullptr;
	class_name_indices = nullptr;
	should_be_enabled = 0;
	enabled = 0;
	has_registered_event_listeners = 0;
	is_top_of_stack = 0;
	has_top_enabled = 3;
	has_not_top_enabled = 3;
}

UIPage::~UIPage()
{
	if (layer_name_indices != nullptr) operator_delete(layer_name_indices);
	if (class_name_indices != nullptr) operator_delete(class_name_indices);
	if (name != nullptr) operator_delete(name);
	for (auto& elem : focus_managers) {
		UIFocusManager_Destructor(elem);
		operator_delete(elem);
	}
}

__declspec(naked) void UIPage::CreateFromFile(char*, ParameterBlock*)
{
	UNIMPLEMENTED(0x006279f0);
}

__declspec(naked) void UIPage::CreateFromName(char*)
{
	UNIMPLEMENTED(0x00625ed0);
}

void UIPage::Initialize()
{
}

__declspec(naked) void UIPage::Render()
{
	UNIMPLEMENTED(0x006239e0);
}

__declspec(naked) void UIPage::Tick(float dt)
{
	UNIMPLEMENTED(0x00623a40);
}

__declspec(naked) void UIPage::StartPage()
{
	UNIMPLEMENTED(0x00625fa0);
}

__declspec(naked) void UIPage::EndPage()
{
	UNIMPLEMENTED(0x00626100);
}

__declspec(naked) void UIPage::RegisterEventListeners()
{
	UNIMPLEMENTED(0x00623aa0);
}

__declspec(naked) void UIPage::DeRegisterEventListeners()
{
	UNIMPLEMENTED(0x00623b00);
}

__declspec(naked) void UIPage::RegisterFocusManagers()
{
	UNIMPLEMENTED(0x00625f40);
}

__declspec(naked) void UIPage::DeRegisterFocusManagers()
{
	UNIMPLEMENTED(0x00625f70);
}

void UIPage::SetEnabled(int _enabled)
{
	enabled = _enabled;
}

__declspec(naked) void UIPage::SetPageStackState(PageStackState)
{
	UNIMPLEMENTED(0x00623b60);
}

__declspec(naked) void UIPage::Repaint()
{
	UNIMPLEMENTED(0x00623ca0);
}

void UIPage::ReportToDebugOverlay(int, DebugOverlay*)
{
}

__declspec(naked) void UIPage::PageStackStateChanged()
{
	UNIMPLEMENTED(0x00623bd0);
}

void UIPage::Start()
{
}

void UIPage::End()
{
}
