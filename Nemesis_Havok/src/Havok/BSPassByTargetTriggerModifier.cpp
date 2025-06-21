#include "Havok/BSPassByTargetTriggerModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(BSPassByTargetTriggerModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::BSPassByTargetTriggerModifier::Class; });

nemesis::BSPassByTargetTriggerModifier::BSPassByTargetTriggerModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::BSPassByTargetTriggerModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::BSPassByTargetTriggerModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer); // offset: 0/0 size: 44/80 align: 4/8
    serializer.Pad(16);                            // offset: 44/80 size: 4/0

    serializer.WriteValue("targetPosition", m_targetPosition); // offset: 48/80 size: 16/16 align: 16/16
    serializer.WriteValue("radius", m_radius);                 // offset: 64/96 size: 4/4 align: 4/4
    serializer.Skip(12);                                       // offset: 68/100 size: 12/12

    serializer.WriteValue("movementDirection",
                          m_movementDirection);             // offset: 80/112 size: 16/16 align: 16/16
    serializer.WriteObject("triggerEvent", m_triggerEvent); // offset: 96/128 size: 8/16 align: 4/8
    serializer.WriteSerializeIgnoredValue("targetPassed",
                                          m_targetPassed); // offset: 104/144 size: 1/1 align: 1/1
    serializer.Pad(16);                                    // offset: 105/145 size: 7/15
    // class size: 112/160 align: 16/16
}

void nemesis::BSPassByTargetTriggerModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.Pad(16);                                // offset: 44/80 size: 4/0

    deserializer.ReadValue("targetPosition", m_targetPosition); // offset: 48/80 size: 16/16 align: 16/16
    deserializer.ReadValue("radius", m_radius);                 // offset: 64/96 size: 4/4 align: 4/4
    deserializer.Skip(12);                                      // offset: 68/100 size: 12/12

    deserializer.ReadValue("movementDirection",
                           m_movementDirection);             // offset: 80/112 size: 16/16 align: 16/16
    deserializer.ReadObject("triggerEvent", m_triggerEvent); // offset: 96/128 size: 8/16 align: 4/8
    deserializer.ReadValue("targetPassed", m_targetPassed);  // offset: 104/144 size: 1/1 align: 1/1
    deserializer.Pad(16);                                    // offset: 105/145 size: 7/15
    // class size: 112/160 align: 16/16
}
