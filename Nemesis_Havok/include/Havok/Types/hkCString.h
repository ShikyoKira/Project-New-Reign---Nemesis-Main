#pragma once

#include "Havok/Base/hkStringBase.h"

namespace nemesis
{
    struct hkCString : nemesis::hkStringBase
    {
    public:
        hkCString();
        hkCString(const std::string& val);

        const std::string& SetValue(const std::string& val) override;
    };
}
