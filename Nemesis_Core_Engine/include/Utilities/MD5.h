#pragma once

#include <string>

namespace nemesis
{
	namespace md5
	{
        std::string hash_to_string(const std::string& message);
        std::wstring hash_to_string(const std::wstring& message);
	}
}
