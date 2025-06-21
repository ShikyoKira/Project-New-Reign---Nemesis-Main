#include "Havok/hkpLinkedCollidableCollisionEntry.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpLinkedCollidableCollisionEntry,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpLinkedCollidableCollisionEntry::Class; });

nemesis::hkpLinkedCollidableCollisionEntry::hkpLinkedCollidableCollisionEntry() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpLinkedCollidableCollisionEntry::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpLinkedCollidableCollisionEntry::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("agentEntry", m_agentEntry); // offset: 0/0 size: 4/8 align: 4/8
    serializer.WriteObject("partner", m_partner);       // offset: 4/8 size: 4/8 align: 4/8
    // class size: 8/16 align: 4/8
}

void nemesis::hkpLinkedCollidableCollisionEntry::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("agentEntry", m_agentEntry); // offset: 0/0 size: 4/8 align: 4/8
    deserializer.ReadObject("partner", m_partner);       // offset: 4/8 size: 4/8 align: 4/8
    // class size: 8/16 align: 4/8
}
