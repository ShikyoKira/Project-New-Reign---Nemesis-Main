#pragma once

#include "Havok/Base/hkbGenerator.h"

#include "Havok/hkbEventProperty.h"

namespace nemesis
{
    struct BSCyclicBlendTransitionGenerator : nemesis::hkbGenerator
    {
        static constexpr nemesis::hkClass Class{0x5119eb06,
                                                "BSCyclicBlendTransitionGenerator",
                                                &nemesis::hkbGenerator::Class,
                                                176,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(BSCyclicBlendTransitionGenerator);

        using BlendCurve = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkRefPtr<nemesis::hkbGenerator> m_pBlenderGenerator; // align 16
        nemesis::hkbEventProperty m_EventToFreezeBlendValue;
        nemesis::hkbEventProperty m_EventToCrossBlend;
        float m_fBlendParameter{};
        float m_fTransitionDuration{};
        nemesis::hkEnum<BlendCurve, char> m_eBlendCurve = {
            {"BLEND_CURVE_SMOOTH", 0},
            {"BLEND_CURVE_LINEAR", 1},
            {"BLEND_CURVE_LINEAR_TO_SMOOTH", 2},
            {"BLEND_CURVE_SMOOTH_TO_LINEAR", 3},
        };
        nemesis::hkRefPtr<nemesis::HavokObject> m_pTransitionBlenderGenerator; // align 16
        nemesis::hkRefPtr<nemesis::HavokObject> m_pTransitionEffect;           // align 16
        char m_currentMode = '\0';

    public:
        BSCyclicBlendTransitionGenerator() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::BSCyclicBlendTransitionGenerator::Class;
}
