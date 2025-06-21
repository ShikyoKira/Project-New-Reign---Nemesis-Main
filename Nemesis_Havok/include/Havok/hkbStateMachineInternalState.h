#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkbStateMachineActiveTransitionInfo.h"
#include "Havok/hkbStateMachineDelayedTransitionInfo.h"

namespace nemesis
{
    struct hkbStateMachineInternalState : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xbd1a7502,
                                                "hkbStateMachineInternalState",
                                                &nemesis::hkReferencedObject::Class,
                                                104,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbStateMachineInternalState);

        nemesis::hkArray<nemesis::hkbStateMachineActiveTransitionInfo> m_activeTransitions;
        nemesis::hkArray<unsigned char> m_transitionFlags;
        nemesis::hkArray<unsigned char> m_wildcardTransitionFlags;
        nemesis::hkArray<nemesis::hkbStateMachineDelayedTransitionInfo> m_delayedTransitions;
        float m_timeInState;
        float m_lastLocalTime;
        int m_currentStateId;
        int m_previousStateId;
        int m_nextStartStateIndexOverride;
        bool m_stateOrTransitionChanged;
        bool m_echoNextUpdate;

    public:
        hkbStateMachineInternalState() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbStateMachineInternalState::Class;
}
