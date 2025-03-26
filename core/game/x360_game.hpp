#pragma once

#include "game.hpp"
#include <d3d9.h>

class X360Game : public Game {
public:
	int unused3;
	int unused4;
	int unused1;
	int unused2;
	HWND hwnd;
	char unused5[16];
	IDirect3D9* d3d9;
public:
	virtual ~X360Game() override;
	virtual int InitializeRenderer(const char*) override;
	virtual int PreGameInitialize(DisplayMode*) override;
	virtual int SetBasicRenderStates() override;
	virtual void PrepareFrame() override;
	virtual int KeyUpHandler(ProcessNode*, KeyInfo*) override;
	virtual int KeyDownHandler(ProcessNode*, KeyInfo*) override;
	virtual int Terminate() override;
	virtual int SetDisplayMode(DisplayMode*) override;
	virtual RenderTarget* CreateRenderTarget() override;
};

static_assert(sizeof(X360Game) == 0x3b0);