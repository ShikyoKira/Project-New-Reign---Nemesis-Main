#include "Havok/hkbStateMachineTransitionInfoReference.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbStateMachineTransitionInfoReference,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbStateMachineTransitionInfoReference::Class; });

nemesis::hkbStateMachineTransitionInfoReference::hkbStateMachineTransitionInfoReference() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbStateMachineTransitionInfoReference::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbStateMachineTransitionInfoReference::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("fromStateIndex", m_fromStateIndex);   // offset: 0/0 size: 2/2 align: 2/2
    serializer.WriteValue("transitionIndex", m_transitionIndex); // offset: 2/2 size: 2/2 align: 2/2
    serializer.WriteValue("stateMachineId", m_stateMachineId);   // offset: 4/4 size: 2/2 align: 2/2
    // class size: 6/6 align: 2/2
}

void nemesis::hkbStateMachineTransitionInfoReference::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("fromStateIndex", m_fromStateIndex);   // offset: 0/0 size: 2/2 align: 2/2
    deserializer.ReadValue("transitionIndex", m_transitionIndex); // offset: 2/2 size: 2/2 align: 2/2
    deserializer.ReadValue("stateMachineId", m_stateMachineId);   // offset: 4/4 size: 2/2 align: 2/2
    // class size: 6/6 align: 2/2
}
