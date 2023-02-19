#pragma once

#include "base/havokdata.h"

namespace nemesis
{
    struct VarId : public HavokData
    {
        VarId(const std::string& name, size_t id);
    };
} // namespace nemesis
