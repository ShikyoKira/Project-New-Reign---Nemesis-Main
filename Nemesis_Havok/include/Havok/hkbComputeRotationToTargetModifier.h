#pragma once

#include "Havok/Base/hkbModifier.h"

namespace nemesis
{
    struct hkbComputeRotationToTargetModifier : nemesis::hkbModifier
    {
        static constexpr nemesis::hkClass Class{0x47665f1c,
                                                "hkbComputeRotationToTargetModifier",
                                                &nemesis::hkbModifier::Class,
                                                192,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkbComputeRotationToTargetModifier);

        nemesis::hkQuaternion m_rotationOut;
        nemesis::hkVector4 m_targetPosition;
        nemesis::hkVector4 m_currentPosition;
        nemesis::hkQuaternion m_currentRotation;
        nemesis::hkVector4 m_localAxisOfRotation;
        nemesis::hkVector4 m_localFacingDirection;
        bool m_resultIsDelta;

    public:
        hkbComputeRotationToTargetModifier() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkbComputeRotationToTargetModifier::Class;
}
