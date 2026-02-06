#include "Havok/Base/hkStringBase.h"

nemesis::hkStringBase::hkStringBase()
{
}

nemesis::hkStringBase::hkStringBase(const std::string& val)
    : m_stringAndFlag(val)
{
}

const std::string& nemesis::hkStringBase::GetValue() const noexcept
{
    return m_stringAndFlag;
}

const std::string& nemesis::hkStringBase::SetValue(const std::string& val)
{
    return m_stringAndFlag = val;
}

bool nemesis::hkStringBase::operator==(const std::string& str) const
{
    return m_stringAndFlag == str;
}

bool nemesis::hkStringBase::operator==(const nemesis::hkStringBase& str) const
{
    return m_stringAndFlag == str.m_stringAndFlag;
}

bool nemesis::hkStringBase::operator!=(const std::string& str) const
{
    return m_stringAndFlag != str;
}

bool nemesis::hkStringBase::operator!=(const nemesis::hkStringBase& str) const
{
    return m_stringAndFlag != str.m_stringAndFlag;
}
