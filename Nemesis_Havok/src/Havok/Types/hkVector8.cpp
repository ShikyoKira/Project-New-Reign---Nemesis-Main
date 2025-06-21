#include "Havok/Types/hkVector8.h"

nemesis::hkVector8::hkVector8(const nemesis::hkVector4& lin_vec4, const nemesis::hkVector4& ang_vec4) noexcept
    : m_lin(lin_vec4)
    , m_ang(ang_vec4)
{
}

nemesis::hkVector4& nemesis::hkVector8::GetLin()
{
    return m_lin;
}

nemesis::hkVector4& nemesis::hkVector8::GetAng()
{
    return m_ang;
}

const nemesis::hkVector4& nemesis::hkVector8::GetLin() const
{
    return m_lin;
}

const nemesis::hkVector4& nemesis::hkVector8::GetAng() const
{
    return m_ang;
}
