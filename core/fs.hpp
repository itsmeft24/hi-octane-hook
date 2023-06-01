#pragma once

#include <filesystem>

namespace fs {

	struct CallbackContext {
	  const char *file_path;
	  void *data_ptr;
	  unsigned long max_file_size;
	  unsigned long file_size;
	};

	struct CallbackInfo {
	  unsigned long max_file_size;
	  void *callback;
	};

	typedef int (*CarsFileCallback)(CallbackContext *);

	std::filesystem::path resolve_path(std::string path);

	void init();

	std::filesystem::path save_dir();

};