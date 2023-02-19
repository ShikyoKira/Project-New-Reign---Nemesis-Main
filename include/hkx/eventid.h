#pragma once

#include "base/havokdata.h"

namespace nemesis
{
	struct EventId : public HavokData
    {
        EventId(const std::string& name, size_t id);
    };
}
