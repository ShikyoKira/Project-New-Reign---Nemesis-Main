#pragma once

#include "Havok/Base/hkpFirstPersonGun.h"

namespace nemesis
{
    struct hkpBallGun : nemesis::hkpFirstPersonGun
    {
        static constexpr nemesis::hkClass Class{0x57b06d35,
                                                "hkpBallGun",
                                                &nemesis::hkpFirstPersonGun::Class,
                                                112,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpBallGun);

        float m_bulletRadius;
        float m_bulletVelocity;
        float m_bulletMass;
        float m_damageMultiplier;
        int m_maxBulletsInWorld;
        nemesis::hkVector4 m_bulletOffsetFromCenter;
        nemesis::hkRefPtr<nemesis::HavokObject> m_addedBodies;

    public:
        hkpBallGun() noexcept;
        hkpBallGun(const nemesis::hkClass& cls) noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpBallGun::Class;
}
