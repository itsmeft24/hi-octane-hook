#pragma once

class UIFlashAnimationListener {
public:
	virtual void AnimationDone() = 0;

	REPLACE_OPERATOR_NEW_DELETE
};

class UIEvent {
public:
	inline virtual ~UIEvent() {}

	REPLACE_OPERATOR_NEW_DELETE
};

class UIInputEvent : public UIEvent {
public:
	int device_id;
	int unk1;
	int key_count;
	int key_values[100];
	char key_states[100];
	int unk2;
	int unk3;
public:
	inline UIInputEvent() {
		device_id = -1;
		unk2 = 0;
		key_count = 0;
	}

	REPLACE_OPERATOR_NEW_DELETE
};

class UIInputEventListener {
public:
	inline virtual ~UIInputEventListener() {}
	virtual void ProcessKeydown(UIInputEvent*) = 0;
	virtual void ProcessKeyup(UIInputEvent*) = 0;

	REPLACE_OPERATOR_NEW_DELETE
};

static_assert(sizeof(UIFlashAnimationListener) == 4);
static_assert(sizeof(UIEvent) == 4);
static_assert(sizeof(UIInputEvent) == 0x20c);
static_assert(sizeof(UIInputEventListener) == 4);