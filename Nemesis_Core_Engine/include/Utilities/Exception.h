#pragma once

#include <string>

namespace nemesis
{
    struct Exception : std::exception
    {
        Exception(const std::string& msg)
            : std::exception(msg.c_str())
        {
        }
	};
}