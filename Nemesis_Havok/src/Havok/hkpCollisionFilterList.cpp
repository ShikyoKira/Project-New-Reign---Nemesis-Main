#include "Havok/hkpCollisionFilterList.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpCollisionFilterList,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpCollisionFilterList::Class; });

nemesis::hkpCollisionFilterList::hkpCollisionFilterList() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpCollisionFilter(Class)
{
}

const nemesis::hkClass* nemesis::hkpCollisionFilterList::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpCollisionFilterList::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpCollisionFilter::SerializeTo(serializer);           // offset: 0/0 size: 48/72 align: 4/8
    serializer.WriteObject("collisionFilters", m_collisionFilters); // offset: 48/72 size: 12/16 align: 4/8
    // class size: 60/88 align: 4/8
}

void nemesis::hkpCollisionFilterList::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpCollisionFilter::DeserializeFrom(deserializer);      // offset: 0/0 size: 48/72 align: 4/8
    deserializer.ReadObject("collisionFilters", m_collisionFilters); // offset: 48/72 size: 12/16 align: 4/8
    // class size: 60/88 align: 4/8
}
