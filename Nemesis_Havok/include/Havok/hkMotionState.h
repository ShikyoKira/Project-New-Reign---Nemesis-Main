#pragma once

#include "Havok/hkSweptTransform.h"

namespace nemesis
{
    struct hkMotionState : nemesis::HavokObject
    {
        static constexpr nemesis::hkClass Class{0x5797386e,
                                                "hkMotionState",
                                                nullptr,
                                                176,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkMotionState);

        nemesis::hkTransform m_transform;
        nemesis::hkSweptTransform m_sweptTransform;
        nemesis::hkVector4 m_deltaAngle;
        float m_objectRadius;
        nemesis::hkHalf m_linearDamping;
        nemesis::hkHalf m_angularDamping;
        nemesis::hkHalf m_timeFactor;
        nemesis::hkUFloat8 m_maxLinearVelocity;
        nemesis::hkUFloat8 m_maxAngularVelocity;
        unsigned char m_deactivationClass;

    public:
        hkMotionState() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkMotionState::Class;
}
