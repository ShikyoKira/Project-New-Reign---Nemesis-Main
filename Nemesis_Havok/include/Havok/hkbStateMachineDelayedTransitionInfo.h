#pragma once

#include "Havok/hkbStateMachineProspectiveTransitionInfo.h"

namespace nemesis
{
    struct hkbStateMachineDelayedTransitionInfo : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x26d5499,
                                                "hkbStateMachineDelayedTransitionInfo",
                                                nullptr,
                                                24,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbStateMachineDelayedTransitionInfo);

        nemesis::hkbStateMachineProspectiveTransitionInfo m_delayedTransition;
        float m_timeDelayed;
        bool m_isDelayedTransitionReturnToPreviousState;
        bool m_wasInAbutRangeLastFrame;

    public:
        hkbStateMachineDelayedTransitionInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbStateMachineDelayedTransitionInfo::Class;
}
