#pragma once

#include "Havok/Base/hkbGenerator.h"

#include "Havok/hkbCustomIdSelector.h"
#include "Havok/hkbEvent.h"
#include "Havok/hkbStateChooser.h"
#include "Havok/hkbStateMachineStateInfo.h"

namespace nemesis
{
    struct hkbStateMachine : nemesis::hkbGenerator
    {
        static constexpr nemesis::hkClass Class{0x816c1dcb,
                                                "hkbStateMachine",
                                                &nemesis::hkbGenerator::Class,
                                                264,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbStateMachine);

        using StartStateMode                 = nemesis::hkInternalClassEnumItem<char>;
        using StateMachineSelfTransitionMode = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkbEvent m_eventToSendWhenStateOrTransitionChanges;
        nemesis::hkRefPtr<nemesis::hkbStateChooser> m_startStateChooser;
        nemesis::hkRefPtr<nemesis::hkbCustomIdSelector> m_startStateIdSelector;
        int m_startStateId;
        nemesis::hkEventId m_returnToPreviousStateEventId;
        nemesis::hkEventId m_randomTransitionEventId;
        nemesis::hkEventId m_transitionToNextHigherStateEventId;
        nemesis::hkEventId m_transitionToNextLowerStateEventId;
        nemesis::hkVariableId m_syncVariableIndex;
        int m_currentStateId;
        bool m_wrapAroundStateId;
        char m_maxSimultaneousTransitions;
        nemesis::hkEnum<StartStateMode, char> m_startStateMode = {
            {"START_STATE_MODE_DEFAULT", 0},
            {"START_STATE_MODE_SYNC", 1},
            {"START_STATE_MODE_RANDOM", 2},
            {"START_STATE_MODE_CHOOSER", 3},
        };
        nemesis::hkEnum<StateMachineSelfTransitionMode, char> m_selfTransitionMode = {
            {"SELF_TRANSITION_MODE_NO_TRANSITION", 0},
            {"SELF_TRANSITION_MODE_TRANSITION_TO_START_STATE", 1},
            {"SELF_TRANSITION_MODE_FORCE_TRANSITION_TO_START_STATE", 2},
        };
        bool m_isActive;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::hkbStateMachineStateInfo>> m_states;
        nemesis::hkRefPtr<nemesis::hkbStateMachineTransitionInfoArray> m_wildcardTransitions;
        nemesis::hkRefPtr<nemesis::HavokObject> m_stateIdToIndexMap;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_activeTransitions;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_transitionFlags;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_wildcardTransitionFlags;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_delayedTransitions;
        float m_timeInState;
        float m_lastLocalTime;
        int m_previousStateId;
        int m_nextStartStateIndexOverride;
        bool m_stateOrTransitionChanged;
        bool m_echoNextUpdate;
        unsigned short m_sCurrentStateIndexAndEntered;

    public:
        hkbStateMachine() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbStateMachine::Class;
}
