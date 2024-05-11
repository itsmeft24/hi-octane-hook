#include "ui.hpp"

UIComponent::UIComponent() {
	parent_container = nullptr;
	transform = DirectX::XMMatrixIdentity();
	name = nullptr;
	visible = 1;
	enabled = 1;
	focusable = 1;
	focused = 0;
	selectable = 1;
	selected = 0;
	parent_mouse_menu = nullptr;
}

UIComponent::~UIComponent() {
	free(name);
	parent_mouse_menu = nullptr;
}

void UIComponent::Create(char* new_name) {
	std::size_t len = strlen(new_name);
	name = reinterpret_cast<char*>(malloc(len + 1));
	memcpy(name, new_name, len);
	name[len] = 0;
	transform = DirectX::XMMatrixIdentity();
}

void UIComponent::StartComponent() {}

void UIComponent::EndComponent() {}

int UIComponent::IsContainer() {
	return 0;
}

UIComponent* UIComponent::GetComponent(char* component) {
	if (stricmp(name, component) != 0) {
		return nullptr;
	}
	return this;
}

void UIComponent::GetDimensions(float* x, float* y) {
	*x = 0;
	*y = 0;
}

void UIComponent::SetDimensions(float* x, float* y) {}

void UIComponent::GetExtents(__RECT* extents) {
	extents->x = 0;
	extents->y = 0;
	extents->w = 0;
	extents->h = 0;
}

void UIComponent::GetExtentsRelative(__RECT* extents) {
	this->GetExtents(extents);
	if (parent_container != nullptr) {
		float parent_x = 0.0f;
		float parent_y = 0.0f;
		parent_container->GetAbsolutePosition(&parent_x, &parent_y);
		extents->x -= parent_x;
		extents->w -= parent_x;
		extents->y -= parent_y;
		extents->h -= parent_y;
	}
}

float UIComponent::GetWidth() {
	return 0.0f;
}

float UIComponent::GetHeight() {
	return 0.0f;
}

void UIComponent::GetPosition(float* x, float* y) {
	*x = 0;
	*y = 0;
}

void UIComponent::SetPosition(float x, float y) {}

void UIComponent::GetAbsolutePosition(float* x, float* y) {
	this->GetPosition(x, y);
	if (parent_container != nullptr) {
		float parent_x = 0.0f;
		float parent_y = 0.0f;
		parent_container->GetAbsolutePosition(&parent_x, &parent_y);
		*x += parent_x;
		*y += parent_y;
	}
}

void UIComponent::SetAngle(float angle) {}

void UIComponent::PropogateChange() {
	if (parent_container != nullptr) {
		parent_container->PropogateChange();
	}
}

void UIComponent::Repaint() {}

DirectX::XMMATRIX* UIComponent::GetTransform() {
	return &transform;
}

DirectX::XMMATRIX* UIComponent::GetScreenTransform(DirectX::XMMATRIX* dest) {
	if (parent_container == nullptr) {
		*dest = transform;
		return dest;
	}
	else {
		DirectX::XMMATRIX parent_transform{};
		parent_container->GetScreenTransform(&parent_transform);
		*dest = DirectX::XMMatrixMultiply(parent_transform, transform);
		return dest;
	}
}

void UIComponent::SetTransform(DirectX::XMMATRIX* mat) {
	transform = *mat;
}

void UIComponent::UpdateTransform(DirectX::XMMATRIX* mat) {}

void UIComponent::ApplyLayout(UIComponentLayout* layout) {
	if (layout->visible != 0) {
		this->SetVisible(layout->visible != 2);
	}
	if (layout->enabled != 0) {
		this->SetEnabled(layout->enabled != 2);
	}
	if (layout->focusable != 0) {
		this->SetFocusable(layout->focusable != 2);
	}
	if (layout->selectable != 0) {
		this->SetSelectable(layout->selectable != 2);
	}
	if (layout->focused != 0) {
		this->SetFocused(layout->focused != 2);
	}
	if (layout->selected != 0) {
		this->SetSelected(layout->selected != 2);
	}
}

std::uint32_t UIComponent::GetColor() {
	return 0xffffffff;
}

std::uint32_t UIComponent::GetAlpha() {
	return this->GetColor() >> 24;
}

void UIComponent::SetAlpha(int alpha) {
	alpha = std::clamp(alpha, 0, 0xFF);
	std::uint32_t color = this->GetColor();
	this->SetColor(color & 0xffffff | alpha << 0x18);
}

