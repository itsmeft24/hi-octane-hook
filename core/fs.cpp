#pragma once

#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <shlobj.h>

#include "core/bink_reexport.hpp"
#include "core/config.hpp"
#include "core/fs.hpp"
#include "core/globals.hpp"
#include "core/logging.hpp"
#include "hooking/framework.hpp"
#include "core/utils.hpp"

DeclareFunction(void*, __cdecl, __fsopen, 0x0063FBFB, char*, char*, int); // VS2005 CRT function
DeclareFunction(DWORD, __stdcall, BASS_SampleLoad, GetProcAddress(GetModuleHandleA("bass.dll"), "BASS_SampleLoad"), BOOL, char*, DWORD, DWORD, DWORD, DWORD);
DeclareFunction(DWORD, __stdcall, BASS_StreamCreateFile, GetProcAddress(GetModuleHandleA("bass.dll"), "BASS_StreamCreateFile"),BOOL, char*, DWORD, DWORD, DWORD);

std::unordered_map<std::string, std::filesystem::path> MAP;

std::vector<std::string> MARK_AS_DELETED;

std::vector<std::string> BlackListedModFiles = { "config.ini", "language.ini" };

std::filesystem::path fs::resolve_path(std::string path) {
	utils::make_lowercase(path);
	if (MAP.find(path) != MAP.end()) { // if we have a modfile for the file
		return MAP.at(path);
	}
	return g_DataDir / path;
}

