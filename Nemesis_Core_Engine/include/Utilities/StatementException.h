#pragma once

#include <string>

namespace nemesis
{
	struct StatementException : std::exception
	{
        StatementException(const std::string& msg)
            : std::exception(msg.c_str())
        {
        }
	};
}
