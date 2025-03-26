#include "core/config.hpp"
#include "core/logging.hpp"
#include "save_file_expansion.hpp"
#include "sunset/sunset.hpp"
#include <array>

void save_file_expansion::install()
{
	// Doubles the size of all PSLOT and GlobalData save files.
	if (config::g_ExpandedSaveFilesEnabled) {
		std::array<uintptr_t, 16> addresses = {
			0x0043f953,
			0x0043f977,
			0x0043f9d3,
			0x0043fa14,
			0x004845c8,
			0x00484605,
			0x00487285,
			0x0048aaa7,
			0x004c5573,
			0x004c5615,
			0x004c563d,
			0x004da5db,
			0x004fdf3c,
			0x0051bb50,
			0x0051bb74,
			0x0051bb97
		};
		for (const auto addr : addresses) {
			sunset::inst::push_u32(reinterpret_cast<void*>(addr), 0x20000);
		}
		sunset::utils::set_permission(reinterpret_cast<void*>(0x004845e7), 7, sunset::utils::Perm::ExecuteReadWrite);
		sunset::utils::set_permission(reinterpret_cast<void*>(0x0048729a), 7, sunset::utils::Perm::ExecuteReadWrite);
		*(std::uint8_t*)(0x004845e7 + 5) = 0x02;
		*(std::uint8_t*)(0x0048729a + 5) = 0x02;
	}

	logging::log("[save_file_expansion::install] Successfully installed patch!");
}
