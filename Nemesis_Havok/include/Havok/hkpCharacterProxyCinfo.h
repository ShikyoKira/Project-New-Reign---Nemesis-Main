#pragma once

#include "Havok/hkpCharacterControllerCinfo.h"

#include "Havok/Base/hkpShapePhantom.h"

namespace nemesis
{
    struct hkpCharacterProxyCinfo : nemesis::hkpCharacterControllerCinfo
    {
        static constexpr nemesis::hkClass Class{0x586d97b2,
                                                "hkpCharacterProxyCinfo",
                                                &nemesis::hkpCharacterControllerCinfo::Class,
                                                144,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpCharacterProxyCinfo);

        nemesis::hkVector4 m_position;
        nemesis::hkVector4 m_velocity;
        float m_dynamicFriction;
        float m_staticFriction;
        float m_keepContactTolerance;
        nemesis::hkVector4 m_up;
        float m_extraUpStaticFriction;
        float m_extraDownStaticFriction;
        nemesis::hkRefPtr<nemesis::hkpShapePhantom> m_shapePhantom;
        float m_keepDistance;
        float m_contactAngleSensitivity;
        unsigned int m_userPlanes;
        float m_maxCharacterSpeedForSolver;
        float m_characterStrength;
        float m_characterMass;
        float m_maxSlope;
        float m_penetrationRecoverySpeed;
        int m_maxCastIterations;
        bool m_refreshManifoldInCheckSupport;

    public:
        hkpCharacterProxyCinfo() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpCharacterProxyCinfo::Class;
}
