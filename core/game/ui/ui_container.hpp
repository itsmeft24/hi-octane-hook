#pragma once
#include <cstdint>
#include <windows.h>
#include <DirectXMath.h>
#include <cstddef>
#include <cstring>
#include "ui_component.hpp"
#include "../container_list.hpp"

class UIContainerLayout : public UIComponentLayout {
public:
	float x;
	float y;
	float scale;
public:
	inline UIContainerLayout(char* new_name) : UIComponentLayout(new_name) {
		x = 0.0f;
		y = 0.0f;
		scale = 1.0f;
	}
	inline virtual ~UIContainerLayout() override {
	}
	virtual void Create(ParameterBlock*);
};

class UIContainer : public UIComponent {
protected:
	ContainerList<UIComponent*> components;
public:
	virtual ~UIContainer() override;
	virtual void StartComponent() override;
	virtual void EndComponent() override;
	virtual int IsContainer() override;
	virtual UIComponent* GetComponent(char* component_name) override;
	virtual void GetDimensions(float* x, float* y) override;
	virtual void GetExtents(__RECT* extents) override;
	virtual void GetPosition(float* x, float* y) override;
	virtual void SetPosition(float x, float y) override;
	virtual void Repaint() override;
	virtual void SetTransform(DirectX::XMMATRIX* mat) override;
	virtual void UpdateTransform(DirectX::XMMATRIX* mat) override;
	virtual void ApplyLayout(UIComponentLayout* layout) override;
	virtual void SetAlpha(int alpha) override;
	virtual void ScaleAlpha(float scale) override;
	virtual void RegisterEventListeners() override;
	virtual void DeRegisterEventListeners() override;
	virtual void SetScale(float scale) override;
	virtual void DoTick(float dt) override;
	virtual void DoRender(int render) override;
};

static_assert(sizeof(UIContainerLayout) == 0x30);
static_assert(sizeof(UIContainer) == 0xa0);