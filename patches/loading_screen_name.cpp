
#include <Windows.h>
#include <iostream>

#include "core/config.hpp"
#include "core/logging.hpp"
#include "sunset/sunset.hpp"
#include "core/game/cars_game.hpp"
#include "loading_screen_name.hpp"
#include "widescreen.hpp"

DefineInlineHook(FixLoadingScreenPath) {
	static void callback(sunset::InlineCtx& ctx) {
		std::string calculated_path{};
		if (*g_ScreenMode == 2) {
			calculated_path = std::format("{}\\ui\\tex_hd\\{}", config::g_LangPrefix, (*lpCarsGame)->loading_screen_name);
		}
		else {
			calculated_path = std::format("{}\\ui\\tex\\{}", config::g_LangPrefix, (*lpCarsGame)->loading_screen_name);
		}

		if (calculated_path.size() < 0x104) {
			char* buffer = *reinterpret_cast<char**>(ctx.esp.pointer);
			std::memset(buffer, 0, 0x104);
			std::memcpy(buffer, calculated_path.data(), calculated_path.size());
		}
	}
};

void loading_screen_name::install() {
	FixLoadingScreenPath::install_at_ptr(0x004053a7);
    logging::log("[loading_screen_name::install] Successfully installed patch!");
}