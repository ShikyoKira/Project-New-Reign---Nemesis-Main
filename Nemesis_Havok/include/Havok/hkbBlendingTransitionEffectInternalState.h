#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkbBlendingTransitionEffectInternalState : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0xb18c70c2,
                                                "hkbBlendingTransitionEffectInternalState",
                                                &nemesis::hkReferencedObject::Class,
                                                48,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbBlendingTransitionEffectInternalState);

        using SelfTransitionMode = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkVector4 m_fromPos;
        nemesis::hkQuaternion m_fromRot;
        nemesis::hkVector4 m_toPos;
        nemesis::hkQuaternion m_toRot;
        nemesis::hkVector4 m_lastPos;
        nemesis::hkQuaternion m_lastRot;
        nemesis::hkArray<nemesis::hkQsTransform> m_characterPoseAtBeginningOfTransition;
        float m_timeRemaining{};
        float m_timeInTransition{};
        bool m_resetToGenerator{};
        nemesis::hkEnum<SelfTransitionMode, char> m_toGeneratorSelfTranstitionMode = {
            {"SELF_TRANSITION_MODE_CONTINUE_IF_CYCLIC_BLEND_IF_ACYCLIC", 0},
            {"SELF_TRANSITION_MODE_CONTINUE", 1},
            {"SELF_TRANSITION_MODE_RESET", 2},
            {"SELF_TRANSITION_MODE_BLEND", 3},
        };
        bool m_applySelfTransition{};
        bool m_initializeCharacterPose{};
        bool m_alignThisFrame{};
        bool m_alignmentFinished{};

    public:
        hkbBlendingTransitionEffectInternalState() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbBlendingTransitionEffectInternalState::Class;
}
