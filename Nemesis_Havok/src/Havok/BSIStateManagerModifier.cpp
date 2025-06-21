#include "Havok/BSIStateManagerModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(BSIStateManagerModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::BSIStateManagerModifier::Class; });

nemesis::BSIStateManagerModifier::BSIStateManagerModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::BSIStateManagerModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::BSIStateManagerModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer);   // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteValue("iStateVar", m_iStateVar); // offset: 44/80 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());     // offset: 48/84 size: 0/4

    serializer.WriteObject("stateData", m_stateData); // offset: 48/88 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("myStateListener",
                                           m_myStateListener); // offset: 60/104 size: 12/24 align: 4/8
    // class size: 72/128 align: 4/8
}

void nemesis::BSIStateManagerModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadValue("iStateVar", m_iStateVar);    // offset: 44/80 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());     // offset: 48/84 size: 0/4

    deserializer.ReadObject("stateData", m_stateData);             // offset: 48/88 size: 12/16 align: 4/8
    deserializer.ReadObject("myStateListener", m_myStateListener); // offset: 60/104 size: 12/24 align: 4/8
    // class size: 72/128 align: 4/8
}
