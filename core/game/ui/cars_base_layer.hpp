#pragma once
#include "ui_layer.hpp"
#include "ui_event.hpp"
#include "core/game/virtual_network.hpp"

class CarsBaseLayer : public UILayer, VNObject, UIFlashAnimationListener {
protected:
	int flags;
public:
	CarsBaseLayer();
	virtual ~CarsBaseLayer() override;
	virtual void Render(int) override;

	REPLACE_OPERATOR_NEW_DELETE
};

static_assert(sizeof(CarsBaseLayer) == 0x44);