void UIComponent::ScaleAlpha(float scale) {
	scale = std::clamp(scale, 0.0f, 1.0f);
	std::uint32_t alpha = static_cast<uint32_t>(this->GetAlpha() * scale);
	this->SetAlpha(alpha);
}

void UIComponent::SetColor(std::uint32_t color) {}

void UIComponent::GetObjectColorScale(float object_color_scale[4]) {
	object_color_scale[0] = 1.0;
	object_color_scale[1] = 1.0;
	object_color_scale[2] = 1.0;
	object_color_scale[3] = 1.0;
}

void UIComponent::SetObjectColorScale(float object_color_scale[4]) {}

void UIComponent::SetEnabled(int _enabled) {
	enabled = _enabled;
}

void UIComponent::SetVisible(int _visible) {
	visible = _visible;
}

void UIComponent::SetFocusable(int _focusable) {
	focusable = _focusable;
}

void UIComponent::SetFocused(UIComponent* other) {
	if (focusable != 0) {
		this->SetFocused(1);
		if (other != nullptr && other != this) {
			other->SetFocused(0);
		}
	}
}

void UIComponent::SetFocused(int _focused) {
	if (focusable != 0) {
		focused = _focused;
	}
}

void UIComponent::SetSelectable(int _selectable) {
	selectable = _selectable;
}

void UIComponent::SetSelected(UIComponent* other) {
	if (selectable != 0) {
		this->SetSelected(1);
		if (other != nullptr && other != this) {
			other->SetSelected(0);
		}
	}
}

void UIComponent::SetSelected(int _selected) {
	if (selectable != 0) {
		selected = _selected;
		this->Repaint();
	}
}

void UIComponent::RegisterEventListeners() {}

void UIComponent::DeRegisterEventListeners() {}

void UIComponent::SetScale(float scale) {}

void* UIComponent::GetLayer() {
	UNIMPLEMENTED(0x00623530);
}

void* UIComponent::GetStyleSheet() {
	UNIMPLEMENTED(0x00623590);
}

void UIComponent::CursorClicked(std::uint32_t u_msg, int button, int x, int y) {
	UNIMPLEMENTED(0x006277c0);
	/*
	if (u_msg == WM_RBUTTONDOWN) {
		if (parent_mouse_menu != nullptr) {
			parent_mouse_menu->Callback(1, button, x, y);
		}
	}
	else if (u_msg != WM_MBUTTONDOWN && parent_mouse_menu != nullptr) {
		parent_mouse_menu->Callback(0, button, x, y);
	}
	*/
}

void UIComponent::CursorSelected(std::uint32_t u_msg, int button, int x, int y) {
	UNIMPLEMENTED(0x00627830);
	/*
	if (parent_mouse_menu != nullptr) {
		parent_mouse_menu->Callback(2, button, x, y);
	}
	*/
}

uintptr_t UIComponent::FUN_00625050() {
	UNIMPLEMENTED(0x00625050);
}

void UIComponent::ResetMouseMenu() {
	parent_mouse_menu = nullptr;
}

void UIComponent::SetMouseMenu(MouseMenu* menu) {
	this->ResetMouseMenu();
	parent_mouse_menu = menu;
}

void UIComponent::FUN_00625170() {
	UNIMPLEMENTED(0x00625170);
}

void UIComponent::DoTick(float dt) {}

void UIComponent::DoRender(int) {}

UIContainer::~UIContainer() {
}

void UIContainer::StartComponent() {
	for (std::size_t i = 0; i < components.Length(); i++) {
		components[i]->StartComponent();
	}
}

void UIContainer::EndComponent() {
	for (std::size_t i = 0; i < components.Length(); i++) {
		components[i]->EndComponent();
	}
}

int UIContainer::IsContainer() {
	return 1;
}

UIComponent* UIContainer::GetComponent(char* component_name) {
	if (stricmp(name, component_name) == 0) {
		return this;
	}
	for (std::size_t i = 0; i < components.Length(); i++) {
		UIComponent* component = components[i]->GetComponent(component_name);
		if (component != nullptr) {
			return component;
		}
	}
	return nullptr;
}

void UIContainer::GetDimensions(float* x, float* y) {
	__RECT extents{};
	this->GetExtents(&extents);
	*x = extents.w - extents.x;
	*y = extents.h - extents.y;
}

