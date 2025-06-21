#pragma once

#include "Havok/hkpLimitedForceConstraintMotor.h"

namespace nemesis
{
    struct hkpPositionConstraintMotor : nemesis::hkpLimitedForceConstraintMotor
    {
        static constexpr nemesis::hkClass Class{0x748fb303,
                                                "hkpPositionConstraintMotor",
                                                &nemesis::hkpLimitedForceConstraintMotor::Class,
                                                48,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpPositionConstraintMotor);

        float m_tau;
        float m_damping;
        float m_proportionalRecoveryVelocity;
        float m_constantRecoveryVelocity;

    public:
        hkpPositionConstraintMotor() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpPositionConstraintMotor::Class;
}
