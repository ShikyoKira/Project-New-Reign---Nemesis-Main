#pragma once

#include "Havok/Base/hkStringBase.h"

namespace nemesis
{
    struct hkStringPtr : nemesis::hkStringBase
    {
    public:
        hkStringPtr();
        hkStringPtr(const std::string& val);
        hkStringPtr(const nemesis::hkStringPtr& val);

        nemesis::hkStringPtr& operator=(const nemesis::hkStringPtr& val);

        const std::string& SetValue(const std::string& val) override;
    };
}
