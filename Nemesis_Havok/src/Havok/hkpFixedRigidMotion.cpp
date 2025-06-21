#include "Havok/hkpFixedRigidMotion.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpFixedRigidMotion,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpFixedRigidMotion::Class; });

nemesis::hkpFixedRigidMotion::hkpFixedRigidMotion() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpKeyframedRigidMotion(Class)
{
}

const nemesis::hkClass* nemesis::hkpFixedRigidMotion::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpFixedRigidMotion::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpKeyframedRigidMotion::SerializeTo(serializer); // offset: 0/0 size: 288/320 align: 16/16
    // class size: 288/320 align: 16/16
}

void nemesis::hkpFixedRigidMotion::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpKeyframedRigidMotion::DeserializeFrom(deserializer); // offset: 0/0 size: 288/320 align: 16/16
    // class size: 288/320 align: 16/16
}
