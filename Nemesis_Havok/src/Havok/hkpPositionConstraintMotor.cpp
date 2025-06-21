#include "Havok/hkpPositionConstraintMotor.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpPositionConstraintMotor,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpPositionConstraintMotor::Class; });

nemesis::hkpPositionConstraintMotor::hkpPositionConstraintMotor() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpLimitedForceConstraintMotor(Class)
{
}

const nemesis::hkClass* nemesis::hkpPositionConstraintMotor::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpPositionConstraintMotor::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpLimitedForceConstraintMotor::SerializeTo(serializer); // offset: 0/0 size: 20/32 align: 4/8
    serializer.WriteValue("tau", m_tau);                              // offset: 20/32 size: 4/4 align: 4/4
    serializer.WriteValue("damping", m_damping);                      // offset: 24/36 size: 4/4 align: 4/4
    serializer.WriteValue("proportionalRecoveryVelocity",
                          m_proportionalRecoveryVelocity); // offset: 28/40 size: 4/4 align: 4/4
    serializer.WriteValue("constantRecoveryVelocity",
                          m_constantRecoveryVelocity); // offset: 32/44 size: 4/4 align: 4/4
    // class size: 36/48 align: 4/8
}

void nemesis::hkpPositionConstraintMotor::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpLimitedForceConstraintMotor::DeserializeFrom(
        deserializer);                            // offset: 0/0 size: 20/32 align: 4/8
    deserializer.ReadValue("tau", m_tau);         // offset: 20/32 size: 4/4 align: 4/4
    deserializer.ReadValue("damping", m_damping); // offset: 24/36 size: 4/4 align: 4/4
    deserializer.ReadValue("proportionalRecoveryVelocity",
                           m_proportionalRecoveryVelocity); // offset: 28/40 size: 4/4 align: 4/4
    deserializer.ReadValue("constantRecoveryVelocity",
                           m_constantRecoveryVelocity); // offset: 32/44 size: 4/4 align: 4/4
    // class size: 36/48 align: 4/8
}
