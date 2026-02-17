#pragma once

#include "Havok/Base/hkbGenerator.h"

#include "Havok/hkbTransitionEffect.h"

namespace nemesis
{
    struct hkbGeneratorTransitionEffect : nemesis::hkbTransitionEffect
    {
        static constexpr nemesis::hkClass Class{0x5f771b12,
                                                "hkbGeneratorTransitionEffect",
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
        REGISTER_HAVOK_POINTER_HEADER(hkbGeneratorTransitionEffect);

        nemesis::hkRefPtr<nemesis::hkbGenerator> m_transitionGenerator;
        float m_blendInDuration{};
        float m_blendOutDuration{};
        bool m_syncToGeneratorStartTime{};
        nemesis::hkRefPtr<nemesis::HavokObject> m_fromGenerator;
        nemesis::hkRefPtr<nemesis::HavokObject> m_toGenerator;
        float m_timeInTransition{};
        float m_duration{};
        float m_effectiveBlendInDuration{};
        float m_effectiveBlendOutDuration{};
        char m_toGeneratorState{};
        bool m_echoTransitionGenerator{};
        bool m_echoToGenerator{};
        bool m_justActivated{};
        bool m_updateActiveNodes{};
        char m_stage{};

    public:
        hkbGeneratorTransitionEffect() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbGeneratorTransitionEffect::Class;
}
