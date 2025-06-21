#include "Havok/hkaSkeletonMapperDataPartitionMappingRange.h"

REGISTER_HAVOK_POINTER_SOURCE(hkaSkeletonMapperDataPartitionMappingRange,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkaSkeletonMapperDataPartitionMappingRange::Class; });

nemesis::hkaSkeletonMapperDataPartitionMappingRange::hkaSkeletonMapperDataPartitionMappingRange() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkaSkeletonMapperDataPartitionMappingRange::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkaSkeletonMapperDataPartitionMappingRange::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("startMappingIndex", m_startMappingIndex); // offset: 0/0 size: 4/4 align: 4/4
    serializer.WriteValue("numMappings", m_numMappings);             // offset: 4/4 size: 4/4 align: 4/4
    // class size: 8/8 align: 4/4
}

void nemesis::hkaSkeletonMapperDataPartitionMappingRange::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("startMappingIndex", m_startMappingIndex); // offset: 0/0 size: 4/4 align: 4/4
    deserializer.ReadValue("numMappings", m_numMappings);             // offset: 4/4 size: 4/4 align: 4/4
    // class size: 8/8 align: 4/4
}
