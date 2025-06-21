#pragma once

#include "Havok/Base/hkStringBase.h"

namespace nemesis
{
    struct hkStringPtr : nemesis::hkStringBase
    {
    private:
        bool Null;

    public:
        hkStringPtr();
        hkStringPtr(const std::string& val);
        hkStringPtr(const nemesis::hkStringPtr& val);

        nemesis::hkStringPtr& operator=(const nemesis::hkStringPtr& val);

        /// <summary>
        /// Set value to null
        /// </summary>
        void Clear();
        const std::string& SetValue(const std::string& val) override;

        bool IsNull() const noexcept;
    };
}
