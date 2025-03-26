#pragma once
#include <cstdint>
#include <windows.h>
#include <DirectXMath.h>
#include <cstddef>
#include <cstring>
#include <algorithm>
#include "../container_list.hpp"
#include "../parameter_block.hpp"

enum PageStackState {
	NotTopOfStack,
	TopOfStack,
};

class UIPage {
protected:
	char* name;
	int layer_count;
	int* layer_name_indices;
	int* class_name_indices;
	int should_be_enabled;
	int enabled;
	int has_registered_event_listeners;
	ContainerList<void*> focus_managers;
	int is_top_of_stack;
	std::uint8_t has_not_top_enabled;
	std::uint8_t has_top_enabled;
public:
	UIPage();
	virtual ~UIPage();
	virtual void CreateFromFile(char*, ParameterBlock*);
	virtual void CreateFromName(char*);
	virtual void Initialize();
	virtual void Render();
	virtual void Tick(float dt);
	virtual void StartPage();
	virtual void EndPage();
	virtual void RegisterEventListeners();
	virtual void DeRegisterEventListeners();
	virtual void RegisterFocusManagers();
	virtual void DeRegisterFocusManagers();
	virtual void SetEnabled(int);
	virtual void SetPageStackState(PageStackState);
	virtual void Repaint();
	virtual void ReportToDebugOverlay(int, struct DebugOverlay*);
	virtual void PageStackStateChanged();
	virtual void Start();
	virtual void End();

	REPLACE_OPERATOR_NEW_DELETE
};

static_assert(sizeof(PageStackState) == 4);
static_assert(sizeof(UIPage) == 0x40);