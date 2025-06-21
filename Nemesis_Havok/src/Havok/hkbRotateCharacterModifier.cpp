#include "Havok/hkbRotateCharacterModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbRotateCharacterModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbRotateCharacterModifier::Class; });

nemesis::hkbRotateCharacterModifier::hkbRotateCharacterModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbRotateCharacterModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbRotateCharacterModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer);                 // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteValue("degreesPerSecond", m_degreesPerSecond); // offset: 44/80 size: 4/4 align: 4/4
    serializer.WriteValue("speedMultiplier", m_speedMultiplier);   // offset: 48/84 size: 4/4 align: 4/4
    serializer.Pad(16);                                            // offset: 52/88 size: 12/8

    serializer.WriteValue("axisOfRotation", m_axisOfRotation); // offset: 64/96 size: 16/16 align: 16/16
    serializer.WriteSerializeIgnoredValue("angle", m_angle);   // offset: 80/112 size: 4/4 align: 4/4
    serializer.Skip(12);                                       // offset: 84/116 size: 12/12
    // class size: 96/128 align: 16/16
}

void nemesis::hkbRotateCharacterModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer);            // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadValue("degreesPerSecond", m_degreesPerSecond); // offset: 44/80 size: 4/4 align: 4/4
    deserializer.ReadValue("speedMultiplier", m_speedMultiplier);   // offset: 48/84 size: 4/4 align: 4/4
    deserializer.Pad(16);                                           // offset: 52/88 size: 12/8

    deserializer.ReadValue("axisOfRotation", m_axisOfRotation); // offset: 64/96 size: 16/16 align: 16/16
    deserializer.ReadValue("angle", m_angle);                   // offset: 80/112 size: 4/4 align: 4/4
    deserializer.Skip(12);                                      // offset: 84/116 size: 12/12
    // class size: 96/128 align: 16/16
}
