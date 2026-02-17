#pragma once

#include "Havok/Base/hkbModifier.h"

namespace nemesis
{
    struct hkbDampingModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0x9a040f03,
                                                "hkbDampingModifier",
                                                &nemesis::hkbModifier::Class,
                                                192,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbDampingModifier);

        float m_kP{};
        float m_kI{};
        float m_kD{};
        bool m_enableScalarDamping{};
        bool m_enableVectorDamping{};
        float m_rawValue{};
        float m_dampedValue{};
        nemesis::hkVector4 m_rawVector;
        nemesis::hkVector4 m_dampedVector;
        nemesis::hkVector4 m_vecErrorSum;
        nemesis::hkVector4 m_vecPreviousError;
        float m_errorSum{};
        float m_previousError{};

    public:
        hkbDampingModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbDampingModifier::Class;
}
