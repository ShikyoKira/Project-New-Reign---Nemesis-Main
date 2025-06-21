#pragma once

#include "Havok/Base/hkpConstraintAtom.h"
#include "Havok/Base/hkpConstraintMotor.h"

namespace nemesis
{
    struct hkpAngMotorConstraintAtom : nemesis::hkpConstraintAtom
    {
        static constexpr nemesis::hkClass Class{0x81f087ff,
                                                "hkpAngMotorConstraintAtom",
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
        REGISTER_HAVOK_POINTER_HEADER(hkpAngMotorConstraintAtom);

        bool m_isEnabled;
        unsigned char m_motorAxis;
        short m_initializedOffset;
        short m_previousTargetAngleOffset;
        short m_correspondingAngLimitSolverResultOffset;
        float m_targetAngle;
        nemesis::hkRefPtr<nemesis::hkpConstraintMotor> m_motor;

    public:
        hkpAngMotorConstraintAtom() noexcept;

        const nemesis::hkClass* GetClass(nemesis::HavokVersion version) const override;

        void SerializeTo(nemesis::Serializer& serializer) const override;
        void DeserializeFrom(nemesis::Deserializer& deserializer) override;
    };

    constexpr nemesis::hkClass nemesis::hkpAngMotorConstraintAtom::Class;
}
