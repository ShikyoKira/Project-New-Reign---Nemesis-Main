#include "Havok/hkbStateMachine.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbStateMachine,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbStateMachine::Class; });

nemesis::hkbStateMachine::hkbStateMachine() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbGenerator(Class)
{
}

const nemesis::hkClass* nemesis::hkbStateMachine::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbStateMachine::SerializeTo(nemesis::Serializer& serializer) const
{
    unsigned int padding_size = serializer.GetPointerSize();

    nemesis::hkbGenerator::SerializeTo(serializer); // offset: 0/0 size: 40/72 align: 4/8
    serializer.Pad(padding_size);

    serializer.WriteObject("eventToSendWhenStateOrTransitionChanges",
                           m_eventToSendWhenStateOrTransitionChanges); // offset: 40/72 size: 12/24 align: 4/8

    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2013_2_0)
    {
        serializer.WriteObject("startStateIdSelector", m_startStateIdSelector);
    }
    else if (cur_ver > HavokVersion::HK_710)
    {
        serializer.WriteObject("startStateChooser",
                               m_startStateChooser); // offset: 52/96 size: 4/8 align: 4/8
    }

    serializer.WriteValue("startStateId", m_startStateId); // offset: 56/104 size: 4/4 align: 4/4
    serializer.WriteValue("returnToPreviousStateEventId",
                          m_returnToPreviousStateEventId); // offset: 60/108 size: 4/4 align: 4/4
    serializer.WriteValue("randomTransitionEventId",
                          m_randomTransitionEventId); // offset: 64/112 size: 4/4 align: 4/4
    serializer.WriteValue("transitionToNextHigherStateEventId",
                          m_transitionToNextHigherStateEventId); // offset: 68/116 size: 4/4 align: 4/4
    serializer.WriteValue("transitionToNextLowerStateEventId",
                          m_transitionToNextLowerStateEventId);      // offset: 72/120 size: 4/4 align: 4/4
    serializer.WriteValue("syncVariableIndex", m_syncVariableIndex); // offset: 76/124 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("currentStateId",
                                          m_currentStateId);         // offset: 80/128 size: 4/4 align: 4/4
    serializer.WriteValue("wrapAroundStateId", m_wrapAroundStateId); // offset: 84/132 size: 1/1 align: 1/1
    serializer.WriteValue("maxSimultaneousTransitions",
                          m_maxSimultaneousTransitions);               // offset: 85/133 size: 1/1 align: 1/1
    serializer.WriteValue("startStateMode", m_startStateMode);         // offset: 86/134 size: 1/1 align: 1/1
    serializer.WriteValue("selfTransitionMode", m_selfTransitionMode); // offset: 87/135 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("isActive", m_isActive);     // offset: 88/136 size: 1/1 align: 1/1
    serializer.Pad(padding_size);                                      // offset: 89/137 size: 3/7

    serializer.WriteObject("states", m_states); // offset: 92/144 size: 12/16 align: 4/8
    serializer.WriteObject("wildcardTransitions",
                           m_wildcardTransitions); // offset: 104/160 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("stateIdToIndexMap",
                                           m_stateIdToIndexMap); // offset: 108/168 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("activeTransitions",
                                           m_activeTransitions); // offset: 112/176 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("transitionFlags",
                                           m_transitionFlags); // offset: 124/192 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject(
        "wildcardTransitionFlags", m_wildcardTransitionFlags); // offset: 136/208 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("delayedTransitions",
                                           m_delayedTransitions); // offset: 148/224 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredValue("timeInState",
                                          m_timeInState); // offset: 160/240 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("lastLocalTime",
                                          m_lastLocalTime); // offset: 164/244 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("previousStateId",
                                          m_previousStateId); // offset: 168/248 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue(
        "nextStartStateIndexOverride", m_nextStartStateIndexOverride); // offset: 172/252 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("stateOrTransitionChanged",
                                          m_stateOrTransitionChanged); // offset: 176/256 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue("echoNextUpdate",
                                          m_echoNextUpdate); // offset: 177/257 size: 1/1 align: 1/1
    serializer.WriteSerializeIgnoredValue(
        "sCurrentStateIndexAndEntered",
        m_sCurrentStateIndexAndEntered); // offset: 178/258 size: 2/2 align: 2/2
    serializer.Pad(padding_size);        // offset: 180/260 size: 0/4
    // class size: 180/264 align: 4/8
}

