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

const std::string& nemesis::hkStringPtr::SetValue(const std::string& val)
{
    if (val == "\u2400")
    {
        Null = true;
        m_stringAndFlag.clear();
        return m_stringAndFlag;
    }

    Null                   = false;
    return m_stringAndFlag = val;
}
