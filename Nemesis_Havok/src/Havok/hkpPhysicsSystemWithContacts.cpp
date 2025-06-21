#include "Havok/hkpPhysicsSystemWithContacts.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpPhysicsSystemWithContacts,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpPhysicsSystemWithContacts::Class; });

nemesis::hkpPhysicsSystemWithContacts::hkpPhysicsSystemWithContacts() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpPhysicsSystem(Class)
{
}

const nemesis::hkClass* nemesis::hkpPhysicsSystemWithContacts::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpPhysicsSystemWithContacts::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpPhysicsSystem::SerializeTo(serializer); // offset: 0/0 size: 68/104 align: 4/8
    serializer.WriteObject("contacts", m_contacts);     // offset: 68/104 size: 12/16 align: 4/8
    // class size: 80/120 align: 4/8
}

void nemesis::hkpPhysicsSystemWithContacts::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpPhysicsSystem::DeserializeFrom(deserializer); // offset: 0/0 size: 68/104 align: 4/8
    deserializer.ReadObject("contacts", m_contacts);          // offset: 68/104 size: 12/16 align: 4/8
    // class size: 80/120 align: 4/8
}
