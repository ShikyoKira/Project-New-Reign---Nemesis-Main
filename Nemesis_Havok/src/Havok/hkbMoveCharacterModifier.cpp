#include "Havok/hkbMoveCharacterModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbMoveCharacterModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbMoveCharacterModifier::Class; });

nemesis::hkbMoveCharacterModifier::hkbMoveCharacterModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbMoveCharacterModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbMoveCharacterModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer); // offset: 0/0 size: 44/80 align: 4/8
    serializer.Pad(16);                            // offset: 44/80 size: 4/0

    serializer.WriteValue("offsetPerSecondMS", m_offsetPerSecondMS); // offset: 48/80 size: 16/16 align: 16/16
    serializer.WriteSerializeIgnoredValue("timeSinceLastModify",
                                          m_timeSinceLastModify); // offset: 64/96 size: 4/4 align: 4/4
    serializer.Skip(12);                                          // offset: 68/100 size: 12/12
    // class size: 80/112 align: 16/16
}

void nemesis::hkbMoveCharacterModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.Pad(16);                                // offset: 44/80 size: 4/0

    deserializer.ReadValue("offsetPerSecondMS",
                           m_offsetPerSecondMS); // offset: 48/80 size: 16/16 align: 16/16
    deserializer.ReadValue("timeSinceLastModify",
                           m_timeSinceLastModify); // offset: 64/96 size: 4/4 align: 4/4
    deserializer.Skip(12);                         // offset: 68/100 size: 12/12
    // class size: 80/112 align: 16/16
}
