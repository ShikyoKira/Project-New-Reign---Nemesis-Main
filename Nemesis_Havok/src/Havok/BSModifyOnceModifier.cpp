#include "Havok/BSModifyOnceModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(BSModifyOnceModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::BSModifyOnceModifier::Class; });

nemesis::BSModifyOnceModifier::BSModifyOnceModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::BSModifyOnceModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::BSModifyOnceModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer); // offset: 0/0 size: 44/80 align: 4/8
    serializer.Pad(16);                            // offset: 44/80 size: 4/0

    serializer.WriteObject("pOnActivateModifier",
                           m_pOnActivateModifier); // offset: 48/80 size: 4/8 align: 16/16
    serializer.Pad(16);                            // offset: 52/88 size: 12/8

    serializer.WriteObject("pOnDeactivateModifier",
                           m_pOnDeactivateModifier); // offset: 64/96 size: 4/8 align: 16/16
    serializer.Pad(16);                              // offset: 68/104 size: 12/8
    // class size: 80/112 align: 16/16
}

void nemesis::BSModifyOnceModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.Pad(16);                                // offset: 44/80 size: 4/0

    deserializer.ReadObject("pOnActivateModifier",
                            m_pOnActivateModifier); // offset: 48/80 size: 4/8 align: 16/16
    deserializer.Pad(16);                           // offset: 52/88 size: 12/8

    deserializer.ReadObject("pOnDeactivateModifier",
                            m_pOnDeactivateModifier); // offset: 64/96 size: 4/8 align: 16/16
    deserializer.Pad(16);                             // offset: 68/104 size: 12/8
    // class size: 80/112 align: 16/16
}
