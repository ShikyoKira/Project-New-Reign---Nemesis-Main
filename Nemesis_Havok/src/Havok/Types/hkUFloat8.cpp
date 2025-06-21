#include "Havok/Types/hkUFloat8.h"

#include "Serialize/Deserializer.h"

nemesis::hkUFloat8::hkUFloat8(const float f)
{
    *this = f;
}

nemesis::hkUFloat8::hkUFloat8(const double d)
{
    *this = d;
}

nemesis::hkUFloat8& nemesis::hkUFloat8::operator=(const double& dv)
{
    return this->operator=(static_cast<float>(dv));
}

nemesis::hkUFloat8& nemesis::hkUFloat8::operator=(const float& fv)
{
    m_value = GetClosestIndex(fv);
    return *this;
}

nemesis::hkUFloat8& nemesis::hkUFloat8::operator=(const unsigned char& b)
{
    m_value = b;
    return *this;
}

bool nemesis::hkUFloat8::IsZero() const noexcept
{
    return m_value == 0;
}

void nemesis::hkUFloat8::SetZero() noexcept
{
    m_value = 0;
}

bool nemesis::hkUFloat8::IsMax() const noexcept
{
    return m_value == 255;
}

void nemesis::hkUFloat8::SetMax() noexcept
{
    m_value = 255;
}

bool nemesis::hkUFloat8::operator==(const hkUFloat8& other) const noexcept
{
    return m_value == other.m_value;
}

void nemesis::hkUFloat8::SetArithmeticMean(const hkUFloat8& a, const hkUFloat8& b)
{
    m_value = static_cast<unsigned char>((static_cast<int>(a.m_value) + static_cast<int>(b.m_value)) >> 1);
}

float nemesis::hkUFloat8::AsFloat() const
{
    return DecodeFloat(GetEncodedFloat(m_value));
}

unsigned char nemesis::hkUFloat8::AsByte() const
{
    return m_value;
}
