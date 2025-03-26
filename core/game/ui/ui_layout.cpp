#include "ui_layout.hpp"

UILayout::~UILayout()
{
	for (auto& component_layout : component_layouts) {
		delete component_layout;
	}
	// UNIMPLEMENTED(0x00610340);
}

__declspec(naked) void UILayout::Create()
{
	UNIMPLEMENTED(0x006103f0);
}