void nemesis::hkbStateMachine::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    unsigned int padding_size = deserializer.GetPointerSize();

    nemesis::hkbGenerator::DeserializeFrom(deserializer); // offset: 0/0 size: 40/72 align: 4/8
    deserializer.Pad(padding_size);

    deserializer.ReadObject(
        "eventToSendWhenStateOrTransitionChanges",
        m_eventToSendWhenStateOrTransitionChanges); // offset: 40/72 size: 12/24 align: 4/8

    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2013_2_0)
    {
        deserializer.ReadObject("startStateIdSelector", m_startStateIdSelector);
    }
    else if (cur_ver > HavokVersion::HK_710)
    {
        deserializer.ReadObject("startStateChooser",
                                m_startStateChooser); // offset: 52/96 size: 4/8 align: 4/8
    }

    deserializer.ReadValue("startStateId", m_startStateId); // offset: 56/104 size: 4/4 align: 4/4
    deserializer.ReadValue("returnToPreviousStateEventId",
                           m_returnToPreviousStateEventId); // offset: 60/108 size: 4/4 align: 4/4
    deserializer.ReadValue("randomTransitionEventId",
                           m_randomTransitionEventId); // offset: 64/112 size: 4/4 align: 4/4
    deserializer.ReadValue("transitionToNextHigherStateEventId",
                           m_transitionToNextHigherStateEventId); // offset: 68/116 size: 4/4 align: 4/4
    deserializer.ReadValue("transitionToNextLowerStateEventId",
                           m_transitionToNextLowerStateEventId);      // offset: 72/120 size: 4/4 align: 4/4
    deserializer.ReadValue("syncVariableIndex", m_syncVariableIndex); // offset: 76/124 size: 4/4 align: 4/4
    deserializer.ReadValue("currentStateId", m_currentStateId);       // offset: 80/128 size: 4/4 align: 4/4
    deserializer.ReadValue("wrapAroundStateId", m_wrapAroundStateId); // offset: 84/132 size: 1/1 align: 1/1
    deserializer.ReadValue("maxSimultaneousTransitions",
                           m_maxSimultaneousTransitions);               // offset: 85/133 size: 1/1 align: 1/1
    deserializer.ReadValue("startStateMode", m_startStateMode);         // offset: 86/134 size: 1/1 align: 1/1
    deserializer.ReadValue("selfTransitionMode", m_selfTransitionMode); // offset: 87/135 size: 1/1 align: 1/1
    deserializer.ReadValue("isActive", m_isActive);                     // offset: 88/136 size: 1/1 align: 1/1
    deserializer.Pad(padding_size);                                     // offset: 89/137 size: 3/7

    deserializer.ReadObject("states", m_states); // offset: 92/144 size: 12/16 align: 4/8
    deserializer.ReadObject("wildcardTransitions",
                            m_wildcardTransitions);                    // offset: 104/160 size: 4/8 align: 4/8
    deserializer.ReadObject("stateIdToIndexMap", m_stateIdToIndexMap); // offset: 108/168 size: 4/8 align: 4/8
    deserializer.ReadObject("activeTransitions",
                            m_activeTransitions);                  // offset: 112/176 size: 12/16 align: 4/8
    deserializer.ReadObject("transitionFlags", m_transitionFlags); // offset: 124/192 size: 12/16 align: 4/8
    deserializer.ReadObject("wildcardTransitionFlags",
                            m_wildcardTransitionFlags); // offset: 136/208 size: 12/16 align: 4/8
    deserializer.ReadObject("delayedTransitions",
                            m_delayedTransitions);                // offset: 148/224 size: 12/16 align: 4/8
    deserializer.ReadValue("timeInState", m_timeInState);         // offset: 160/240 size: 4/4 align: 4/4
    deserializer.ReadValue("lastLocalTime", m_lastLocalTime);     // offset: 164/244 size: 4/4 align: 4/4
    deserializer.ReadValue("previousStateId", m_previousStateId); // offset: 168/248 size: 4/4 align: 4/4
    deserializer.ReadValue("nextStartStateIndexOverride",
                           m_nextStartStateIndexOverride); // offset: 172/252 size: 4/4 align: 4/4
    deserializer.ReadValue("stateOrTransitionChanged",
                           m_stateOrTransitionChanged);         // offset: 176/256 size: 1/1 align: 1/1
    deserializer.ReadValue("echoNextUpdate", m_echoNextUpdate); // offset: 177/257 size: 1/1 align: 1/1
    deserializer.ReadValue("sCurrentStateIndexAndEntered",
                           m_sCurrentStateIndexAndEntered); // offset: 178/258 size: 2/2 align: 2/2
    deserializer.Pad(padding_size);                         // offset: 180/260 size: 0/4
    // class size: 180/264 align: 4/8
}
