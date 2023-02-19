#pragma once

#include "core/animvalue.h"

namespace nemesis
{
	struct StringValue : public AnimValue
    {
    private:
        std::string base;

    public:
        std::string GetResult(nemesis::ScopeInfo& scopeinfo) const;
    };
}
