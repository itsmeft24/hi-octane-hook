#pragma once

namespace Logging {

	bool Init();

	void Log(const char* format, ...);

	void Deinit();
};