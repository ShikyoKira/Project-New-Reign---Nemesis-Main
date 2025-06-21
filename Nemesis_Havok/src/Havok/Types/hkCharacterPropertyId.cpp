#include "Havok/Types/hkCharacterPropertyId.h"

nemesis::hkCharacterPropertyId&
nemesis::hkCharacterPropertyId::operator=(const nemesis::hkCharacterPropertyId& prop_id)
{
    Value = prop_id;
    return *this;
}

nemesis::hkCharacterPropertyId& nemesis::hkCharacterPropertyId::operator=(int prop_id)
{
    Value = prop_id;
    return *this;
}

nemesis::hkCharacterPropertyId::operator int() const
{
    return Value;
}
