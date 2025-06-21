#include "Havok/hkbStateMachineTransitionInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbStateMachineTransitionInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbStateMachineTransitionInfo::Class; });

nemesis::hkbStateMachineTransitionInfo::hkbStateMachineTransitionInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbStateMachineTransitionInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbStateMachineTransitionInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("triggerInterval", m_triggerInterval);    // offset: 0/0 size: 16/16 align: 4/4
    serializer.WriteObject("initiateInterval", m_initiateInterval);  // offset: 16/16 size: 16/16 align: 4/4
    serializer.WriteObject("transition", m_transition);              // offset: 32/32 size: 4/8 align: 4/8
    serializer.WriteObject("condition", m_condition);                // offset: 36/40 size: 4/8 align: 4/8
    serializer.WriteValue("eventId", m_eventId);                     // offset: 40/48 size: 4/4 align: 4/4
    serializer.WriteValue("toStateId", m_toStateId);                 // offset: 44/52 size: 4/4 align: 4/4
    serializer.WriteValue("fromNestedStateId", m_fromNestedStateId); // offset: 48/56 size: 4/4 align: 4/4
    serializer.WriteValue("toNestedStateId", m_toNestedStateId);     // offset: 52/60 size: 4/4 align: 4/4
    serializer.WriteValue("priority", m_priority);                   // offset: 56/64 size: 2/2 align: 2/2
    serializer.WriteValue("flags", m_flags);                         // offset: 58/66 size: 2/2 align: 2/2
    serializer.Pad(serializer.GetPointerSize());                     // offset: 60/68 size: 0/4
    // class size: 60/72 align: 4/8
}

void nemesis::hkbStateMachineTransitionInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("triggerInterval", m_triggerInterval);    // offset: 0/0 size: 16/16 align: 4/4
    deserializer.ReadObject("initiateInterval", m_initiateInterval);  // offset: 16/16 size: 16/16 align: 4/4
    deserializer.ReadObject("transition", m_transition);              // offset: 32/32 size: 4/8 align: 4/8
    deserializer.ReadObject("condition", m_condition);                // offset: 36/40 size: 4/8 align: 4/8
    deserializer.ReadValue("eventId", m_eventId);                     // offset: 40/48 size: 4/4 align: 4/4
    deserializer.ReadValue("toStateId", m_toStateId);                 // offset: 44/52 size: 4/4 align: 4/4
    deserializer.ReadValue("fromNestedStateId", m_fromNestedStateId); // offset: 48/56 size: 4/4 align: 4/4
    deserializer.ReadValue("toNestedStateId", m_toNestedStateId);     // offset: 52/60 size: 4/4 align: 4/4
    deserializer.ReadValue("priority", m_priority);                   // offset: 56/64 size: 2/2 align: 2/2
    deserializer.ReadValue("flags", m_flags);                         // offset: 58/66 size: 2/2 align: 2/2
    deserializer.Pad(deserializer.GetPointerSize());                  // offset: 60/68 size: 0/4
    // class size: 60/72 align: 4/8
}
