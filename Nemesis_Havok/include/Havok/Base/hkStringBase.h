#pragma once

#include <string>

#include "Havok/Base/hkVariant.h"

namespace nemesis
{
    struct hkStringBase : nemesis::hkVariant
    {
    protected:
        bool Null = true;
        std::string m_stringAndFlag;

    public:
        hkStringBase();
        hkStringBase(const std::string& val);

        virtual void Clear();
        virtual const std::string& GetValue() const noexcept;
        virtual const std::string& SetValue(const std::string& val);

        bool operator==(const std::string& str) const;
        bool operator==(const nemesis::hkStringBase& str) const;
        bool operator!=(const std::string& str) const;
        bool operator!=(const nemesis::hkStringBase& str) const;

        bool IsNull() const noexcept;
    };
}
