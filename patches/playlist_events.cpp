#include <iostream>
#include <unordered_map>

#include "core/logging.hpp"
#include "core/hooking/framework.hpp"
#include "patches/playlist_events.hpp"

const std::unordered_map<std::string, int> activityNameToPlaylistIdx = {
	{"HubRace", 1},
	{"StadiumRace", 2},
	{"RustbucketRace", 3},
};

DefineInlineHook(HandleRaceTypes) {
	static void __cdecl callback(hooking::InlineContext ctx) {
		const std::string activity = *reinterpret_cast<char**>(ctx.esi.unsigned_integer + 0xc);
		if (activityNameToPlaylistIdx.contains(activity)) {
			ctx.eax.unsigned_integer = activityNameToPlaylistIdx.at(activity);
		}
		else {
			logging::log("[playlist_events] Encountered race activity: {} which does not have an assigned playlist! defaulting to frontend...", activity);
			ctx.eax.unsigned_integer = 12;
		}
	}
};

void playlist_events::install() {
	HandleRaceTypes::install_at_ptr(0x00431016);
}