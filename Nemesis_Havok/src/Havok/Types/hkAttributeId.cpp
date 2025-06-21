#include "Havok/Types/hkAttributeId.h"

nemesis::hkAttributeId& nemesis::hkAttributeId::operator=(const nemesis::hkAttributeId& attr_id)
{
    Value = attr_id;
    return *this;
}

nemesis::hkAttributeId& nemesis::hkAttributeId::operator=(int attr_id)
{
    Value = attr_id;
    return *this;
}

nemesis::hkAttributeId::operator int() const
{
    return Value;
}
