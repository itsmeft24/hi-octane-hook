#pragma once
#include <d3d9.h>
#include <cstdint>
#include "../container_list.hpp"

class VideoCard {
protected:
	int initialized;
	int unused;
	int should_inc_frame_count;
	int unk_refresh_rate_param;
	int unk_refresh_rate_param_2;
	int unused2;
	int unused3;
	ContainerList<std::uint32_t> unused4;
public:
	VideoCard();
	virtual ~VideoCard();
	virtual void FreeCursor();
	virtual int ResetStateManager() = 0;
	virtual int DisplayToScreen(int inc_frame_count) = 0;
	virtual void Unk4() = 0;
	virtual void Unk5() = 0;
	virtual void Unk6() = 0;
	virtual void Unk7() = 0;
	virtual void Unk8();
};

static_assert(sizeof(VideoCard) == 0x38);

class X360VideoCard : public VideoCard {
private:
	IUnknown* unused;
	D3DCAPS9 capabilities;
	int current_frame;
	std::uint8_t flags;
	bool enable_fullscreen_effects;
public:

};

static_assert(sizeof(X360VideoCard) == 0x174);

inline auto g_VideoCard = reinterpret_cast<X360VideoCard**>(0x006ff394);