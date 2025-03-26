#pragma once
#include <cstdint>
#include <windows.h>
#include <DirectXMath.h>
#include <cstddef>
#include <cstring>
#include <algorithm>
#include "mouse_menu.hpp"
#include "core/game/bind.hpp"
#include "core/game/parameter_block.hpp"
//#include "ui_container.hpp"

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
		name = reinterpret_cast<char*>(operator_new(len + 1));
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
	inline virtual ~UIComponentLayout() {
		operator_delete(name);
	}
	virtual void Create(ParameterBlock*);
	__forceinline void* operator new(std::size_t sz) {
		return operator_new(sz);
	}
	__forceinline void operator delete(void* ptr) {
		return operator_delete(ptr);
	}
};

struct __RECT {
	int x;
	int y;
	int w;
	int h;
};

class UIComponent {
public:
	char unk[12] = {};
	class UIContainer* parent_container;
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

	REPLACE_OPERATOR_NEW_DELETE
};

static_assert(sizeof(UIComponentLayout) == 0x24);
static_assert(sizeof(UIComponent) == 0x80);