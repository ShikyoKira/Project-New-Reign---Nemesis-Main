#include "Havok/hkTrackerSerializableScanSnapshot.h"

REGISTER_HAVOK_POINTER_SOURCE(hkTrackerSerializableScanSnapshot,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkTrackerSerializableScanSnapshot::Class; });

nemesis::hkTrackerSerializableScanSnapshot::hkTrackerSerializableScanSnapshot() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkTrackerSerializableScanSnapshot::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkTrackerSerializableScanSnapshot::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);   // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("allocations", m_allocations);   // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteObject("blocks", m_blocks);             // offset: 20/32 size: 12/16 align: 4/8
    serializer.WriteObject("refs", m_refs);                 // offset: 32/48 size: 12/16 align: 4/8
    serializer.WriteObject("typeNames", m_typeNames);       // offset: 44/64 size: 12/16 align: 4/8
    serializer.WriteObject("traceText", m_traceText);       // offset: 56/80 size: 12/16 align: 4/8
    serializer.WriteObject("traceAddrs", m_traceAddrs);     // offset: 68/96 size: 12/16 align: 4/8
    serializer.WriteObject("traceParents", m_traceParents); // offset: 80/112 size: 12/16 align: 4/8
    // class size: 92/128 align: 4/8
}

void nemesis::hkTrackerSerializableScanSnapshot::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("allocations", m_allocations);      // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadObject("blocks", m_blocks);                // offset: 20/32 size: 12/16 align: 4/8
    deserializer.ReadObject("refs", m_refs);                    // offset: 32/48 size: 12/16 align: 4/8
    deserializer.ReadObject("typeNames", m_typeNames);          // offset: 44/64 size: 12/16 align: 4/8
    deserializer.ReadObject("traceText", m_traceText);          // offset: 56/80 size: 12/16 align: 4/8
    deserializer.ReadObject("traceAddrs", m_traceAddrs);        // offset: 68/96 size: 12/16 align: 4/8
    deserializer.ReadObject("traceParents", m_traceParents);    // offset: 80/112 size: 12/16 align: 4/8
    // class size: 92/128 align: 4/8
}
