#pragma once

#include "Havok/Base/hkpConstraintAtom.h"
#include "Havok/Base/hkpConstraintMotor.h"

namespace nemesis
{
    struct hkpLinMotorConstraintAtom : nemesis::hkpConstraintAtom
    {
        static constexpr nemesis::hkClass Class{0x10312464,
                                                "hkpLinMotorConstraintAtom",
                                                &nemesis::hkpConstraintAtom::Class,
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
        REGISTER_HAVOK_POINTER_HEADER(hkpLinMotorConstraintAtom);

        bool m_isEnabled{};
        unsigned char m_motorAxis{};
        short m_initializedOffset{};
        short m_previousTargetPositionOffset{};
        float m_targetPosition{};
        nemesis::hkRefPtr<nemesis::hkpConstraintMotor> m_motor;

    public:
        hkpLinMotorConstraintAtom() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpLinMotorConstraintAtom::Class;
}
