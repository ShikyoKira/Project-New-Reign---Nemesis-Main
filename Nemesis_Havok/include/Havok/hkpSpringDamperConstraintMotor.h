#pragma once

#include "Havok/hkpLimitedForceConstraintMotor.h"

namespace nemesis
{
    struct hkpSpringDamperConstraintMotor : nemesis::hkpLimitedForceConstraintMotor
    {
        static constexpr nemesis::hkClass Class{0x7ead26f6,
                                                "hkpSpringDamperConstraintMotor",
                                                &nemesis::hkpLimitedForceConstraintMotor::Class,
                                                40,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpSpringDamperConstraintMotor);

        float m_springConstant;
        float m_springDamping;

    public:
        hkpSpringDamperConstraintMotor() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpSpringDamperConstraintMotor::Class;
}
