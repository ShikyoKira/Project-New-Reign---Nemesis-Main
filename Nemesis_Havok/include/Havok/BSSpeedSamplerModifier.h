#pragma once

#include "Havok/Base/hkbModifier.h"

namespace nemesis
{
    struct BSSpeedSamplerModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0xd297fda9,
                                                "BSSpeedSamplerModifier",
                                                &nemesis::hkbModifier::Class,
                                                96,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(BSSpeedSamplerModifier);

        int m_state{};
        float m_direction{};
        float m_goalSpeed{};
        float m_speedOut{};

    public:
        BSSpeedSamplerModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::BSSpeedSamplerModifier::Class;
}
