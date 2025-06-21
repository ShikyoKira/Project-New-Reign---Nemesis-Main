#include "Havok/hkpMaxSizeMotion.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpMaxSizeMotion,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpMaxSizeMotion::Class; });

nemesis::hkpMaxSizeMotion::hkpMaxSizeMotion() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpKeyframedRigidMotion(Class)
{
}

const nemesis::hkClass* nemesis::hkpMaxSizeMotion::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpMaxSizeMotion::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpKeyframedRigidMotion::SerializeTo(serializer); // offset: 0/0 size: 288/320 align: 16/16
    // class size: 288/320 align: 16/16
}

void nemesis::hkpMaxSizeMotion::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpKeyframedRigidMotion::DeserializeFrom(deserializer); // offset: 0/0 size: 288/320 align: 16/16
    // class size: 288/320 align: 16/16
}
