#include "Havok/Types/hkCString.h"

nemesis::hkCString::hkCString()
{
    Null = true;
}

nemesis::hkCString::hkCString(const std::string& val)
    : nemesis::hkStringBase(val)
{
    Null = false;
}

void nemesis::hkCString::Clear()
{
    Null = true;
    m_stringAndFlag.clear();
}

const std::string& nemesis::hkCString::SetValue(const std::string& val)
{
    Null                   = false;
    return m_stringAndFlag = val;
}

bool nemesis::hkCString::IsNull() const noexcept
{
    return Null;
}