// Determines the absolute path for a resource. Takes in three arguments: a file
// path string, a destination string pointer, the size of the allocated pointer
// (used for bounds checking).
HIOCTANE_API bool __stdcall HiOctane_GetPathForFile(const char* filepath, char* destination, unsigned int dst_len) {
	std::string path(filepath);
	utils::make_lowercase(path);
	if (MAP.find(path) != MAP.end()) { // if we have a modfile for the file
		std::string out_str = MAP.at(path).string();
		if (out_str.size() < dst_len) {
			memcpy((void*)destination, out_str.c_str(), out_str.size());
		}
		else {
			return false;
		}
	}
	else if (std::filesystem::exists(g_DataDir / path)) {
		std::string out_str = (g_DataDir / path).string();
		if (out_str.size() < dst_len) {
			memcpy((void*)destination, out_str.c_str(), out_str.size());
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
	return true;
}

std::string get_mod_for_file(const std::string& file) {
	std::filesystem::path& path = MAP.at(file);
	std::string str = path.string();
	utils::make_lowercase(str);
	str = str.substr(str.find("\\mods\\") + 6);
	return str.substr(0, str.find("\\"));
}

// Takes in three arguments: an asset file path string, a destination buffer,
// and the size of the buffer (used for bounds checking) If the file does not
// exist, 0 is returned. If the buffer is too small, the size of the file is
// returned. Otherwise, the size of the file is returned.
HIOCTANE_API unsigned int __stdcall HiOctane_LoadFile(const char* filepath, void* buffer, size_t allocated) {
	auto p = fs::resolve_path(filepath);
	if (!std::filesystem::exists(p)) {
		return 0;
	}

	uintmax_t file_size = std::filesystem::file_size(p);
	if (file_size > allocated) {
		return 0;
	}

	std::ifstream f(p, std::ios::in | std::ios::binary);
	f.read((char*)buffer, file_size);
	f.close();
	return static_cast<unsigned int>(file_size);
}

// Registers a deleted file, takes in a single argument being a file path
// string. Deletion is handled in the same manner as the MARK_AS_DELETED.txt
// text file.
HIOCTANE_API void __stdcall HiOctane_RegisterDeletedFile(const char* str) {
	MARK_AS_DELETED.push_back(str);
}

bool discover_files() {
	std::filesystem::path mods_dir(g_InstallDir / "mods\\");
	if (std::filesystem::is_directory(mods_dir)) {
		for (const auto mod : std::filesystem::directory_iterator(mods_dir)) {
			if (mod.is_directory() && mod.path().filename().string()[0] != '.') {
				for (const auto& entry : std::filesystem::recursive_directory_iterator(mod)) {
					if (entry.is_regular_file()) {
						// do NOT load any modules stored in mod folders.
						if (entry.path().extension().string() == ".dll") {
							continue;
						}

						auto fname = entry.path().filename().string();
						
						// Make sure the file isn't blacklisted.
						if (std::find(BlackListedModFiles.begin(), BlackListedModFiles.end(), entry.path().filename().string()) != BlackListedModFiles.end()) {
							continue;
						}

						if (fname == "mark_as_deleted.txt") {
							std::ifstream file(entry.path(), std::ios::in);
							if (!file) {
								return false;
							}

							std::string line;
							while (std::getline(file, line)) {
								utils::make_lowercase(line);
								if (std::find(MARK_AS_DELETED.begin(), MARK_AS_DELETED.end(), line) == MARK_AS_DELETED.end()) {
									MARK_AS_DELETED.push_back(line);
									logging::log("[fs::discover_files] Added {} to the deletion queue.", line);
								}
							}
							file.close();
							continue;
						}

						if (entry.path().extension().string() == ".wav" || entry.path().extension().string() == ".mp3" || entry.path().extension().string() == ".ogg" || entry.path().extension().string() == ".aiff") {
							std::string base_filename = entry.path().string().substr(mod.path().string().size() + 1, entry.path().string().size() - mod.path().string().size() - entry.path().extension().string().size()) + "ogg";
							utils::make_lowercase(base_filename);
							// For adding mp3/wav/aiff support, we strip the mod path root AND the extension before adding .ogg to the end.

							if (MAP.find(base_filename) != MAP.end()) {
								logging::log("[fs::discover_files] File: {} in mod: {} overwrites existing mod file in: {}", base_filename, mod.path().filename().string(), get_mod_for_file(base_filename));
								MAP.insert_or_assign(base_filename, entry.path());
							}
							else {
								logging::log("[fs::discover_files] Found file: {} in mod: {}", base_filename, mod.path().filename().string());
								MAP.insert_or_assign(base_filename, entry.path());
							}
						}
						else {
							std::string base_filename = entry.path().string().substr(mod.path().string().size() + 1);
							utils::make_lowercase(base_filename);

							if (MAP.find(base_filename) != MAP.end()) {
								logging::log("[fs::discover_files] File: {} in mod: {} overwrites existing mod file in: {}", base_filename, mod.path().filename().string(), get_mod_for_file(base_filename));
								MAP.insert_or_assign(base_filename, entry.path());
							}
							else {
								logging::log("[fs::discover_files] Found file: {} in mod: {}", base_filename, mod.path().filename().string());
								MAP.insert_or_assign(base_filename, entry.path());
							}
						}
					}
				}
			}
		}
		return !MAP.empty();
	}
	else {
		return false;
	}
}

BINK* __stdcall BinkOpenHook(char* file, std::uint32_t flags) {

	// apologies for this ugly hack
	DeclareFunction(BINK*, __stdcall, BinkOpen, BINKOPEN, char*, std::uint32_t);

	std::string base_filepath = file;
	// Strip DataPC path and make it lowercase.
	base_filepath = base_filepath.substr(g_DataDir.string().size() + 1);
	utils::make_lowercase(base_filepath);

	// If we have a modfile for the file:
	if (MAP.find(base_filepath) != MAP.end()) {
		const auto& out_path = MAP.at(base_filepath);

		logging::log("[fs::BinkW32::BinkOpen] Loading Bink file: {} from mod: {}...", base_filepath, get_mod_for_file(base_filepath));

		// Return expected result
		return BinkOpen((char*)out_path.string().c_str(), flags);
	}


	// If the file should be marked as deleted, then return nullptr.
	// this comes after swapping the path for modfiles, so if another mod (or
	// the mod itself) has that file it will still be loaded.
	if (std::find(MARK_AS_DELETED.begin(), MARK_AS_DELETED.end(), base_filepath) != MARK_AS_DELETED.end()) {
		return 0;
	}

	// Return expected result
	return BinkOpen(file, flags);
}

DWORD __stdcall BASS_StreamCreateFileHook(BOOL mem, char* file, DWORD offset, DWORD length, DWORD flags) {
	std::string base_filepath = file;
	// Strip DataPC path and make it lowercase.
	base_filepath = base_filepath.substr(g_DataDir.string().size() + 1);
	utils::make_lowercase(base_filepath);

	// If we have a modfile for the file:
	if (MAP.find(base_filepath) != MAP.end()) {
		const auto& out_path = MAP.at(base_filepath);

		logging::log("[fs::BASS::StreamCreateFile] Loading stream file: {} from mod: {}...", base_filepath, get_mod_for_file(base_filepath));

		// Return expected result
		return BASS_StreamCreateFile(mem, (char*)out_path.string().c_str(), offset, length, flags);
	}
	else {
		logging::log( "[fs::BASS::StreamCreateFile] Loading stream file: {}...", base_filepath);
	}

	// If the file should be marked as deleted, then return nullptr.
	// this comes after swapping the path for modfiles, so if another mod (or
	// the mod itself) has that file it will still be loaded.
	if (std::find(MARK_AS_DELETED.begin(), MARK_AS_DELETED.end(), base_filepath) != MARK_AS_DELETED.end()) {
		return 0;
	}

	// return expected result
	return BASS_StreamCreateFile(mem, file, offset, length, flags);
}

DWORD __stdcall BASS_SampleLoadHook(BOOL mem, char* file, DWORD offset, DWORD length, DWORD max, DWORD flags) {

	std::string base_filepath = file;
	// Strip DataPC path and make it lowercase.
	base_filepath = base_filepath.substr(g_DataDir.string().size() + 1);
	utils::make_lowercase(base_filepath);

	// If we have a modfile for the file:
	if (MAP.find(base_filepath) != MAP.end()) {
		const auto& out_path = MAP.at(base_filepath);

		logging::log("[fs::BASS::SampleLoad] Loading stream file: {} from mod: {}...", base_filepath, MAP.at(base_filepath).string());

		// Return expected result
		return BASS_SampleLoad(mem, (char*)out_path.c_str(), offset, length, max, flags);
	}
	else {
		logging::log("[fs::BASS::SampleLoad] Loading stream file: {}...", base_filepath);
	}

	// If the file should be marked as deleted, then return nullptr.
	// this comes after swapping the path for modfiles, so if another mod (or
	// the mod itself) has that file it will still be loaded.
	if (std::find(MARK_AS_DELETED.begin(), MARK_AS_DELETED.end(), base_filepath) != MARK_AS_DELETED.end()) {
		return 0;
	}

	// Return expected result
	return BASS_SampleLoad(mem, file, offset, length, max, flags);
}

// modified _fopen from VS2005
DefineReplacementHook(fopenHook) {
	static void* __cdecl callback(char* _Filename, char* _Mode) { 
		if (*_Mode == 'r') {
			if (_strnicmp(_Filename, g_DataDir.string().c_str(), g_DataDir.string().size()) == 0) { // is a DataPC file

				std::string base_filepath = _Filename;
				// Strip DataPC path and make it lowercase.
				base_filepath = base_filepath.substr(g_DataDir.string().size() + 1);
				utils::make_lowercase(base_filepath);

				if (MAP.find(base_filepath) != MAP.end()) { // if we have a modfile for the file

					const auto& out_path = MAP.at(base_filepath);

					logging::log("[fs::fopen] Loading file: {} from mod: {}...", base_filepath, get_mod_for_file(base_filepath));

					return __fsopen((char*)out_path.string().c_str(), _Mode, 0x40); // return expected result
				}

				// If the file should be marked as deleted, then return nullptr.
				// this comes after swapping the path for modfiles, so if another mod (or
				// the mod itself) has that file it will still be loaded.
				if (std::find(MARK_AS_DELETED.begin(), MARK_AS_DELETED.end(), base_filepath) != MARK_AS_DELETED.end()) {
					return nullptr;
				}
			}
			return __fsopen(_Filename, _Mode, 0x40); // return expected result
		}
		std::filesystem::path parent_dir = std::filesystem::path(_Filename).parent_path();
		if (!std::filesystem::exists(parent_dir)) {
			std::filesystem::create_directories(parent_dir);
		}
		return __fsopen(_Filename, _Mode, 0x40); // return expected result
	}
};

/*
Fixes an issue where the game will erroneously strip away large portions of a path, while trying to replace its file extension.
*/
DefineReplacementHook(FixExtensionReplacement) {
	static void callback(char* buffer, char* extension) {
		std::string suffix(extension);
		auto start = suffix.find_first_of('.');

		std::filesystem::path path(buffer + suffix.substr(0, start));
		path.replace_extension(suffix.substr(start));
		std::string resolved = path.string();

		// (Below code is *VERY* unsafe but pls i don't know how else to do this)
		std::memcpy(buffer, resolved.c_str(), resolved.size());
		buffer[resolved.size()] = 0;
	}
};

DefineReplacementHook(TRCEngineGetSaveDir) {
	static void __fastcall callback(char* _this) {
		std::filesystem::path dir = fs::save_dir();
		if (!std::filesystem::is_directory(dir)) {
			std::filesystem::create_directories(dir);
		}
		std::string str = dir.string();
		std::memcpy(_this, str.data(), str.size());
		_this[str.size()] = 0;
	}
};

void fs::init() {
	discover_files();

	if (config::g_SaveRedirectionEnabled) {
		std::filesystem::path dir = fs::save_dir();
		if (dir.string().size() < 256) {
			TRCEngineGetSaveDir::install_at_ptr(0x00425280);
		}
		else {
			logging::log("[fs::init] Save redirection path: {} is too large, reverting to standard save data location!", dir.string());
			config::g_SaveRedirectionEnabled = false;
		}
	}

	std::memcpy(reinterpret_cast<char*>(0x006f6778), "Tex_HD\\", 7);

	FixExtensionReplacement::install_at_ptr(0x00409ec0);

	fopenHook::install_at_ptr(0x0063FCBF);

	winapi::set_permission(0x006742E8, 4, winapi::Perm::ReadWrite);
	*reinterpret_cast<void**>(0x006742E8) = BinkOpenHook;

	winapi::set_permission(0x00674084, 4, winapi::Perm::ReadWrite);
	*reinterpret_cast<void**>(0x00674084) = BASS_SampleLoadHook;

	winapi::set_permission(0x00674040, 4, winapi::Perm::ReadWrite);
	*reinterpret_cast<void**>(0x00674040) = BASS_StreamCreateFileHook;

	logging::log("[fs::init] Filesystem successfully initialized!");
}

std::filesystem::path fs::save_dir()
{
	if (config::g_SaveRedirectionEnabled) {
		return g_InstallDir / "LocalStorage\\SaveData\\";
	}
	else {
		wchar_t documents[1024];
		SHGetFolderPathW(0, CSIDL_MYDOCUMENTS, 0, 0, documents);
		return std::wstring(documents) + L"\\THQ\\Cars2\\";
	}
}
