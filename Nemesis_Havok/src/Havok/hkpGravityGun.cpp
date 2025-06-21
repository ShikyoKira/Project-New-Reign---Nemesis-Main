#include "Havok/hkpGravityGun.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpGravityGun,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpGravityGun::Class; });

nemesis::hkpGravityGun::hkpGravityGun() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpFirstPersonGun(Class)
{
}

const nemesis::hkClass* nemesis::hkpGravityGun::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpGravityGun::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpFirstPersonGun::SerializeTo(serializer); // offset: 0/0 size: 32/56 align: 4/8
    serializer.WriteSerializeIgnoredObject("grabbedBodies",
                                           m_grabbedBodies); // offset: 32/56 size: 12/16 align: 4/8
    serializer.WriteValue("maxNumObjectsPicked", m_maxNumObjectsPicked); // offset: 44/72 size: 4/4 align: 4/4
    serializer.WriteValue("maxMassOfObjectPicked",
                          m_maxMassOfObjectPicked); // offset: 48/76 size: 4/4 align: 4/4
    serializer.WriteValue("maxDistOfObjectPicked",
                          m_maxDistOfObjectPicked); // offset: 52/80 size: 4/4 align: 4/4
    serializer.WriteValue("impulseAppliedWhenObjectNotPicked",
                          m_impulseAppliedWhenObjectNotPicked); // offset: 56/84 size: 4/4 align: 4/4
    serializer.WriteValue("throwVelocity", m_throwVelocity);    // offset: 60/88 size: 4/4 align: 4/4
    serializer.Pad(16);                                         // offset: 64/92 size: 0/4

    serializer.WriteValue("capturedObjectPosition",
                          m_capturedObjectPosition); // offset: 64/96 size: 16/16 align: 16/16
    serializer.WriteValue("capturedObjectsOffset",
                          m_capturedObjectsOffset); // offset: 80/112 size: 16/16 align: 16/16
    // class size: 96/128 align: 16/16
}

void nemesis::hkpGravityGun::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpFirstPersonGun::DeserializeFrom(deserializer); // offset: 0/0 size: 32/56 align: 4/8
    deserializer.ReadObject("grabbedBodies", m_grabbedBodies); // offset: 32/56 size: 12/16 align: 4/8
    deserializer.ReadValue("maxNumObjectsPicked",
                           m_maxNumObjectsPicked); // offset: 44/72 size: 4/4 align: 4/4
    deserializer.ReadValue("maxMassOfObjectPicked",
                           m_maxMassOfObjectPicked); // offset: 48/76 size: 4/4 align: 4/4
    deserializer.ReadValue("maxDistOfObjectPicked",
                           m_maxDistOfObjectPicked); // offset: 52/80 size: 4/4 align: 4/4
    deserializer.ReadValue("impulseAppliedWhenObjectNotPicked",
                           m_impulseAppliedWhenObjectNotPicked); // offset: 56/84 size: 4/4 align: 4/4
    deserializer.ReadValue("throwVelocity", m_throwVelocity);    // offset: 60/88 size: 4/4 align: 4/4
    deserializer.Pad(16);                                        // offset: 64/92 size: 0/4

    deserializer.ReadValue("capturedObjectPosition",
                           m_capturedObjectPosition); // offset: 64/96 size: 16/16 align: 16/16
    deserializer.ReadValue("capturedObjectsOffset",
                           m_capturedObjectsOffset); // offset: 80/112 size: 16/16 align: 16/16
    // class size: 96/128 align: 16/16
}
