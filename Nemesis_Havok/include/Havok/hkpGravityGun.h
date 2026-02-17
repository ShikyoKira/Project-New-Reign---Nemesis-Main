#pragma once

#include "Havok/Base/hkpFirstPersonGun.h"

namespace nemesis
{
    struct hkpGravityGun : nemesis::hkpFirstPersonGun
    {
        static constexpr nemesis::hkClass Class{0x5e2754cd,
                                                "hkpGravityGun",
                                                &nemesis::hkpFirstPersonGun::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpGravityGun);

        nemesis::hkArray<nemesis::hkRefPtr<nemesis::HavokObject>> m_grabbedBodies;
        int m_maxNumObjectsPicked{};
        float m_maxMassOfObjectPicked{};
        float m_maxDistOfObjectPicked{};
        float m_impulseAppliedWhenObjectNotPicked{};
        float m_throwVelocity{};
        nemesis::hkVector4 m_capturedObjectPosition;
        nemesis::hkVector4 m_capturedObjectsOffset;

    public:
        hkpGravityGun() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpGravityGun::Class;
}
