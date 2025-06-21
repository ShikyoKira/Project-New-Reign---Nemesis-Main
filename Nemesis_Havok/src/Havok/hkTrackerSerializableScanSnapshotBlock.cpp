#include "Havok/hkTrackerSerializableScanSnapshotBlock.h"

REGISTER_HAVOK_POINTER_SOURCE(hkTrackerSerializableScanSnapshotBlock,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkTrackerSerializableScanSnapshotBlock::Class; });

nemesis::hkTrackerSerializableScanSnapshotBlock::hkTrackerSerializableScanSnapshotBlock() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkTrackerSerializableScanSnapshotBlock::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkTrackerSerializableScanSnapshotBlock::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("typeIndex", m_typeIndex); // offset: 0/0 size: 4/4 align: 4/4
    serializer.Skip(4);                              // offset: 4/4 size: 4/4

    serializer.WriteValue("start", m_start);                             // offset: 8/8 size: 8/8 align: 8/8
    serializer.WriteValue("size", m_size);                               // offset: 16/16 size: 8/8 align: 8/8
    serializer.WriteValue("arraySize", m_arraySize);                     // offset: 24/24 size: 4/4 align: 4/4
    serializer.WriteValue("startReferenceIndex", m_startReferenceIndex); // offset: 28/28 size: 4/4 align: 4/4
    serializer.WriteValue("numReferences", m_numReferences);             // offset: 32/32 size: 4/4 align: 4/4
    serializer.Skip(4);                                                  // offset: 36/36 size: 4/4
    // class size: 40/40 align: 8/8
}

void nemesis::hkTrackerSerializableScanSnapshotBlock::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("typeIndex", m_typeIndex); // offset: 0/0 size: 4/4 align: 4/4
    deserializer.Skip(4);                             // offset: 4/4 size: 4/4

    deserializer.ReadValue("start", m_start);         // offset: 8/8 size: 8/8 align: 8/8
    deserializer.ReadValue("size", m_size);           // offset: 16/16 size: 8/8 align: 8/8
    deserializer.ReadValue("arraySize", m_arraySize); // offset: 24/24 size: 4/4 align: 4/4
    deserializer.ReadValue("startReferenceIndex",
                           m_startReferenceIndex);            // offset: 28/28 size: 4/4 align: 4/4
    deserializer.ReadValue("numReferences", m_numReferences); // offset: 32/32 size: 4/4 align: 4/4
    deserializer.Skip(4);                                     // offset: 36/36 size: 4/4
    // class size: 40/40 align: 8/8
}
