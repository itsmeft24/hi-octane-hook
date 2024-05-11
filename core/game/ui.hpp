#pragma once

#include <cstdint>
#include <windows.h>
#include <DirectXMath.h>
#include <cstddef>
#include <cstring>
#include <algorithm>
#include "core/game/all.hpp"

struct __RECT {
	int x;
	int y;
	int w;
	int h;
};

class UIComponent;
class UIContainer;

class UIComponentLayout {
	enum class ApplyCondition : std::uint8_t {
		Never = 0,
		Once,
		Always
	};
public:
	char* name;
	int visible;
	int enabled;
	int focusable;
	int focused;
	int selectable;
	int selected;
	ApplyCondition condition;
public:
	inline UIComponentLayout(char* new_name) {
		std::size_t len = strlen(new_name);
		name = reinterpret_cast<char*>(malloc(len + 1));
		memcpy(name, new_name, len);
		name[len] = 0;
		visible = 1;
		enabled = 1;
		selectable = 1;
		selected = 0;
		focusable = 1;
		focused = 0;
		condition = ApplyCondition::Always;
	}
	virtual ~UIComponentLayout() {
		free(name);
	}
	virtual void Create(struct ParameterBlock*) {
		UNIMPLEMENTED(0x0060ef90);
	}
};

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
	virtual ~UIContainerLayout() override {
		UIComponentLayout::~UIComponentLayout();
	}
	virtual void Create(struct ParameterBlock*) {
		UNIMPLEMENTED(0x0060f130);
	}
};

class MouseMenu {
public:
	virtual void Callback(int state, int button, int x, int y) {};
};

class UIComponent {
public:
	// char unk[12] = {};
	UIContainer* parent_container;
	// char unk2[12] = {};
	DirectX::XMMATRIX transform;
	char* name;
	int visible;
	int enabled;
	int focusable;
	int focused;
	int selectable;
	int selected;
	MouseMenu* parent_mouse_menu;
public:
	UIComponent();
	virtual ~UIComponent();
	virtual void Create(char* new_name);
	virtual void StartComponent();
	virtual void EndComponent();
	virtual int IsContainer();
	virtual UIComponent* GetComponent(char* component);
	virtual void GetDimensions(float* x, float* y);
	virtual void SetDimensions(float* x, float* y);
	virtual void GetExtents(__RECT* extents);
	virtual void GetExtentsRelative(__RECT* extents);
	virtual float GetWidth();
	virtual float GetHeight();
	virtual void GetPosition(float* x, float* y);
	virtual void SetPosition(float x, float y);
	virtual void GetAbsolutePosition(float* x, float* y);
	virtual void SetAngle(float angle);
	virtual void PropogateChange();
	virtual void Repaint();
	virtual DirectX::XMMATRIX* GetTransform();
	virtual DirectX::XMMATRIX* GetScreenTransform(DirectX::XMMATRIX* dest);
	virtual void SetTransform(DirectX::XMMATRIX* mat);
	virtual void UpdateTransform(DirectX::XMMATRIX* mat);
	virtual void ApplyLayout(UIComponentLayout* layout);
	virtual std::uint32_t GetColor();
	virtual std::uint32_t GetAlpha();
	virtual void SetAlpha(int alpha);
	virtual void ScaleAlpha(float scale);
	virtual void SetColor(std::uint32_t color);
	virtual void GetObjectColorScale(float object_color_scale[4]);
	virtual void SetObjectColorScale(float object_color_scale[4]);
	virtual void SetEnabled(int _enabled);
	virtual void SetVisible(int _visible);
	virtual void SetFocusable(int _focusable);
	virtual void SetFocused(UIComponent* other);
	virtual void SetFocused(int _focused);
	virtual void SetSelectable(int _selectable);
	virtual void SetSelected(UIComponent* other);
	virtual void SetSelected(int _selected);
	virtual void RegisterEventListeners();
	virtual void DeRegisterEventListeners();
	virtual void SetScale(float scale);
	virtual void* GetLayer();
	virtual void* GetStyleSheet();
	virtual void CursorClicked(std::uint32_t u_msg, int button, int x, int y);
	virtual void CursorSelected(std::uint32_t u_msg, int button, int x, int y);
	virtual uintptr_t FUN_00625050();
	virtual void ResetMouseMenu();
	virtual void SetMouseMenu(MouseMenu* menu);
	virtual void FUN_00625170();
	virtual void DoTick(float dt);
	virtual void DoRender(int);
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

struct UIStyleSheet;

class UILayer {
protected:
	int unk;
	int unk2;
	UIContainer* container;
	ContainerList<UIComponent*> components;
	UIStyleSheet* style_sheet;
	std::uint8_t field6_0x20;
public:
	UILayer();
};

template <typename T> class UILinkedListNode {
public:
	T value;
	UILinkedListNode<T>* prev;
	UILinkedListNode<T>* next;
public:
	virtual ~UILinkedListNode<T>() {
	}
};

template <typename T> class UILinkedList {
private:
	UILinkedListNode<T>* start;
	std::size_t len;
public:
	virtual ~UILinkedList<T>() {
		UILinkedListNode<T>* node = this->start;
		while (len > 0) {
			if (node == nullptr) {
				break;
			}
			UILinkedListNode<T>* next_node = node->next;
			node->~UILinkedListNode<T>();
			node = next_node;
			len--;
		}
	}
};

static_assert(sizeof(UILinkedListNode<int>) == 0x10);
static_assert(sizeof(UILinkedList<int>) == 0xc);

static_assert(sizeof(MouseMenu) == 0x4);
static_assert(sizeof(UIComponentLayout) == 0x24);
static_assert(sizeof(UIContainerLayout) == 0x30);
static_assert(sizeof(UIComponent) == 0x80);
static_assert(sizeof(UIContainer) == 0xa0);
