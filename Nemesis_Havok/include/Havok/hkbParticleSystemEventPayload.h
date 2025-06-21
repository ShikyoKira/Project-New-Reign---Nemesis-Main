#pragma once

#include "Havok/hkbEventPayload.h"

namespace nemesis
{
    struct hkbParticleSystemEventPayload : nemesis::hkbEventPayload
    {
        static constexpr nemesis::hkClass Class{0x9df46cd6,
                                                "hkbParticleSystemEventPayload",
                                                &nemesis::hkbEventPayload::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkbParticleSystemEventPayload);

        using SystemType = nemesis::hkInternalClassEnumItem<unsigned char>;

        nemesis::hkEnum<SystemType, unsigned char> m_type = {
            {"DEBRIS", 0},
            {"DUST", 1},
            {"EXPLOSION", 2},
            {"SMOKE", 3},
            {"SPARKS", 4},
        };
        short m_emitBoneIndex;
        nemesis::hkVector4 m_offset;
        nemesis::hkVector4 m_direction;
        int m_numParticles;
        float m_speed;

    public:
        hkbParticleSystemEventPayload() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbParticleSystemEventPayload::Class;
}
