#include "Havok/Types/hkStringPtr.h"

nemesis::hkStringPtr::hkStringPtr()
{
    Null = true;
}

nemesis::hkStringPtr::hkStringPtr(const std::string& val)
    : nemesis::hkStringBase(val)
{
    Null = false;
}

nemesis::hkStringPtr::hkStringPtr(const nemesis::hkStringPtr& val)
{
    Null = val.Null;
    m_stringAndFlag = val.m_stringAndFlag;
}

nemesis::hkStringPtr& nemesis::hkStringPtr::operator=(const nemesis::hkStringPtr& val)
{
    Null            = val.Null;
    m_stringAndFlag = val.m_stringAndFlag;
    return *this;
}

void nemesis::hkStringPtr::Clear()
{
    Null = true;
    m_stringAndFlag.clear();
}

const std::string& nemesis::hkStringPtr::SetValue(const std::string& val)
{
    Null = false;
    return m_stringAndFlag = val;
}

bool nemesis::hkStringPtr::IsNull() const noexcept
{
    return Null;
}
