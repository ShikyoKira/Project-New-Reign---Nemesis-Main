#pragma once

#include <bit>

#include "Havok/Base/hkVariant.h"

namespace nemesis
{
	struct hkHalf : nemesis::hkVariant
	{
    private:
        short m_value;

	public:
        hkHalf() = default;
        hkHalf(const float& f);

        nemesis::hkHalf& operator=(const float& f)
        {
            int i = std::bit_cast<int>(f);
            m_value = static_cast<short>(i >> 16);
            return *this;
        }
        
        nemesis::hkHalf& operator=(const short& s)
        {
            m_value = s;
            return *this;
        }

        void Store(float* f) const;

        float AsFloat() const;
        short AsShort() const;

        inline operator float() const
        {
            return AsFloat();
        }
	};
}
