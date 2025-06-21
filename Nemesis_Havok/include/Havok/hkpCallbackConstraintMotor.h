#pragma once

#include "Havok/hkpLimitedForceConstraintMotor.h"

namespace nemesis
{
    struct hkpCallbackConstraintMotor : nemesis::hkpLimitedForceConstraintMotor
    {
        static constexpr nemesis::hkClass Class{0xafcd79ad,
                                                "hkpCallbackConstraintMotor",
                                                &nemesis::hkpLimitedForceConstraintMotor::Class,
                                                72,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        REGISTER_HAVOK_POINTER_HEADER(hkpCallbackConstraintMotor);

        using CallbackType = nemesis::hkInternalClassEnumItem<unsigned int>;

        nemesis::hkRefPtr<nemesis::HavokObject> m_callbackFunc;
        nemesis::hkEnum<CallbackType, unsigned int> m_callbackType = {
            {"CALLBACK_MOTOR_TYPE_HAVOK_DEMO_SPRING_DAMPER", 0},
            {"CALLBACK_MOTOR_TYPE_USER_0", 1},
            {"CALLBACK_MOTOR_TYPE_USER_1", 2},
            {"CALLBACK_MOTOR_TYPE_USER_2", 3},
            {"CALLBACK_MOTOR_TYPE_USER_3", 4},
        };
        nemesis::hkUlong m_userData0;
        nemesis::hkUlong m_userData1;
        nemesis::hkUlong m_userData2;

    public:
        hkpCallbackConstraintMotor() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpCallbackConstraintMotor::Class;
}
