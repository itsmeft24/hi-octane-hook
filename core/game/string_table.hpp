#include "base_object.hpp"

class StringTable : public BaseObject {
protected:

public:
	virtual ~StringTable() override;
	virtual int Load(int);
	virtual void Unload(int);
	virtual int Request(int);
	virtual void Request(int, void*);
	virtual int AddItem(char*, void*, int);
	virtual void Report();
	virtual void RemoveItem(int);
	virtual void PlaceTableMarker();
	virtual void RemoveTableMarker();
	virtual void RemoveAllMarkers();
	virtual void RemoveAllAfterMarker(int);
	virtual void Unk2(int);
	virtual void Unk3(int, int);
};

static_assert(sizeof(StringTable) == 0x54);



