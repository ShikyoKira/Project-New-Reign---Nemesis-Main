#include "Havok/hkpTriggerVolume.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpTriggerVolume,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpTriggerVolume::Class; });

nemesis::hkpTriggerVolume::hkpTriggerVolume() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
    , nemesis::hkpContactListener(Class)
    , nemesis::hkpWorldPostSimulationListener(Class)
    , nemesis::hkpEntityListener(Class)
{
}

const nemesis::hkClass* nemesis::hkpTriggerVolume::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpTriggerVolume::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);             // offset: 0/0 size: 8/16 align: 4/8
    nemesis::hkpContactListener::SerializeTo(serializer);             // offset: 8/16 size: 4/8 align: 4/8
    nemesis::hkpWorldPostSimulationListener::SerializeTo(serializer); // offset: 12/24 size: 4/8 align: 4/8
    nemesis::hkpEntityListener::SerializeTo(serializer);              // offset: 16/32 size: 4/8 align: 4/8
    serializer.WriteObject("overlappingBodies", m_overlappingBodies); // offset: 20/40 size: 12/16 align: 4/8
    serializer.WriteObject("eventQueue", m_eventQueue);               // offset: 32/56 size: 12/16 align: 4/8
    serializer.WriteObject("triggerBody", m_triggerBody);             // offset: 44/72 size: 4/8 align: 4/8
    serializer.WriteValue("sequenceNumber", m_sequenceNumber);        // offset: 48/80 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());                      // offset: 52/84 size: 0/4
    // class size: 52/88 align: 4/8
}

void nemesis::hkpTriggerVolume::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    nemesis::hkpContactListener::DeserializeFrom(deserializer); // offset: 8/16 size: 4/8 align: 4/8
    nemesis::hkpWorldPostSimulationListener::DeserializeFrom(
        deserializer);                                                 // offset: 12/24 size: 4/8 align: 4/8
    nemesis::hkpEntityListener::DeserializeFrom(deserializer);         // offset: 16/32 size: 4/8 align: 4/8
    deserializer.ReadObject("overlappingBodies", m_overlappingBodies); // offset: 20/40 size: 12/16 align: 4/8
    deserializer.ReadObject("eventQueue", m_eventQueue);               // offset: 32/56 size: 12/16 align: 4/8
    deserializer.ReadObject("triggerBody", m_triggerBody);             // offset: 44/72 size: 4/8 align: 4/8
    deserializer.ReadValue("sequenceNumber", m_sequenceNumber);        // offset: 48/80 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());                   // offset: 52/84 size: 0/4
    // class size: 52/88 align: 4/8
}
