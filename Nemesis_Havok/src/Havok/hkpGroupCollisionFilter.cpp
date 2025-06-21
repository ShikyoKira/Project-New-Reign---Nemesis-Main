#include "Havok/hkpGroupCollisionFilter.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpGroupCollisionFilter,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpGroupCollisionFilter::Class; });

nemesis::hkpGroupCollisionFilter::hkpGroupCollisionFilter() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkpCollisionFilter(Class)
{
}

const nemesis::hkClass* nemesis::hkpGroupCollisionFilter::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpGroupCollisionFilter::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkpCollisionFilter::SerializeTo(serializer); // offset: 0/0 size: 48/72 align: 4/8
    serializer.WriteValue("noGroupCollisionEnabled",
                          m_noGroupCollisionEnabled); // offset: 48/72 size: 1/1 align: 1/1
    serializer.Skip(3);                               // offset: 49/73 size: 3/3

    serializer.WriteValue("collisionGroups", m_collisionGroups); // offset: 52/76 size: 128/128 align: 4/4
    serializer.Pad(serializer.GetPointerSize());                 // offset: 180/204 size: 0/4
    // class size: 180/208 align: 4/8
}

void nemesis::hkpGroupCollisionFilter::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkpCollisionFilter::DeserializeFrom(deserializer); // offset: 0/0 size: 48/72 align: 4/8
    deserializer.ReadValue("noGroupCollisionEnabled",
                           m_noGroupCollisionEnabled); // offset: 48/72 size: 1/1 align: 1/1
    deserializer.Skip(3);                              // offset: 49/73 size: 3/3

    deserializer.ReadValue("collisionGroups", m_collisionGroups); // offset: 52/76 size: 128/128 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());              // offset: 180/204 size: 0/4
    // class size: 180/208 align: 4/8
}
