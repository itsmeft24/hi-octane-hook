#pragma once

class SerializableInterface {
public:
	virtual int Serialize(void* buffer, int len) = 0;
	virtual int DeSerialize(void* buffer, int len) = 0;
	virtual int GetSerializedDataSize() = 0;
};