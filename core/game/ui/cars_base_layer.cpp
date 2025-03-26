#include "cars_base_layer.hpp"

CarsBaseLayer::CarsBaseLayer() : UILayer(), VNObject(2), UIFlashAnimationListener()
{
	flags = 0;
}

CarsBaseLayer::~CarsBaseLayer()
{
}

__declspec(naked) void CarsBaseLayer::Render(int)
{
	UNIMPLEMENTED(0x004538c0);
}
