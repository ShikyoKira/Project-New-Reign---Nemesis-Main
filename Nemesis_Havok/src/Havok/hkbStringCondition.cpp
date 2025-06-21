#include "Havok/hkbStringCondition.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbStringCondition,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbStringCondition::Class; });

nemesis::hkbStringCondition::hkbStringCondition() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbCondition(Class)
{
}

const nemesis::hkClass* nemesis::hkbStringCondition::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbStringCondition::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbCondition::SerializeTo(serializer);              // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("conditionString", m_conditionString); // offset: 8/16 size: 4/8 align: 4/8
    // class size: 12/24 align: 4/8
}

void nemesis::hkbStringCondition::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbCondition::DeserializeFrom(deserializer);         // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("conditionString", m_conditionString); // offset: 8/16 size: 4/8 align: 4/8
    // class size: 12/24 align: 4/8
}
