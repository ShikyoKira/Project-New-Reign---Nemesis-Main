#pragma once

#include "Havok/hkpCharacterControllerCinfo.h"

#include "Havok/Base/hkpShape.h"

namespace nemesis
{
    struct hkpCharacterRigidBodyCinfo : nemesis::hkpCharacterControllerCinfo
    {
        static constexpr nemesis::hkClass Class{0x892f441,
                                                "hkpCharacterRigidBodyCinfo",
                                                &nemesis::hkpCharacterControllerCinfo::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpCharacterRigidBodyCinfo);

        unsigned int m_collisionFilterInfo{};
        nemesis::hkRefPtr<nemesis::hkpShape> m_shape;
        nemesis::hkVector4 m_position;
        nemesis::hkQuaternion m_rotation;
        float m_mass{};
        float m_friction{};
        float m_maxLinearVelocity{};
        float m_allowedPenetrationDepth{};
        nemesis::hkVector4 m_up;
        float m_maxSlope{};
        float m_maxForce{};
        float m_unweldingHeightOffsetFactor{};
        float m_maxSpeedForSimplexSolver{};
        float m_supportDistance{};
        float m_hardSupportDistance{};
        int m_vdbColor{};

    public:
        hkpCharacterRigidBodyCinfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpCharacterRigidBodyCinfo::Class;
}
