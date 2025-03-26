#pragma
#include <cstdint>
#include <windows.h>
#include <DirectXMath.h>
#include <cstddef>
#include <cstring>
#include <algorithm>
#include "core/game/parameter_block.hpp"
#include "ui_container.hpp"
#include "ui_layout.hpp"
#include "ui_page.hpp"

struct UIStyleSheet;
class UILayer {
protected:
	char* name;
	int layer_unk;
	UIContainer* container;
	ContainerList<UIComponent*> components;
	UIStyleSheet* style_sheet;
	std::uint8_t layer_unk2;
public:
	UILayer();
	virtual ~UILayer();
	virtual void CreateFromFile(char*, ParameterBlock*);
	virtual void CreateFromName(char*);
	virtual void Tick(float dt);
	virtual void Render(int);
	virtual void StartLayer();
	virtual void EndLayer();
	virtual void StartComponents();
	virtual void EndComponents();
	virtual void RegisterEventListeners();
	virtual void DeRegisterEventListeners();
	virtual void PageStackStateChanged(PageStackState);
	virtual void SetEnabled(int);
	virtual int IsEnabled();
	virtual void SetVisible(int);
	virtual int IsVisible();
	virtual void SetLayout(UILayout*);
	// bruh what the hell is this?????
	virtual void FUN_004699c0();
	virtual void Repaint(int);
	virtual int CursorClicked(std::uint32_t u_msg, int button, int x, int y);
	virtual void CursorSelected(std::uint32_t u_msg, int button, int x, int y);

	REPLACE_OPERATOR_NEW_DELETE
};

static_assert(sizeof(UILayer) == 0x30);