#include "Havok/hkbSetWorldFromModelModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbSetWorldFromModelModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbSetWorldFromModelModifier::Class; });

nemesis::hkbSetWorldFromModelModifier::hkbSetWorldFromModelModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbSetWorldFromModelModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbSetWorldFromModelModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer); // offset: 0/0 size: 44/80 align: 4/8
    serializer.Pad(16);                            // offset: 44/80 size: 4/0

    serializer.WriteValue("translation", m_translation);       // offset: 48/80 size: 16/16 align: 16/16
    serializer.WriteValue("rotation", m_rotation);             // offset: 64/96 size: 16/16 align: 16/16
    serializer.WriteValue("setTranslation", m_setTranslation); // offset: 80/112 size: 1/1 align: 1/1
    serializer.WriteValue("setRotation", m_setRotation);       // offset: 81/113 size: 1/1 align: 1/1
    serializer.Skip(14);                                       // offset: 82/114 size: 14/14
    // class size: 96/128 align: 16/16
}

void nemesis::hkbSetWorldFromModelModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.Pad(16);                                // offset: 44/80 size: 4/0

    deserializer.ReadValue("translation", m_translation);       // offset: 48/80 size: 16/16 align: 16/16
    deserializer.ReadValue("rotation", m_rotation);             // offset: 64/96 size: 16/16 align: 16/16
    deserializer.ReadValue("setTranslation", m_setTranslation); // offset: 80/112 size: 1/1 align: 1/1
    deserializer.ReadValue("setRotation", m_setRotation);       // offset: 81/113 size: 1/1 align: 1/1
    deserializer.Skip(14);                                      // offset: 82/114 size: 14/14
    // class size: 96/128 align: 16/16
}
