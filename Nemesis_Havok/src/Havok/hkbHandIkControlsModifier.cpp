#include "Havok/hkbHandIkControlsModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbHandIkControlsModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbHandIkControlsModifier::Class; });

nemesis::hkbHandIkControlsModifier::hkbHandIkControlsModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbHandIkControlsModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbHandIkControlsModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer); // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteObject("hands", m_hands);      // offset: 44/80 size: 12/16 align: 4/8
    // class size: 56/96 align: 4/8
}

void nemesis::hkbHandIkControlsModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadObject("hands", m_hands);           // offset: 44/80 size: 12/16 align: 4/8
    // class size: 56/96 align: 4/8
}
