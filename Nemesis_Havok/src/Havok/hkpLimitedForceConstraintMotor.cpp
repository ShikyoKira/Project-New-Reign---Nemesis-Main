#include "Havok/hkpLimitedForceConstraintMotor.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpLimitedForceConstraintMotor,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpLimitedForceConstraintMotor::Class; });

nemesis::hkpLimitedForceConstraintMotor::hkpLimitedForceConstraintMotor() noexcept
    : nemesis::hkpLimitedForceConstraintMotor(Class)
{
}

nemesis::hkpLimitedForceConstraintMotor::hkpLimitedForceConstraintMotor(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkpConstraintMotor(cls)
{
}

const nemesis::hkClass* nemesis::hkpLimitedForceConstraintMotor::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpLimitedForceConstraintMotor::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpConstraintMotor::SerializeTo(serializer); // offset: 0/0 size: 12/24 align: 4/8
    serializer.WriteValue("minForce", m_minForce);        // offset: 12/24 size: 4/4 align: 4/4
    serializer.WriteValue("maxForce", m_maxForce);        // offset: 16/28 size: 4/4 align: 4/4
    // class size: 20/32 align: 4/8
}

void nemesis::hkpLimitedForceConstraintMotor::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpConstraintMotor::DeserializeFrom(deserializer); // offset: 0/0 size: 12/24 align: 4/8
    deserializer.ReadValue("minForce", m_minForce);             // offset: 12/24 size: 4/4 align: 4/4
    deserializer.ReadValue("maxForce", m_maxForce);             // offset: 16/28 size: 4/4 align: 4/4
    // class size: 20/32 align: 4/8
}
