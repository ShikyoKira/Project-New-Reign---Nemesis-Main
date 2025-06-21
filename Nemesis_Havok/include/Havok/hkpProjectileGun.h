#pragma once

#include "Havok/Base/hkpFirstPersonGun.h"

namespace nemesis
{
    struct hkpProjectileGun : nemesis::hkpFirstPersonGun
    {
        static constexpr nemesis::hkClass Class{0xb4f30148,
                                                "hkpProjectileGun",
                                                &nemesis::hkpFirstPersonGun::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpProjectileGun);

        int m_maxProjectiles;
        float m_reloadTime;
        float m_reload;
        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_projectiles;
        nemesis::hkRefPtr<nemesis::HavokObject> m_world;
        nemesis::hkRefPtr<nemesis::HavokObject> m_destructionWorld;

    public:
        hkpProjectileGun() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpProjectileGun::Class;
}
