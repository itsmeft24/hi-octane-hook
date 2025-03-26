#pragma once
#include <cstdint>
#include <windows.h>
#include <cstddef>
#include <cstring>
#include <algorithm>
#include "ui_component.hpp"
#include "../container_list.hpp"

class UILayout {
protected:
	std::uint8_t unk;
	ContainerList<UIComponentLayout*> component_layouts;
public:
	virtual ~UILayout();
	virtual void Create(/* ParameterBlock* maybe idk*/);

	REPLACE_OPERATOR_NEW_DELETE
};

static_assert(sizeof(UILayout) == 0x20);