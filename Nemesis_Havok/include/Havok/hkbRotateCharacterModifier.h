#pragma once

#include "Havok/Base/hkbModifier.h"

namespace nemesis
{
    struct hkbRotateCharacterModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0x877ebc0b,
                                                "hkbRotateCharacterModifier",
                                                &nemesis::hkbModifier::Class,
                                                128,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbRotateCharacterModifier);

        float m_degreesPerSecond{};
        float m_speedMultiplier{};
        nemesis::hkVector4 m_axisOfRotation;
        float m_angle{};

    public:
        hkbRotateCharacterModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbRotateCharacterModifier::Class;
}
