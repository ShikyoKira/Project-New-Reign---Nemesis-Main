#pragma once

#include "Havok/Base/hkpConstraintMotor.h"

namespace nemesis
{
    struct hkpLimitedForceConstraintMotor : nemesis::hkpConstraintMotor
    {
        static constexpr nemesis::hkClass Class{0x3377b0b0,
                                                "hkpLimitedForceConstraintMotor",
                                                &nemesis::hkpConstraintMotor::Class,
                                                32,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpLimitedForceConstraintMotor);

        float m_minForce{};
        float m_maxForce{};

    public:
        hkpLimitedForceConstraintMotor() noexcept;
        hkpLimitedForceConstraintMotor(const nemesis::hkClass& cls) noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpLimitedForceConstraintMotor::Class;
}
