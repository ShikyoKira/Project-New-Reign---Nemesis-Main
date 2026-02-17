#pragma once

#include "Havok/Base/hkbModifier.h"

namespace nemesis
{
    struct BSInterpValueModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0x29adc802,
                                                "BSInterpValueModifier",
                                                &nemesis::hkbModifier::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(BSInterpValueModifier);

        float m_source{};
        float m_target{};
        float m_result{};
        float m_gain{};
        float m_timeStep{};

    public:
        BSInterpValueModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::BSInterpValueModifier::Class;
}
