#pragma once
#include <iostream>
#include <Windows.h>

namespace plugin_manager {

	struct Plugin {
		std::string name;
		HMODULE handle;

		void start();

		void exit();
	};

	void load_plugins();

	void start_plugins();

	void exit_plugins();

};