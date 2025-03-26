#pragma once
#include "ui_page.hpp"

class CarsUIPage : public UIPage {
protected:
	struct GameText* game_text;
	int unk;
public:
	virtual ~CarsUIPage() override;
};

static_assert(sizeof(CarsUIPage) == 0x48);