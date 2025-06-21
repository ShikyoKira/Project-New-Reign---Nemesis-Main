#pragma once

#include "Havok/Base/hkReferencedObject.h"

namespace nemesis
{
    struct hkpConstraintMotor : nemesis::hkReferencedObject
    {
        static constexpr nemesis::hkClass Class{0x6a44c317,
                                                "hkpConstraintMotor",
                                                &nemesis::hkReferencedObject::Class,
                                                24,
                                                nullptr,
                                                0,
                                                {},
                                                {},
                                                nullptr,
                                                nullptr,
                                                nemesis::hkClass::FlagValues::FLAGS_NONE,
                                                0};

    private:
        using MotorType = nemesis::hkInternalClassEnumItem<char>;

        nemesis::hkEnum<MotorType, char> m_type = {
            {"TYPE_INVALID", 0},
            {"TYPE_POSITION", 1},
            {"TYPE_VELOCITY", 2},
            {"TYPE_SPRING_DAMPER", 3},
            {"TYPE_CALLBACK", 4},
            {"TYPE_MAX", 5},
        };

    public:
        hkpConstraintMotor(const nemesis::hkClass& cls) noexcept;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpConstraintMotor::Class;
}
