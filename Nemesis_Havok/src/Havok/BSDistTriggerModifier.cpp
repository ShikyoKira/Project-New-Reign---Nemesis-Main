#include "Havok/BSDistTriggerModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(BSDistTriggerModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::BSDistTriggerModifier::Class; });

nemesis::BSDistTriggerModifier::BSDistTriggerModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::BSDistTriggerModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::BSDistTriggerModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer); // offset: 0/0 size: 44/80 align: 4/8
    serializer.Pad(16);                            // offset: 44/80 size: 4/0

    serializer.WriteValue("targetPosition", m_targetPosition);   // offset: 48/80 size: 16/16 align: 16/16
    serializer.WriteValue("distance", m_distance);               // offset: 64/96 size: 4/4 align: 4/4
    serializer.WriteValue("distanceTrigger", m_distanceTrigger); // offset: 68/100 size: 4/4 align: 4/4
    serializer.WriteObject("triggerEvent", m_triggerEvent);      // offset: 72/104 size: 8/16 align: 4/8
    serializer.Pad(16);                                          // offset: 80/120 size: 0/8
    // class size: 80/128 align: 16/16
}

void nemesis::BSDistTriggerModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.Pad(16);                                // offset: 44/80 size: 4/0

    deserializer.ReadValue("targetPosition", m_targetPosition);   // offset: 48/80 size: 16/16 align: 16/16
    deserializer.ReadValue("distance", m_distance);               // offset: 64/96 size: 4/4 align: 4/4
    deserializer.ReadValue("distanceTrigger", m_distanceTrigger); // offset: 68/100 size: 4/4 align: 4/4
    deserializer.ReadObject("triggerEvent", m_triggerEvent);      // offset: 72/104 size: 8/16 align: 4/8
    deserializer.Pad(16);                                         // offset: 80/120 size: 0/8
    // class size: 80/128 align: 16/16
}
