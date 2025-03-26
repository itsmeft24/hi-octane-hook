#include "music.hpp"

__declspec(naked) Music::~Music() {
	UNIMPLEMENTED(0x005bf160);
}

__declspec(naked) X360Music::~X360Music() {
	UNIMPLEMENTED(0x0041b120);
}

__declspec(naked) int X360Music::Tick(float dt) {
	UNIMPLEMENTED(0x0041baa0);
}

__declspec(naked) int X360Music::Pause(int paused) {
	UNIMPLEMENTED(0x0041b290);
}

__declspec(naked) void X360Music::CreateNumTracks(int param_1, int track_count) {
	UNIMPLEMENTED(0x0041c020);
}

__declspec(naked) void X360Music::Play(int song, int param_3) {
	UNIMPLEMENTED(0x0041b340);
}

__declspec(naked) bool X360Music::IsPlaying() {
	UNIMPLEMENTED(0x004193b0);
}

__declspec(naked) void X360Music::Repeat(int param_1, int param_2) {
	UNIMPLEMENTED(0x0041b380);
}

__declspec(naked) void X360Music::Loop(int param_1, int param_2) {
	UNIMPLEMENTED(0x0041b3c0);
}

__declspec(naked) bool X360Music::IsPaused() {
	UNIMPLEMENTED(0x0041a8c0);
}

__declspec(naked) int X360Music::Stop(float param_1) {
	UNIMPLEMENTED(0x0041a900);
}

__declspec(naked) int X360Music::SetVolume(int param_1) {
	UNIMPLEMENTED(0x0041a8e0);
}

__declspec(naked) int X360Music::RampVolume(int param_1, int param_2, int param_3) {
	UNIMPLEMENTED(0x0041a9c0);
}

__declspec(naked) int X360Music::GetCurrentTrack() {
	UNIMPLEMENTED(0x0041a8d0);
}

__declspec(naked) int X360Music::Shuffle(int param_1) {
	UNIMPLEMENTED(0x0041c0e0);
}

__declspec(naked) int X360Music::ShuffleStartingAt(int param_1, int param_2) {
	UNIMPLEMENTED(0x0041c1d0);
}

__declspec(naked) float X360Music::GetCurrentElapsedTime() {
	UNIMPLEMENTED(0x0041a9b0);
}

int X360Music::GetAdditionalNumberOfJukeboxTracks() {
	return 0;
}