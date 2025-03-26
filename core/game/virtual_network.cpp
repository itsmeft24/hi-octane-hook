#include "virtual_network.hpp"

VNObject::VNObject(int param_1)
{
	unk = 0;
	parent_idk = this;
	if (*lpVirtualNetwork != nullptr) {
		(*lpVirtualNetwork)->CreateReference(this, param_1);
	}
}

VNObject::~VNObject()
{
	if (*lpVirtualNetwork != nullptr) {
		VNObjectRef* ref = (*lpVirtualNetwork)->GetReference(this);
		if (ref != nullptr) {
			(*lpVirtualNetwork)->RemoveReference(ref);
		}
	}
}

__declspec(naked) VNObjectRef* VirtualNetwork::CreateReference(VNObject*, int)
{
	UNIMPLEMENTED(0x005ccf70);
}

__declspec(naked) VNObjectRef* VirtualNetwork::GetReference(VNObject*)
{
	UNIMPLEMENTED(0x0059e8f0);
}

__declspec(naked) int VirtualNetwork::RemoveReference(VNObjectRef*)
{
	UNIMPLEMENTED(0x005ba8f0);
}
