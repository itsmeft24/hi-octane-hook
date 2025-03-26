#include "core/config.hpp"
#include "core/game/gfx/emotion_engine.hpp"
#include "core/game/x360_game.hpp"
#include "core/logging.hpp"
#include "core/utils.hpp"
#include "sunset/sunset.hpp"
#include "widescreen.hpp"
#include "windowed_fullscreen.hpp"

DefineReplacementHook(RendererInitializationErrorLog) {
	static int __fastcall callback(X360Game * game, uintptr_t edx, char* unk) {
		int success = original(game, edx, unk);
		if (success == 0) {
			logging::log("[X360Game::InitializeRenderer] Failed to initialize renderer!");
		}
		return success;
	}
};

DefineReplacementHook(LogAllFoundDisplayModes) {
	static bool __fastcall callback(EE::D3DDeviceManager * _this, uintptr_t edx, HWND unk) {
		bool success = original(_this, edx, unk);
		for (std::size_t i = 0; i < _this->adapter_list.len; i++) {
			for (std::size_t j = 0; j < _this->adapter_list.data[i]->display_mode_list.len; j++) {
				auto display_mode = _this->adapter_list.data[i]->display_mode_list.data[j];
				logging::log("[EE::D3DDeviceManager::CreateAdapters] Found display mode with format: {}, width: {}, height: {}", std::to_underlying(display_mode->format), display_mode->width, display_mode->height);
			}
		}
		return success;
	}
};

DefineInlineHook(ForceWindowedWhenFS) {
	static void __cdecl callback(sunset::InlineCtx & ctx) {
		(*lpD3DDeviceManager)->windowed = true;
	}
};

DefineReplacementHook(IsResolutionAvailable) {
	static bool __fastcall callback(EE::D3DDeviceManager * _this, uintptr_t edx, int w, int h) {
		return GetSystemMetrics(SM_CXSCREEN) >= w && GetSystemMetrics(SM_CYSCREEN) >= h;
	}
};

DefineInlineHook(SetViewportOffsets) {
	static void __cdecl callback(sunset::InlineCtx & ctx) {
		int* offset_x = reinterpret_cast<int*>(0x00716238);
		int* offset_y = reinterpret_cast<int*>(0x0071623c);
		bool is_actually_windowed = *reinterpret_cast<bool*>(0x0071b224);
		if (!is_actually_windowed) {
			int window_width = *reinterpret_cast<int*>(0x006faa54);
			int window_height = *reinterpret_cast<int*>(0x006faa58);
			auto [viewport_width, viewport_height] = utils::resolve_game_window_dimensions(window_width, window_height, *g_ScreenMode == 2);
			*offset_x = (window_width - viewport_width) / 2;
			*offset_y = (window_height - viewport_height) / 2;
		}
		else {
			*offset_x = 0;
			*offset_y = 0;
		}
	}
};

DefineInlineHook(SetViewportPresentOffsets) {
	static void __cdecl callback(sunset::InlineCtx & ctx) {
		int* offset_x = reinterpret_cast<int*>(0x00716238);
		int* offset_y = reinterpret_cast<int*>(0x0071623c);
		D3DVIEWPORT9* viewport = reinterpret_cast<D3DVIEWPORT9*>(ctx.esp.unsigned_integer + 0x14);
		viewport->X = *offset_x;
		viewport->Y = *offset_y;
	}
};

DefineInlineHook(SetViewportPresentOffsets2) {
	static void __cdecl callback(sunset::InlineCtx & ctx) {
		int* offset_x = reinterpret_cast<int*>(0x00716238);
		int* offset_y = reinterpret_cast<int*>(0x0071623c);
		D3DVIEWPORT9* viewport = reinterpret_cast<D3DVIEWPORT9*>(ctx.esp.unsigned_integer + 0x4);
		viewport->X = *offset_x;
		viewport->Y = *offset_y;
	}
};

DefineInlineHook(InterceptViewportDimensions) {
	static void __cdecl callback(sunset::InlineCtx & ctx) {
		bool is_actually_windowed = *reinterpret_cast<bool*>(0x0071b224);
		if (!is_actually_windowed) {
			int window_width = *reinterpret_cast<int*>(0x006faa54);
			int window_height = *reinterpret_cast<int*>(0x006faa58);
			auto [viewport_width, viewport_height] = utils::resolve_game_window_dimensions(window_width, window_height, *g_ScreenMode == 2);
			ctx.eax.unsigned_integer = viewport_width;
			ctx.ecx.unsigned_integer = viewport_height;
		}
	}
};

void windowed_fullscreen::install() {
	if (config::g_WindowedFullscreenEnabled) {
		ForceWindowedWhenFS::install_at_ptr(0x00415096);
		InterceptViewportDimensions::install_at_ptr(0x00421e1f);
		IsResolutionAvailable::install_at_ptr(0x0063b2e0);
		sunset::inst::nop(reinterpret_cast<void*>(0x00421e49), 20);
		SetViewportOffsets::install_at_ptr(0x00421e49);
		sunset::inst::nop(reinterpret_cast<void*>(0x0041529a), 7);
		SetViewportPresentOffsets::install_at_ptr(0x004237fb);
		sunset::inst::nop(reinterpret_cast<void*>(0x0054428f), 2);
		SetViewportPresentOffsets2::install_at_ptr(0x00414937);
		sunset::inst::nop(reinterpret_cast<void*>(0x00544297), 3);
		logging::log("[windowed_fullscreen::install] Enabled windowed fullscreen!");
	}
#ifdef _DEBUG
	LogAllFoundDisplayModes::install_at_ptr(0x0063bdb0);
#endif // _DEBUG
	RendererInitializationErrorLog::install_at_ptr(0x00421ee0);

	logging::log("[windowed_fullscreen::install] Successfully installed patch!");
}
