#pragma once

#include "hkx/base/HavokData.h"

namespace nemesis
{
    struct HkxAttribute : public nemesis::HavokData
    {
        HkxAttribute(const std::string& name, size_t id);
    };
}
