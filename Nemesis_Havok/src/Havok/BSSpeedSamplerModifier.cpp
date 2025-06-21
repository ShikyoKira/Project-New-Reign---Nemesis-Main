#include "Havok/BSSpeedSamplerModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(BSSpeedSamplerModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::BSSpeedSamplerModifier::Class; });

nemesis::BSSpeedSamplerModifier::BSSpeedSamplerModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::BSSpeedSamplerModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::BSSpeedSamplerModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer);   // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteValue("state", m_state);         // offset: 44/80 size: 4/4 align: 4/4
    serializer.WriteValue("direction", m_direction); // offset: 48/84 size: 4/4 align: 4/4
    serializer.WriteValue("goalSpeed", m_goalSpeed); // offset: 52/88 size: 4/4 align: 4/4
    serializer.WriteValue("speedOut", m_speedOut);   // offset: 56/92 size: 4/4 align: 4/4
    // class size: 60/96 align: 4/8
}

void nemesis::BSSpeedSamplerModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadValue("state", m_state);            // offset: 44/80 size: 4/4 align: 4/4
    deserializer.ReadValue("direction", m_direction);    // offset: 48/84 size: 4/4 align: 4/4
    deserializer.ReadValue("goalSpeed", m_goalSpeed);    // offset: 52/88 size: 4/4 align: 4/4
    deserializer.ReadValue("speedOut", m_speedOut);      // offset: 56/92 size: 4/4 align: 4/4
    // class size: 60/96 align: 4/8
}
