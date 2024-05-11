#pragma once
#include "base_object.hpp"

struct ProcessNode;
struct KeyInfo;
enum DumpType {};

struct GameObjectFlags {
	bool enabled : 1;
	bool packetEnabled : 1;
	bool isPaused : 1;
	bool killMe : 1;
};
static_assert(sizeof(GameObjectFlags) == 1);

class GameObject : public BaseObject
{
public:
	virtual ~GameObject() override;
	virtual std::int32_t Release() override;
	virtual void Disable();
	virtual void DisableAll();
	virtual void Enable();
	virtual void EnableAll();
	virtual void AncestorsDisabled();
	virtual void AncestorsEnabled();
	virtual void EnableEvents(std::uint32_t);
	virtual void DisableEvents(std::uint32_t);
	virtual GameObject* Create();
	virtual int TickWhileInactive(float);
	virtual int Tick(float);
	virtual int SimulationTick(float);
	virtual int PrepareGeometry();
	virtual int RenderPre3D();
	virtual int Render3D(int userData);
	virtual int RenderPost3D();
	virtual int Pause(int);
	virtual int LostFocus();
	virtual int Restore();
	virtual int EventHandler(std::int32_t);
	virtual int KeyUpHandler(ProcessNode*, KeyInfo*);
	virtual int KeyDownHandler(ProcessNode*, KeyInfo*);
	virtual int PacketHandler(std::uint32_t, void*, std::uint32_t, std::uint32_t, std::uint32_t);
	virtual void Kill();
	virtual void DumpHierarchy(char*, DumpType, std::int32_t);

	GameObject(bool startEnabled);
	void UpPropogateMasks();
private:
	GameObject* prev;
	GameObject* next;
	GameObject* firstChild;
	GameObject* parent;
	std::uint32_t wantedEventMask;
	std::uint32_t descendantsWantedEventMask;
	std::uint8_t typeID;
	GameObjectFlags flags;
};