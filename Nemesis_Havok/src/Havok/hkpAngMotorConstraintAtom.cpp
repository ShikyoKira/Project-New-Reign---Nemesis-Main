#include "Havok/hkpAngMotorConstraintAtom.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpAngMotorConstraintAtom,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpAngMotorConstraintAtom::Class; });

nemesis::hkpAngMotorConstraintAtom::hkpAngMotorConstraintAtom() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpConstraintAtom(Class)
{
}

const nemesis::hkClass* nemesis::hkpAngMotorConstraintAtom::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpAngMotorConstraintAtom::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintAtom::SerializeTo(serializer); // offset: 0/0 size: 2/2 align: 2/2
    serializer.WriteValue("isEnabled", m_isEnabled);     // offset: 2/2 size: 1/1 align: 1/1
    serializer.WriteValue("motorAxis", m_motorAxis);     // offset: 3/3 size: 1/1 align: 1/1

    if (serializer.GetContentsVersion() > HavokVersion::HK_2011_3_0)
    {
        serializer.WriteSerializeIgnoredValue("initializedOffset", m_initializedOffset);
        serializer.WriteSerializeIgnoredValue("previousTargetAngleOffset", m_previousTargetAngleOffset);
        serializer.WriteSerializeIgnoredValue("correspondingAngLimitSolverResultOffset",
                                              m_correspondingAngLimitSolverResultOffset);
    }
    else
    {
        serializer.WriteValue("initializedOffset", m_initializedOffset); // offset: 4/4 size: 2/2 align: 2/2
        serializer.WriteValue("previousTargetAngleOffset",
                              m_previousTargetAngleOffset); // offset: 6/6 size: 2/2 align: 2/2
        serializer.WriteValue("correspondingAngLimitSolverResultOffset",
                              m_correspondingAngLimitSolverResultOffset); // offset: 8/8 size: 2/2 align: 2/2
    }

    serializer.Skip(2); // offset: 10/10 size: 2/2

    serializer.WriteValue("targetAngle", m_targetAngle); // offset: 12/12 size: 4/4 align: 4/4
    serializer.WriteObject("motor", m_motor);            // offset: 16/16 size: 4/8 align: 4/8
    // class size: 20/24 align: 4/8
}

void nemesis::hkpAngMotorConstraintAtom::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintAtom::DeserializeFrom(deserializer);        // offset: 0/0 size: 2/2 align: 2/2
    deserializer.ReadValue("isEnabled", m_isEnabled);                 // offset: 2/2 size: 1/1 align: 1/1
    deserializer.ReadValue("motorAxis", m_motorAxis);                 // offset: 3/3 size: 1/1 align: 1/1
    deserializer.ReadValue("initializedOffset", m_initializedOffset); // offset: 4/4 size: 2/2 align: 2/2
    deserializer.ReadValue("previousTargetAngleOffset",
                           m_previousTargetAngleOffset); // offset: 6/6 size: 2/2 align: 2/2
    deserializer.ReadValue("correspondingAngLimitSolverResultOffset",
                           m_correspondingAngLimitSolverResultOffset); // offset: 8/8 size: 2/2 align: 2/2
    deserializer.Skip(2);                                              // offset: 10/10 size: 2/2

    deserializer.ReadValue("targetAngle", m_targetAngle); // offset: 12/12 size: 4/4 align: 4/4
    deserializer.ReadObject("motor", m_motor);            // offset: 16/16 size: 4/8 align: 4/8
    // class size: 20/24 align: 4/8
}
