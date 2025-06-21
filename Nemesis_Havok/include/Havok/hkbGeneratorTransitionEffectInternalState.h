#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkbGeneratorTransitionEffectInternalState : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xd6692b5d,
                                                "hkbGeneratorTransitionEffectInternalState",
                                                &nemesis::hkReferencedObject::Class,
                                                40,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbGeneratorTransitionEffectInternalState);

        using ToGeneratorState = nemesis::hkInternalClassEnumItem<char>;
        using Stage = nemesis::hkInternalClassEnumItem<char>;
        using SelfTransitionMode = nemesis::hkInternalClassEnumItem<char>;

        float m_timeInTransition;
        float m_duration;
        float m_effectiveBlendInDuration;
        float m_effectiveBlendOutDuration;
        nemesis::hkEnum<ToGeneratorState, char> m_toGeneratorState = {
            {"STATE_INACTIVE", 0},
            {"STATE_READY_FOR_SET_LOCAL_TIME", 1},
            {"STATE_READY_FOR_APPLY_SELF_TRANSITION_MODE", 2},
            {"STATE_ACTIVE", 3},
        };
        bool m_echoTransitionGenerator;
        bool m_echoToGenerator;
        nemesis::hkEnum<SelfTransitionMode, char> m_toGeneratorSelfTransitionMode = {
            {"SELF_TRANSITION_MODE_CONTINUE_IF_CYCLIC_BLEND_IF_ACYCLIC", 0},
            {"SELF_TRANSITION_MODE_CONTINUE", 1},
            {"SELF_TRANSITION_MODE_RESET", 2},
            {"SELF_TRANSITION_MODE_BLEND", 3},
        };
        bool m_justActivated;
        bool m_updateActiveNodes;
        nemesis::hkEnum<Stage, char> m_stage = {
            {"STAGE_BLENDING_IN", 0},
            {"STAGE_PLAYING_TRANSITION_GENERATOR", 1},
            {"STAGE_BLENDING_OUT", 2},
        };

    public:
        hkbGeneratorTransitionEffectInternalState() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbGeneratorTransitionEffectInternalState::Class;
}
