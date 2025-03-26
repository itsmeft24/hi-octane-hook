#pragma once
#include "cars_base_layer.hpp"

class CarsUIMenuLayer : public CarsBaseLayer {
protected:
	int anim_state;
	char* full_anim;
	char* partial_anim;
public:
	CarsUIMenuLayer();
	~CarsUIMenuLayer();
	virtual void CreateFromFile(char*, ParameterBlock*) override;
	virtual void PlayAnim0(int is_full_on);
	virtual void PlayAnim1(int is_full_on);
	virtual void Unk1();
	virtual void Unk2();
	virtual void SetVisibleIfNotPlayingAnim1();
	virtual void SetVisibleIfNotPlayingAnim2();
	virtual char* GetSelectedOption();
};

static_assert(sizeof(CarsUIMenuLayer) == 0x50);