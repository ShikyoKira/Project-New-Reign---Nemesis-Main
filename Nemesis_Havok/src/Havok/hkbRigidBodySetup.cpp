#include "Havok/hkbRigidBodySetup.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbRigidBodySetup,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbRigidBodySetup::Class; });

nemesis::hkbRigidBodySetup::hkbRigidBodySetup() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbRigidBodySetup::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbRigidBodySetup::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("collisionFilterInfo", m_collisionFilterInfo); // offset: 0/0 size: 4/4 align: 4/4
    serializer.WriteValue("type", m_type);                               // offset: 4/4 size: 1/1 align: 1/1
    serializer.Skip(3);                                                  // offset: 5/5 size: 3/3

    serializer.WriteObject("shapeSetup", m_shapeSetup); // offset: 8/8 size: 16/24 align: 4/8
    // class size: 24/32 align: 4/8
}

void nemesis::hkbRigidBodySetup::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("collisionFilterInfo", m_collisionFilterInfo); // offset: 0/0 size: 4/4 align: 4/4
    deserializer.ReadValue("type", m_type);                               // offset: 4/4 size: 1/1 align: 1/1
    deserializer.Skip(3);                                                 // offset: 5/5 size: 3/3

    deserializer.ReadObject("shapeSetup", m_shapeSetup); // offset: 8/8 size: 16/24 align: 4/8
    // class size: 24/32 align: 4/8
}
