#pragma once

#include "Havok/Base/hkStringBase.h"

namespace nemesis
{
    struct hkCString : nemesis::hkStringBase
    {
    private:
        bool Null;

    public:
        hkCString();
        hkCString(const std::string& val);

        /// <summary>
        /// Set value to null
        /// </summary>
        void Clear();
        const std::string& SetValue(const std::string& val) override;

        bool IsNull() const noexcept;
    };
}
