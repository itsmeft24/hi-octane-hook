#pragma once
#include "bind.hpp"

class BaseObject {
public:
	BaseObject();
	virtual ~BaseObject();
	virtual std::int32_t AddRef();
	virtual std::int32_t RemoveRef();
	virtual std::int32_t Release();
	virtual std::int32_t GetRefCount();
	virtual void PreRelease();

	REPLACE_OPERATOR_NEW_DELETE
protected:
	std::int32_t refCount;
};