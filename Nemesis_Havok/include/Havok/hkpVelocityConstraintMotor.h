#pragma once

#include "Havok/hkpLimitedForceConstraintMotor.h"

namespace nemesis
{
    struct hkpVelocityConstraintMotor : nemesis::hkpLimitedForceConstraintMotor
    {
        static constexpr nemesis::hkClass Class{0xfca2fcc3,
                                                "hkpVelocityConstraintMotor",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpVelocityConstraintMotor);

        float m_tau{};
        float m_velocityTarget{};
        bool m_useVelocityTargetFromConstraintTargets{};

    public:
        hkpVelocityConstraintMotor() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpVelocityConstraintMotor::Class;
}
