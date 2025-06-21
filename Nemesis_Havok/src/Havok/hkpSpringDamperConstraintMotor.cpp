#include "Havok/hkpSpringDamperConstraintMotor.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpSpringDamperConstraintMotor,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpSpringDamperConstraintMotor::Class; });

nemesis::hkpSpringDamperConstraintMotor::hkpSpringDamperConstraintMotor() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpLimitedForceConstraintMotor(Class)
{
}

const nemesis::hkClass* nemesis::hkpSpringDamperConstraintMotor::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpSpringDamperConstraintMotor::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpLimitedForceConstraintMotor::SerializeTo(serializer); // offset: 0/0 size: 20/32 align: 4/8
    serializer.WriteValue("springConstant", m_springConstant);        // offset: 20/32 size: 4/4 align: 4/4
    serializer.WriteValue("springDamping", m_springDamping);          // offset: 24/36 size: 4/4 align: 4/4
    // class size: 28/40 align: 4/8
}

void nemesis::hkpSpringDamperConstraintMotor::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpLimitedForceConstraintMotor::DeserializeFrom(
        deserializer);                                          // offset: 0/0 size: 20/32 align: 4/8
    deserializer.ReadValue("springConstant", m_springConstant); // offset: 20/32 size: 4/4 align: 4/4
    deserializer.ReadValue("springDamping", m_springDamping);   // offset: 24/36 size: 4/4 align: 4/4
    // class size: 28/40 align: 4/8
}
