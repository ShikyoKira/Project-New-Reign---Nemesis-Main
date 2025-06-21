#include "Havok/hkpKeyframedRigidMotion.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpKeyframedRigidMotion,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpKeyframedRigidMotion::Class; });

nemesis::hkpKeyframedRigidMotion::hkpKeyframedRigidMotion() noexcept
    : nemesis::hkpKeyframedRigidMotion(Class)
{
}

nemesis::hkpKeyframedRigidMotion::hkpKeyframedRigidMotion(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkpMotion(cls)
{
}

const nemesis::hkClass* nemesis::hkpKeyframedRigidMotion::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpKeyframedRigidMotion::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpMotion::SerializeTo(serializer); // offset: 0/0 size: 288/320 align: 16/16
    // class size: 288/320 align: 16/16
}

void nemesis::hkpKeyframedRigidMotion::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpMotion::DeserializeFrom(deserializer); // offset: 0/0 size: 288/320 align: 16/16
    // class size: 288/320 align: 16/16
}
