#pragma once

#include "Havok/hkbTransitionEffect.h"

namespace nemesis
{
    struct hkbBlendingTransitionEffect : nemesis::hkbTransitionEffect
    {
        static constexpr nemesis::hkClass Class{0xfd8584fe,
                                                "hkbBlendingTransitionEffect",
                                                &nemesis::hkbTransitionEffect::Class,
                                                144,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbBlendingTransitionEffect);

        using FlagBits = nemesis::hkInternalClassEnumItem<unsigned short, true>;
        using EndMode = nemesis::hkInternalClassEnumItem<char>;
        using BlendCurve = nemesis::hkInternalClassEnumItem<char>;

        float m_duration{};
        float m_toGeneratorStartTimeFraction{};
        nemesis::hkEnum<FlagBits, unsigned short> m_flags = {
            //{"FLAG_NONE", 0x0},
            {"0", 0x0},
            {"FLAG_IGNORE_FROM_WORLD_FROM_MODEL", 0x1},
            {"FLAG_SYNC", 0x2},
            {"FLAG_IGNORE_TO_WORLD_FROM_MODEL", 0x4},
            {"FLAG_IGNORE_TO_WORLD_FROM_MODEL_ROTATION", 0x8},
        };
        nemesis::hkEnum<EndMode, char> m_endMode = {
            {"END_MODE_NONE", 0},
            {"END_MODE_TRANSITION_UNTIL_END_OF_FROM_GENERATOR", 1},
            {"END_MODE_CAP_DURATION_AT_END_OF_FROM_GENERATOR", 2},
        };
        nemesis::hkEnum<BlendCurve, char> m_blendCurve = {
            {"BLEND_CURVE_SMOOTH", 0},
            {"BLEND_CURVE_LINEAR", 1},
            {"BLEND_CURVE_LINEAR_TO_SMOOTH", 2},
            {"BLEND_CURVE_SMOOTH_TO_LINEAR", 3},
        };
        nemesis::hkRefPtr<nemesis::HavokObject> m_fromGenerator;
        nemesis::hkRefPtr<nemesis::HavokObject> m_toGenerator;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_characterPoseAtBeginningOfTransition;
        float m_timeRemaining{};
        float m_timeInTransition{};
        short m_alignmentBone{};
        bool m_applySelfTransition{};
        bool m_initializeCharacterPose{};

    public:
        hkbBlendingTransitionEffect() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbBlendingTransitionEffect::Class;
}
