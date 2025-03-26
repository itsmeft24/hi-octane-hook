#pragma once
#include "core/game/container_list.hpp"
#include "core/game/game_object.hpp"

class VNObject;
struct VNObjectRef;

class VirtualNetwork : public GameObject {
public:
	ContainerList<VNObjectRef*> refs;
public:
	VNObjectRef* CreateReference(VNObject*, int);
	VNObjectRef* GetReference(VNObject*);
	int RemoveReference(VNObjectRef*);
};

inline VirtualNetwork** lpVirtualNetwork = reinterpret_cast<VirtualNetwork**>(0x0071632c);

struct VNMessage {
	int unk;
	int unk1;
	int unk2;
	void* data;
};

class VNObject {
protected:
	int unk;
	VNObject* parent_idk;
public:
	VNObject(int);
	virtual ~VNObject();
	virtual void ReceiveVNOData(VNObject*, void*) = 0;
	virtual void ReceiveVNOEvent(VNObject*, VNMessage*) = 0;
};

static_assert(sizeof(VNObject) == 12);
static_assert(sizeof(VirtualNetwork) == 0x3c);