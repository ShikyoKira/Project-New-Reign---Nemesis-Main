#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkpMaterial : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x33be6570,
                                                "hkpMaterial",
                                                nullptr,
                                                12,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpMaterial);

        using ResponseType = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkEnum<ResponseType, char> m_responseType = {
            {"RESPONSE_INVALID", 0},
            {"RESPONSE_SIMPLE_CONTACT", 1},
            {"RESPONSE_REPORTING", 2},
            {"RESPONSE_NONE", 3},
            {"RESPONSE_MAX_ID", 4},
        };
        nemesis::hkHalf m_rollingFrictionMultiplier;
        float m_friction{};
        float m_restitution{};

    public:
        hkpMaterial() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpMaterial::Class;
}
