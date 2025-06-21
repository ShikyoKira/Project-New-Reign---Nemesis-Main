#pragma once

#include <string>

#include "Havok/Base/hkVariant.h"

namespace nemesis
{
    struct hkStringBase : nemesis::hkVariant
    {
    protected:
        std::string m_stringAndFlag;

    public:
        hkStringBase();
        hkStringBase(const std::string& val);

        virtual const std::string& GetValue() const noexcept;
        virtual const std::string& SetValue(const std::string& val);
    };
}
