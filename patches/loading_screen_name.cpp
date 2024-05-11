
#include <Windows.h>
#include <iostream>

#include "core/config.hpp"
#include "core/logging.hpp"
#include "core/hooking/framework.hpp"
#include "core/game/all.hpp"
#include "loading_screen_name.hpp"

DefineInlineHook(FixLoadingScreenPath) {
	static void callback(hooking::InlineCtx& ctx) {
		const char* loading_screen_name = reinterpret_cast<const char*>(reinterpret_cast<uintptr_t>(*lpCarsGame) + 0x764);
		
		std::string calculated_path{};
		if (config::g_WidescreenEnabled) {
			calculated_path = std::format("{}\\ui\\tex_hd\\{}", config::g_LangPrefix, loading_screen_name);
		}
		else {
			calculated_path = std::format("{}\\ui\\tex\\{}", config::g_LangPrefix, loading_screen_name);
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