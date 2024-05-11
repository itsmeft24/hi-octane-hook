#pragma once

#include <filesystem>

namespace fs {

	std::filesystem::path resolve_path(std::string path);

	void init();

	std::filesystem::path save_dir();

};