#pragma once

#include "Havok/Base/hkReferencedObject.h"

#include "Havok/hkMotionState.h"

namespace nemesis
{
    struct hkpMaxSizeMotion;

    struct hkpMotion : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x98aadb4f,
                                                "hkpMotion",
                                                &nemesis::hkReferencedObject::Class,
                                                320,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        using MotionType = nemesis::hkInternalClassEnumItem<unsigned char>;

        nemesis::hkEnum<MotionType, unsigned char> m_type = {
            {"MOTION_INVALID", 0},
            {"MOTION_DYNAMIC", 1},
            {"MOTION_SPHERE_INERTIA", 2},
            {"MOTION_BOX_INERTIA", 3},
            {"MOTION_KEYFRAMED", 4},
            {"MOTION_FIXED", 5},
            {"MOTION_THIN_BOX_INERTIA", 6},
            {"MOTION_CHARACTER", 7},
            {"MOTION_MAX_ID", 8},
        };
        unsigned char m_deactivationIntegrateCounter;
        std::array<unsigned short, 2> m_deactivationNumInactiveFrames;
        nemesis::hkMotionState m_motionState;
        nemesis::hkVector4 m_inertiaAndMassInv;
        nemesis::hkVector4 m_linearVelocity;
        nemesis::hkVector4 m_angularVelocity;
        std::array<nemesis::hkVector4, 2> m_deactivationRefPosition;
        std::array<unsigned int, 2> m_deactivationRefOrientation;
        nemesis::hkpMaxSizeMotion* m_savedMotion = nullptr;
        unsigned short m_savedQualityTypeIndex;
        nemesis::hkHalf m_gravityFactor;

    public:
        hkpMotion(const nemesis::hkClass& cls) noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpMotion::Class;
}
