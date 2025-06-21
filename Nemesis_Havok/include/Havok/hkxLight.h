#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkxLight : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x81c86d42,
                                                "hkxLight",
                                                &nemesis::hkReferencedObject::Class,
                                                80,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkxLight);

        using LightType = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkEnum<LightType, char> m_type = {
            {"POINT_LIGHT", 0},
            {"DIRECTIONAL_LIGHT", 1},
            {"SPOT_LIGHT", 2},
        };
        nemesis::hkVector4 m_position;
        nemesis::hkVector4 m_direction;
        unsigned int m_color;
        float m_angle;
        float m_range;
        float m_fadeStart;
        float m_fadeEnd;
        short m_decayRate;
        float m_intensity;
        bool m_shadowCaster;

    public:
        hkxLight() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkxLight::Class;
}
