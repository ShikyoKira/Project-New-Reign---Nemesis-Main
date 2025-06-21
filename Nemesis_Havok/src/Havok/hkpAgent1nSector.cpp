#include "Havok/hkpAgent1nSector.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpAgent1nSector,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpAgent1nSector::Class; });

nemesis::hkpAgent1nSector::hkpAgent1nSector() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkpAgent1nSector::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpAgent1nSector::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("bytesAllocated", m_bytesAllocated); // offset: 0/0 size: 4/4 align: 4/4
    serializer.WriteValue("pad0", m_pad0);                     // offset: 4/4 size: 4/4 align: 4/4
    serializer.WriteValue("pad1", m_pad1);                     // offset: 8/8 size: 4/4 align: 4/4
    serializer.WriteValue("pad2", m_pad2);                     // offset: 12/12 size: 4/4 align: 4/4
    serializer.WriteValue("data", m_data);                     // offset: 16/16 size: 496/496 align: 1/1
    // class size: 512/512 align: 4/4
}

void nemesis::hkpAgent1nSector::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("bytesAllocated", m_bytesAllocated); // offset: 0/0 size: 4/4 align: 4/4
    deserializer.ReadValue("pad0", m_pad0);                     // offset: 4/4 size: 4/4 align: 4/4
    deserializer.ReadValue("pad1", m_pad1);                     // offset: 8/8 size: 4/4 align: 4/4
    deserializer.ReadValue("pad2", m_pad2);                     // offset: 12/12 size: 4/4 align: 4/4
    deserializer.ReadValue("data", m_data);                     // offset: 16/16 size: 496/496 align: 1/1
    // class size: 512/512 align: 4/4
}
