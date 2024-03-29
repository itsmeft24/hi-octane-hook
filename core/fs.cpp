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

std::unordered_map<std::string, fs::CallbackInfo> InstalledCallbacks;

std::unordered_map<void*, std::filesystem::path> FileBacking;

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

std::filesystem::path generate_file_backing_path() {
	std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

	std::random_device rd;
	std::mt19937 generator(rd());

	std::shuffle(str.begin(), str.end(), generator);

	if (!std::filesystem::exists(g_InstallDir / "LocalStorage\\FileBacking\\")) {
		std::filesystem::create_directories(g_InstallDir / "LocalStorage\\FileBacking\\");
	}

	return g_InstallDir / "LocalStorage\\FileBacking" /
		(str.substr(0, 16) + ".tmp"); // assumes 32 < number of characters in str
}

inline void* handle_callback(fs::CallbackInfo& info,
	std::string& base_filepath, char* _Mode) {

	void* data = malloc(info.max_file_size); // allocate buffer for callback

	fs::CallbackContext clbkcontext{ base_filepath.c_str(), data, info.max_file_size, 0 };

	logging::log("[fs::handle_callback] Executing file callback for: {}...", base_filepath);

	fs::CarsFileCallback clbk = (fs::CarsFileCallback)InstalledCallbacks.at(base_filepath).callback; // execute callback function

	if (clbk(&clbkcontext) != 0) {

		auto filebackingpath = generate_file_backing_path(); // generate random string for filebacking

		std::ofstream f(filebackingpath, std::ios::out | std::ios::binary); // create file and write modified data to it
		f.write((char*)data, clbkcontext .file_size); // size SHOULD be modified by the callback function
		free(data); // free allocated buffer (plugin SHOULD NOT hold on to this pointer)
		f.close();

		auto ret = __fsopen((char*)filebackingpath.c_str(), _Mode, 0x40); // return expected result

		FileBacking.insert_or_assign( ret, filebackingpath); // keep track of file pointers used in callback handling

		return ret;
	}
	return nullptr;
}

// Registers a file callback. Takes in three arguments: an asset file path
// string, a maximum buffer size, and a callback function pointer.
HIOCTANE_API void __stdcall HiOctane_RegisterFileCallback(const char* filepath, size_t max_size, fs::CarsFileCallback callbackfunc) {
	InstalledCallbacks.insert_or_assign(filepath, fs::CallbackInfo{ max_size, callbackfunc });
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
				for (const auto& entry :
					std::filesystem::recursive_directory_iterator(mod)) {
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

				// If the file is a UI texture AND we are running in widescreen, use UI\Tex_HD instead of UI\Tex
				if (_strnicmp(_Filename + g_DataDir.string().size() + 2, "\\ui\\tex\\", 8) == 0 && config::g_WidescreenEnabled) {
					std::string new_filepath = base_filepath.substr(0, 8) + "_hd" + base_filepath.substr(8);
					if (std::filesystem::exists(g_DataDir / new_filepath) || MAP.find(new_filepath) != MAP.end() || InstalledCallbacks.find(new_filepath) != InstalledCallbacks.end()) {
						memcpy(_Filename + g_DataDir.string().size() + 1, new_filepath.c_str(), new_filepath.size());
						*(_Filename + g_DataDir.string().size() + 1 + new_filepath.size()) = 0;
						base_filepath = new_filepath;
					}
				}

				// If we have an installed callback for the file, handle it first.
				// (Callbacks take priority over normal files no matter what)
				if (InstalledCallbacks.find(base_filepath) != InstalledCallbacks.end()) {

					// If the callback succeeded, return the expected result.
					void* ret = handle_callback(InstalledCallbacks.at(base_filepath), base_filepath, _Mode);
					if (ret != nullptr) {
						return ret;
					}
					// If the callback failed, continue as normal.
				}

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

DefineReplacementHook(fcloseHook) {
	static int __cdecl callback(void* fp) {
		if (FileBacking.find(fp) != FileBacking.end()) {

			int ret = original(fp);

			std::filesystem::remove(FileBacking.at(fp));

			FileBacking.erase(fp);

			return ret;
		}
		return original(fp);
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

void fs::init() {
	discover_files();

	FixExtensionReplacement::install_at_ptr(0x00409ec0);

	fopenHook::install_at_ptr(0x0063FCBF);
	fcloseHook::install_at_ptr(0x0063FD94);

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
	wchar_t documents[1024];
	SHGetFolderPathW(0, CSIDL_MYDOCUMENTS, 0, 0, documents);
	return std::wstring(documents) + L"\\THQ\\Cars2\\";
}
