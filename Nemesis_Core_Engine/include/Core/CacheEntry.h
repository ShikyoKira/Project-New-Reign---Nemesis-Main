#pragma once

#include <chrono>
#include <string>

namespace nemesis
{
	struct CacheEntry
	{
        std::chrono::system_clock::time_point CreateTime;
        std::string Data;
	};
}
