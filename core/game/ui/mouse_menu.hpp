#pragma once
#include "core/game/bind.hpp"

class MouseMenu {
public:
	virtual void Callback(int state, int button, int x, int y) = 0;

	REPLACE_OPERATOR_NEW_DELETE
};
static_assert(sizeof(MouseMenu) == 0x4);