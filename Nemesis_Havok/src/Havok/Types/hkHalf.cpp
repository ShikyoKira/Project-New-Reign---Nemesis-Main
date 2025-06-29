#include "Havok/Types/hkHalf.h"

nemesis::hkHalf::hkHalf(const float& f)
{
    *this = f;
}

void nemesis::hkHalf::Store(float* f) const
{
    int i = static_cast<int>(m_value << 16);
    *f    = std::bit_cast<float>(i);
}

float nemesis::hkHalf::AsFloat() const
{
    int i = static_cast<int>(m_value << 16);
    return std::bit_cast<float>(i);
}

short nemesis::hkHalf::AsShort() const
{
    return m_value;
}
