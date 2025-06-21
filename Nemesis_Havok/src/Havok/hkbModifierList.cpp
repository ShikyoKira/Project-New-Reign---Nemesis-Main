#include "Havok/hkbModifierList.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbModifierList,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbModifierList::Class; });

nemesis::hkbModifierList::hkbModifierList() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbModifierList::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbModifierList::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer);    // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteObject("modifiers", m_modifiers); // offset: 44/80 size: 12/16 align: 4/8
    // class size: 56/96 align: 4/8
}

void nemesis::hkbModifierList::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadObject("modifiers", m_modifiers);   // offset: 44/80 size: 12/16 align: 4/8
    // class size: 56/96 align: 4/8
}
