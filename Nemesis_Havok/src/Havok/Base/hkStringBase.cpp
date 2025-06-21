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
