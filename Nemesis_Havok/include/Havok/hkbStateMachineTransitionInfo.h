#pragma once

#include "Havok/Base/hkbCondition.h"

#include "Havok/hkbStateMachineTimeInterval.h"
#include "Havok/hkbTransitionEffect.h"

namespace nemesis
{
    struct hkbStateMachineTransitionInfo : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0xcdec8025,
                                                "hkbStateMachineTransitionInfo",
                                                nullptr,
                                                72,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbStateMachineTransitionInfo);

        using TransitionFlags = nemesis::hkInternalClassEnumItem<short, true>;

        nemesis::hkbStateMachineTimeInterval m_triggerInterval;
        nemesis::hkbStateMachineTimeInterval m_initiateInterval;
        nemesis::hkRefPtr<nemesis::hkbTransitionEffect> m_transition;
        nemesis::hkRefPtr<nemesis::hkbCondition> m_condition;
        nemesis::hkEventId m_eventId;
        int m_toStateId{};
        int m_fromNestedStateId{};
        int m_toNestedStateId{};
        short m_priority{};
        nemesis::hkEnum<TransitionFlags, short> m_flags = {
            {"FLAG_USE_TRIGGER_INTERVAL", 0x1},
            {"FLAG_USE_INITIATE_INTERVAL", 0x2},
            {"FLAG_UNINTERRUPTIBLE_WHILE_PLAYING", 0x4},
            {"FLAG_UNINTERRUPTIBLE_WHILE_DELAYED", 0x8},
            {"FLAG_DELAY_STATE_CHANGE", 0x10},
            {"FLAG_DISABLED", 0x20},
            {"FLAG_DISALLOW_RETURN_TO_PREVIOUS_STATE", 0x40},
            {"FLAG_DISALLOW_RANDOM_TRANSITION", 0x80},
            {"FLAG_DISABLE_CONDITION", 0x100},
            {"FLAG_ALLOW_SELF_TRANSITION_BY_TRANSITION_FROM_ANY_STATE", 0x200},
            {"FLAG_IS_GLOBAL_WILDCARD", 0x400},
            {"FLAG_IS_LOCAL_WILDCARD", 0x800},
            {"FLAG_FROM_NESTED_STATE_ID_IS_VALID", 0x1000},
            {"FLAG_TO_NESTED_STATE_ID_IS_VALID", 0x2000},
            {"FLAG_ABUT_AT_END_OF_FROM_GENERATOR", 0x4000},
        };

    public:
        hkbStateMachineTransitionInfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbStateMachineTransitionInfo::Class;
}
