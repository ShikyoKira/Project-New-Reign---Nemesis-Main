#pragma once

#include <string>

namespace nemesis
{
	struct AnimValue
    {
        virtual std::string GetResult(nemesis::ScopeInfo& scopeinfo) const = 0;
    };
}
