#include "Havok/BSEventOnDeactivateModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(BSEventOnDeactivateModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::BSEventOnDeactivateModifier::Class; });

nemesis::BSEventOnDeactivateModifier::BSEventOnDeactivateModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::BSEventOnDeactivateModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::BSEventOnDeactivateModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer); // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteObject("event", m_event);      // offset: 44/80 size: 8/16 align: 4/8
    // class size: 52/96 align: 4/8
}

void nemesis::BSEventOnDeactivateModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadObject("event", m_event);           // offset: 44/80 size: 8/16 align: 4/8
    // class size: 52/96 align: 4/8
}
