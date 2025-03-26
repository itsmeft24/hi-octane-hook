#include "core/globals.hpp"
#include "ui_layer.hpp"

DeclareFunction(void*, __thiscall, UIStyleSheet_Destructor, 0x0060ff50, void*);


UILayer::UILayer() : components()
{
	container = nullptr;
	name = nullptr;
	style_sheet = nullptr;
	layer_unk = 0;
	layer_unk2 = 0x10;
}

UILayer::~UILayer()
{
	for (auto& component : components) {
		component->ResetMouseMenu();
	}
	for (auto& component : components) {
		delete component;
	}

	UIStyleSheet_Destructor(style_sheet);
	operator_delete(style_sheet);

	operator_delete(name);
	
	// UNIMPLEMENTED(0x00625be0);
}

__declspec(naked) void UILayer::CreateFromFile(char*, ParameterBlock*)
{
	UNIMPLEMENTED(0x00623860);
}

__declspec(naked) void UILayer::CreateFromName(char*)
{
	UNIMPLEMENTED(0x00625ca0);
}

__declspec(naked) void UILayer::Tick(float dt)
{
	UNIMPLEMENTED(0x00623950);
}

__declspec(naked) void UILayer::Render(int)
{
	UNIMPLEMENTED(0x00623970);
}

void UILayer::StartLayer()
{
}

void UILayer::EndLayer()
{
}

__declspec(naked) void UILayer::StartComponents()
{
	UNIMPLEMENTED(0x00627990);
}

__declspec(naked) void UILayer::EndComponents()
{
	UNIMPLEMENTED(0x006279b0);
}

void UILayer::RegisterEventListeners()
{
	container->RegisterEventListeners();
}

void UILayer::DeRegisterEventListeners()
{
	container->DeRegisterEventListeners();
}

void UILayer::PageStackStateChanged(PageStackState)
{

}

void UILayer::SetEnabled(int enabled)
{
	container->SetEnabled(enabled);
}

int UILayer::IsEnabled()
{
	return container->enabled;
}

void UILayer::SetVisible(int visible)
{
	container->SetVisible(visible);
}

int UILayer::IsVisible()
{
	return container->visible;
}

__declspec(naked) void UILayer::SetLayout(UILayout*)
{
	UNIMPLEMENTED(0x00625d60);
}

__declspec(naked) void UILayer::FUN_004699c0()
{
	UNIMPLEMENTED(0x004699c0);
}

void UILayer::Repaint(int)
{
	container->Repaint();
}

int UILayer::CursorClicked(std::uint32_t u_msg, int button, int x, int y)
{
	return 3;
}

void UILayer::CursorSelected(std::uint32_t u_msg, int button, int x, int y)
{
	return;
}
