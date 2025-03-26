#include "core/globals.hpp"
#include "cars_ui_menu_layer.hpp"

DeclareFunction(void, __thiscall, UIFlashAnimationManager_Deregister, 0x005b9110, uintptr_t, char*);

CarsUIMenuLayer::CarsUIMenuLayer() : CarsBaseLayer()
{
	flags |= 0x1100;
	anim_state = 0;
	full_anim = nullptr;
	partial_anim = nullptr;
}

CarsUIMenuLayer::~CarsUIMenuLayer()
{
	uintptr_t lp_carsui = *reinterpret_cast<uintptr_t*>(0x006fe590);
	uintptr_t ui_flash_animation_manager = *reinterpret_cast<uintptr_t*>(*reinterpret_cast<uintptr_t*>(lp_carsui + 0x24) + 0x2c);

	if (full_anim != nullptr) {
		UIFlashAnimationManager_Deregister(ui_flash_animation_manager, full_anim);
		operator_delete(full_anim);
	}
	if (partial_anim != nullptr) {
		UIFlashAnimationManager_Deregister(ui_flash_animation_manager, partial_anim);
		operator_delete(partial_anim);
	}
	// UNIMPLEMENTED(0x00453b60);
}

__declspec(naked) void CarsUIMenuLayer::CreateFromFile(char*, ParameterBlock*)
{
	UNIMPLEMENTED(0x00453c30);
}

__declspec(naked) void CarsUIMenuLayer::PlayAnim0(int is_full_on)
{
	UNIMPLEMENTED(0x00453d60);
}

__declspec(naked) void CarsUIMenuLayer::PlayAnim1(int is_full_on)
{
	UNIMPLEMENTED(0x00453da0);
}

void CarsUIMenuLayer::Unk1()
{
}

void CarsUIMenuLayer::Unk2()
{
}

__declspec(naked) void CarsUIMenuLayer::SetVisibleIfNotPlayingAnim1()
{
	UNIMPLEMENTED(0x00496fe0);
}

__declspec(naked) void CarsUIMenuLayer::SetVisibleIfNotPlayingAnim2()
{
	UNIMPLEMENTED(0x00496fe0);
}

char* CarsUIMenuLayer::GetSelectedOption()
{
	return nullptr;
}
