#include "ui_container.hpp"

__declspec(naked) void UIContainerLayout::Create(ParameterBlock* pblock) {
	UNIMPLEMENTED(0x0060f130);
}

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
	if (_stricmp(name, component_name) == 0) {
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
	*x = static_cast<float>(extents.w - extents.x);
	*y = static_cast<float>(extents.h - extents.y);
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
		extents->w = static_cast<int>(x);
		extents->x = static_cast<int>(x);
		extents->h = static_cast<int>(y);
		extents->y = static_cast<int>(y);
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