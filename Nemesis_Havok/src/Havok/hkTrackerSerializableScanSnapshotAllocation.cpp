#include "Havok/hkTrackerSerializableScanSnapshotAllocation.h"

REGISTER_HAVOK_POINTER_SOURCE(hkTrackerSerializableScanSnapshotAllocation,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkTrackerSerializableScanSnapshotAllocation::Class; });

nemesis::hkTrackerSerializableScanSnapshotAllocation::hkTrackerSerializableScanSnapshotAllocation() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkTrackerSerializableScanSnapshotAllocation::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkTrackerSerializableScanSnapshotAllocation::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("start", m_start);     // offset: 0/0 size: 8/8 align: 8/8
    serializer.WriteValue("size", m_size);       // offset: 8/8 size: 8/8 align: 8/8
    serializer.WriteValue("traceId", m_traceId); // offset: 16/16 size: 4/4 align: 4/4
    serializer.Skip(4);                          // offset: 20/20 size: 4/4
    // class size: 24/24 align: 8/8
}

void nemesis::hkTrackerSerializableScanSnapshotAllocation::DeserializeFrom(
    nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("start", m_start);     // offset: 0/0 size: 8/8 align: 8/8
    deserializer.ReadValue("size", m_size);       // offset: 8/8 size: 8/8 align: 8/8
    deserializer.ReadValue("traceId", m_traceId); // offset: 16/16 size: 4/4 align: 4/4
    deserializer.Skip(4);                         // offset: 20/20 size: 4/4
    // class size: 24/24 align: 8/8
}
