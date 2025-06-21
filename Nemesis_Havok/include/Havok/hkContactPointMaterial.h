#pragma once

#include "Havok/hkClass.h"

#include "Serialize/Deserializer.h"

namespace nemesis
{
    struct hkContactPointMaterial : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x4e32287c,
                                                "hkContactPointMaterial",
                                                nullptr,
                                                16,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkContactPointMaterial);

        nemesis::hkUlong m_userData;
        nemesis::hkUFloat8 m_friction;
        unsigned char m_restitution;
        nemesis::hkUFloat8 m_maxImpulse;
        unsigned char m_flags;

    public:
        hkContactPointMaterial() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkContactPointMaterial::Class;
}
