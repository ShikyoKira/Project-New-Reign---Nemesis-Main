#pragma once

#include "base/havokdata.h"

namespace nemesis
{
    struct AttrId : public HavokData
    {
        AttrId(const std::string& name, size_t id);
    };
} // namespace nemesis
