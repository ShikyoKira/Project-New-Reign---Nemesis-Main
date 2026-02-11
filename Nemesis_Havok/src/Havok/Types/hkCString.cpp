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

const std::string& nemesis::hkCString::SetValue(const std::string& val)
{
    Null                   = val.empty() || val == "\u2400";
    return m_stringAndFlag = val;
}
