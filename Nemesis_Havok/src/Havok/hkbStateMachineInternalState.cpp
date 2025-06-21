#include "Havok/hkbStateMachineInternalState.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbStateMachineInternalState,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbStateMachineInternalState::Class; });

nemesis::hkbStateMachineInternalState::hkbStateMachineInternalState() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbStateMachineInternalState::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbStateMachineInternalState::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);             // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("activeTransitions", m_activeTransitions); // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteObject("transitionFlags", m_transitionFlags);     // offset: 20/32 size: 12/16 align: 4/8
    serializer.WriteObject("wildcardTransitionFlags",
                           m_wildcardTransitionFlags); // offset: 32/48 size: 12/16 align: 4/8
    serializer.WriteObject("delayedTransitions",
                           m_delayedTransitions);                // offset: 44/64 size: 12/16 align: 4/8
    serializer.WriteValue("timeInState", m_timeInState);         // offset: 56/80 size: 4/4 align: 4/4
    serializer.WriteValue("lastLocalTime", m_lastLocalTime);     // offset: 60/84 size: 4/4 align: 4/4
    serializer.WriteValue("currentStateId", m_currentStateId);   // offset: 64/88 size: 4/4 align: 4/4
    serializer.WriteValue("previousStateId", m_previousStateId); // offset: 68/92 size: 4/4 align: 4/4
    serializer.WriteValue("nextStartStateIndexOverride",
                          m_nextStartStateIndexOverride); // offset: 72/96 size: 4/4 align: 4/4
    serializer.WriteValue("stateOrTransitionChanged",
                          m_stateOrTransitionChanged);         // offset: 76/100 size: 1/1 align: 1/1
    serializer.WriteValue("echoNextUpdate", m_echoNextUpdate); // offset: 77/101 size: 1/1 align: 1/1
    serializer.Skip(2);                                        // offset: 78/102 size: 2/2
    // class size: 80/104 align: 4/8
}

void nemesis::hkbStateMachineInternalState::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer);        // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("activeTransitions", m_activeTransitions); // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadObject("transitionFlags", m_transitionFlags);     // offset: 20/32 size: 12/16 align: 4/8
    deserializer.ReadObject("wildcardTransitionFlags",
                            m_wildcardTransitionFlags); // offset: 32/48 size: 12/16 align: 4/8
    deserializer.ReadObject("delayedTransitions",
                            m_delayedTransitions);                // offset: 44/64 size: 12/16 align: 4/8
    deserializer.ReadValue("timeInState", m_timeInState);         // offset: 56/80 size: 4/4 align: 4/4
    deserializer.ReadValue("lastLocalTime", m_lastLocalTime);     // offset: 60/84 size: 4/4 align: 4/4
    deserializer.ReadValue("currentStateId", m_currentStateId);   // offset: 64/88 size: 4/4 align: 4/4
    deserializer.ReadValue("previousStateId", m_previousStateId); // offset: 68/92 size: 4/4 align: 4/4
    deserializer.ReadValue("nextStartStateIndexOverride",
                           m_nextStartStateIndexOverride); // offset: 72/96 size: 4/4 align: 4/4
    deserializer.ReadValue("stateOrTransitionChanged",
                           m_stateOrTransitionChanged);         // offset: 76/100 size: 1/1 align: 1/1
    deserializer.ReadValue("echoNextUpdate", m_echoNextUpdate); // offset: 77/101 size: 1/1 align: 1/1
    deserializer.Skip(2);                                       // offset: 78/102 size: 2/2
    // class size: 80/104 align: 4/8
}
