#include "Havok/hkpVelocityConstraintMotor.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpVelocityConstraintMotor,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpVelocityConstraintMotor::Class; });

nemesis::hkpVelocityConstraintMotor::hkpVelocityConstraintMotor() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpLimitedForceConstraintMotor(Class)
{
}

const nemesis::hkClass* nemesis::hkpVelocityConstraintMotor::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpVelocityConstraintMotor::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpLimitedForceConstraintMotor::SerializeTo(serializer); // offset: 0/0 size: 20/32 align: 4/8
    serializer.WriteValue("tau", m_tau);                              // offset: 20/32 size: 4/4 align: 4/4
    serializer.WriteValue("velocityTarget", m_velocityTarget);        // offset: 24/36 size: 4/4 align: 4/4
    serializer.WriteValue("useVelocityTargetFromConstraintTargets",
                          m_useVelocityTargetFromConstraintTargets); // offset: 28/40 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());                     // offset: 29/41 size: 3/7
    // class size: 32/48 align: 4/8
}

void nemesis::hkpVelocityConstraintMotor::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpLimitedForceConstraintMotor::DeserializeFrom(
        deserializer);                                          // offset: 0/0 size: 20/32 align: 4/8
    deserializer.ReadValue("tau", m_tau);                       // offset: 20/32 size: 4/4 align: 4/4
    deserializer.ReadValue("velocityTarget", m_velocityTarget); // offset: 24/36 size: 4/4 align: 4/4
    deserializer.ReadValue("useVelocityTargetFromConstraintTargets",
                           m_useVelocityTargetFromConstraintTargets); // offset: 28/40 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());                  // offset: 29/41 size: 3/7
    // class size: 32/48 align: 4/8
}