void UIContainer::GetExtents(__RECT* extents) {
	bool has_visible_components = false;
	for (std::size_t i = 0; i < components.Length(); i++) {
		if (components[i]->visible) {
			__RECT child_extents{};
			components[i]->GetExtents(&child_extents);
			if (extents->y > child_extents.y) {
				extents->y = child_extents.y;
			}
			if (extents->h < child_extents.h) {
				extents->h = child_extents.h;
			}
			if (extents->x > child_extents.x) {
				extents->x = child_extents.x;
			}
			if (extents->w < child_extents.w) {
				extents->w = child_extents.w;
			}
			has_visible_components = true;
		}
	}
	if (!has_visible_components) {
		float x = 0.0f;
		float y = 0.0f;
		this->GetPosition(&x, &y);
		extents->w = x;
		extents->x = x;
		extents->h = y;
		extents->y = y;
	}
}

void UIContainer::GetPosition(float* x, float* y) {
	*x = transform.r[3].m128_f32[0];
	*y = transform.r[3].m128_f32[1];
}

void UIContainer::SetPosition(float x, float y) {
	transform.r[3].m128_f32[0] = x;
	transform.r[3].m128_f32[1] = y;
	transform.r[3].m128_f32[2] = 0.0f;
	DirectX::XMMATRIX mat = DirectX::XMMatrixIdentity();
	if (parent_container != nullptr) {
		parent_container->GetScreenTransform(&mat);
	}
	this->UpdateTransform(&mat);
}

void UIContainer::Repaint() {
	for (std::size_t i = 0; i < components.Length(); i++) {
		components[i]->Repaint();
	}
}

void UIContainer::SetTransform(DirectX::XMMATRIX* mat) {
	transform = *mat;
	if (parent_container == nullptr) {
		this->UpdateTransform(nullptr);
	}
	else {
		DirectX::XMMATRIX parent_transform{};
		parent_container->GetScreenTransform(&parent_transform);
		this->UpdateTransform(&parent_transform);
	}
}

void UIContainer::UpdateTransform(DirectX::XMMATRIX* mat) {
	if (mat == nullptr) {
		for (std::size_t i = 0; i < components.Length(); i++) {
			components[i]->UpdateTransform(&transform);
		}
	}
	else {
		DirectX::XMMATRIX new_transform = DirectX::XMMatrixMultiply(transform, *mat);
		for (std::size_t i = 0; i < components.Length(); i++) {
			components[i]->UpdateTransform(&new_transform);
		}
	}
}

void UIContainer::ApplyLayout(UIComponentLayout* layout) {
	UIComponent::ApplyLayout(layout);
	UIContainerLayout* upcast = dynamic_cast<UIContainerLayout*>(layout);
	this->SetPosition(upcast->x, upcast->y);
	this->SetScale(upcast->scale);
}

void UIContainer::SetAlpha(int alpha) {
	UIComponent::SetAlpha(alpha);
	for (std::size_t i = 0; i < components.Length(); i++) {
		components[i]->SetAlpha(alpha);
	}
}

void UIContainer::ScaleAlpha(float scale) {
	UIComponent::ScaleAlpha(scale);
	for (std::size_t i = 0; i < components.Length(); i++) {
		components[i]->ScaleAlpha(scale);
	}
}

void UIContainer::RegisterEventListeners() {
	for (std::size_t i = 0; i < components.Length(); i++) {
		components[i]->RegisterEventListeners();
	}
}

void UIContainer::DeRegisterEventListeners() {
	for (std::size_t i = 0; i < components.Length(); i++) {
		components[i]->DeRegisterEventListeners();
	}
}

void UIContainer::SetScale(float scale) {
	transform.r[0].m128_f32[0] = scale;
	transform.r[1].m128_f32[1] = scale;
	this->UpdateTransform(nullptr);
}

void UIContainer::DoTick(float dt) {
	for (std::size_t i = 0; i < components.Length(); i++) {
		if (components[i]->enabled) {
			components[i]->DoTick(dt);
		}
	}
}

void UIContainer::DoRender(int render) {
	for (std::size_t i = 0; i < components.Length(); i++) {
		if (components[i]->visible) {
			components[i]->DoRender(render);
		}
	}
}

UILayer::UILayer() {
	unk = 0;
	unk2 = 0;
	container = nullptr;
	style_sheet = nullptr;
	field6_0x20 = 0x10;
}