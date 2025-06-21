#include "Havok/Types/hkHalf.h"

nemesis::hkHalf::hkHalf(const float& f)
{
    *this = f;
}

void nemesis::hkHalf::Store(float* f) const
{
    int i = static_cast<int>(m_value << 16);
    std::memcpy(&f, &i, sizeof(float));
}

float nemesis::hkHalf::AsFloat() const
{
    int i = static_cast<int>(m_value << 16);
    return std::_Bit_cast<float>(i);
}

short nemesis::hkHalf::AsShort() const
{
    return m_value;
}
