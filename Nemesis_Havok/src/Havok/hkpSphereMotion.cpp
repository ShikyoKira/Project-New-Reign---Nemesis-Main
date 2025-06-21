#include "Havok/hkpSphereMotion.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpSphereMotion,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpSphereMotion::Class; });

nemesis::hkpSphereMotion::hkpSphereMotion() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpMotion(Class)
{
}

const nemesis::hkClass* nemesis::hkpSphereMotion::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpSphereMotion::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpMotion::SerializeTo(serializer); // offset: 0/0 size: 288/320 align: 16/16
    // class size: 288/320 align: 16/16
}

void nemesis::hkpSphereMotion::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpMotion::DeserializeFrom(deserializer); // offset: 0/0 size: 288/320 align: 16/16
    // class size: 288/320 align: 16/16
}
