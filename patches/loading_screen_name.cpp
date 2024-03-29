
#include <Windows.h>
#include <iostream>

#include "core/logging.hpp"
#include "core/hooking/framework.hpp"
#include "loading_screen_name.hpp"

DefineInlineHook(FixLoadingScreenPath) {
	static void callback(hooking::InlineCtx& ctx) {
		uintptr_t lpCarsGame = *reinterpret_cast<uintptr_t*>(0x00718A74);
		const char* loading_screen_name = reinterpret_cast<const char*>(lpCarsGame + 0x764);
		std::string calculated_path = std::format("e\\ui\\tex\\{}", loading_screen_name);

		if (calculated_path.size() < 0x103) {
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