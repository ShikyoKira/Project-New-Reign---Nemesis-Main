#include "Havok/hkpPhysicsData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpPhysicsData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpPhysicsData::Class; });

nemesis::hkpPhysicsData::hkpPhysicsData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkpPhysicsData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpPhysicsData::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("worldCinfo", m_worldCinfo);   // offset: 8/16 size: 4/8 align: 4/8
    serializer.WriteObject("systems", m_systems);         // offset: 12/24 size: 12/16 align: 4/8
    // class size: 24/40 align: 4/8
}

void nemesis::hkpPhysicsData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("worldCinfo", m_worldCinfo);        // offset: 8/16 size: 4/8 align: 4/8
    deserializer.ReadObject("systems", m_systems);              // offset: 12/24 size: 12/16 align: 4/8
    // class size: 24/40 align: 4/8
}
