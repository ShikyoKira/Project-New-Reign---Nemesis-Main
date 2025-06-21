#include "Havok/hkpRigidBody.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpRigidBody,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpRigidBody::Class; });

nemesis::hkpRigidBody::hkpRigidBody() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpEntity(Class)
{
}

const nemesis::hkClass* nemesis::hkpRigidBody::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpRigidBody::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpEntity::SerializeTo(serializer); // offset: 0/0 size: 544/720 align: 16/16
    // class size: 544/720 align: 16/16
}

void nemesis::hkpRigidBody::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpEntity::DeserializeFrom(deserializer); // offset: 0/0 size: 544/720 align: 16/16
    // class size: 544/720 align: 16/16
}
