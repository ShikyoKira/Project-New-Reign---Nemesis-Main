#pragma once

#include "hkx/base/HavokData.h"

namespace nemesis
{
    struct HkxEvent : public nemesis::HavokData
    {
        HkxEvent(const std::string& name, size_t id);
    };
}
