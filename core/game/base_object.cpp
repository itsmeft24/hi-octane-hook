#include "base_object.hpp"

BaseObject::~BaseObject()
{
}

std::int32_t BaseObject::AddRef()
{
	return refCount++;
}

std::int32_t BaseObject::RemoveRef()
{
	if (refCount != 0) {
		return refCount--;
	}
	return 0;
}

std::int32_t BaseObject::Release()
{
	refCount--;
	if (refCount == 0) {
		PreRelease();
		delete this;
		return 0;
	}
	return refCount;
}

std::int32_t BaseObject::GetRefCount()
{
	return refCount;
}

void BaseObject::PreRelease()
{
}