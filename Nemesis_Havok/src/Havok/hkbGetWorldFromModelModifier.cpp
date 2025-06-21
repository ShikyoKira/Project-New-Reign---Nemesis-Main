#include "Havok/hkbGetWorldFromModelModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbGetWorldFromModelModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbGetWorldFromModelModifier::Class; });

nemesis::hkbGetWorldFromModelModifier::hkbGetWorldFromModelModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbGetWorldFromModelModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbGetWorldFromModelModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer); // offset: 0/0 size: 44/80 align: 4/8
    serializer.Pad(16);                            // offset: 44/80 size: 4/0

    serializer.WriteValue("translationOut", m_translationOut); // offset: 48/80 size: 16/16 align: 16/16
    serializer.WriteValue("rotationOut", m_rotationOut);       // offset: 64/96 size: 16/16 align: 16/16
    // class size: 80/112 align: 16/16
}

void nemesis::hkbGetWorldFromModelModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.Pad(16);                                // offset: 44/80 size: 4/0

    deserializer.ReadValue("translationOut", m_translationOut); // offset: 48/80 size: 16/16 align: 16/16
    deserializer.ReadValue("rotationOut", m_rotationOut);       // offset: 64/96 size: 16/16 align: 16/16
    // class size: 80/112 align: 16/16
}
