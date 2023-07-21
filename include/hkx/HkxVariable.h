#pragma once

#include "hkx/base/HavokData.h"

namespace nemesis
{
    struct HkxVariable : public nemesis::HavokData
    {
        HkxVariable(const std::string& name, size_t id);
    };
}
