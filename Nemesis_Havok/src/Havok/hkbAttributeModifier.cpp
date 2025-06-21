#include "Havok/hkbAttributeModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbAttributeModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbAttributeModifier::Class; });

nemesis::hkbAttributeModifier::hkbAttributeModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbAttributeModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbAttributeModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer);        // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteObject("assignments", m_assignments); // offset: 44/80 size: 12/16 align: 4/8
    // class size: 56/96 align: 4/8
}

void nemesis::hkbAttributeModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer);   // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadObject("assignments", m_assignments); // offset: 44/80 size: 12/16 align: 4/8
    // class size: 56/96 align: 4/8
}
