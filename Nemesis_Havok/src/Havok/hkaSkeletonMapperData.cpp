#include "Havok/hkaSkeletonMapperData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkaSkeletonMapperData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkaSkeletonMapperData::Class; });

nemesis::hkaSkeletonMapperData::hkaSkeletonMapperData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkaSkeletonMapperData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkaSkeletonMapperData::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("skeletonA", m_skeletonA); // offset: 0/0 size: 4/8 align: 4/8
    serializer.WriteObject("skeletonB", m_skeletonB); // offset: 4/8 size: 4/8 align: 4/8

    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2011_3_0)
    {
        serializer.WriteObject("partitionMap", m_partitionMap);
        serializer.WriteObject("simpleMappingPartitionRanges", m_simpleMappingPartitionRanges);
        serializer.WriteObject("chainMappingPartitionRanges", m_chainMappingPartitionRanges);
    }

    serializer.WriteObject("simpleMappings", m_simpleMappings); // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteObject("chainMappings", m_chainMappings);   // offset: 20/32 size: 12/16 align: 4/8
    serializer.WriteObject("unmappedBones", m_unmappedBones);   // offset: 32/48 size: 12/16 align: 4/8

    if (cur_ver > HavokVersion::HK_660)
    {
        serializer.Pad(16); // offset: 44/64 size: 4/0

        serializer.WriteValue("extractedMotionMapping",
                              m_extractedMotionMapping); // offset: 48/64 size: 48/48 align: 16/16
        serializer.WriteValue("keepUnmappedLocal",
                              m_keepUnmappedLocal); // offset: 96/112 size: 1/1 align: 1/1
        serializer.Skip(3);                         // offset: 97/113 size: 3/3

        serializer.WriteValue("mappingType", m_mappingType); // offset: 100/116 size: 4/4 align: 4/4
        serializer.Skip(8);                                  // offset: 104/120 size: 8/8
    }
    else
    {
        serializer.WriteValue("keepUnmappedLocal", m_keepUnmappedLocal);
        serializer.Pad(16);
    }

    // class size: 112/128 align: 16/16
}

void nemesis::hkaSkeletonMapperData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("skeletonA", m_skeletonA); // offset: 0/0 size: 4/8 align: 4/8
    deserializer.ReadObject("skeletonB", m_skeletonB); // offset: 4/8 size: 4/8 align: 4/8

    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2011_3_0)
    {
        deserializer.ReadObject("partitionMap", m_partitionMap);
        deserializer.ReadObject("simpleMappingPartitionRanges", m_simpleMappingPartitionRanges);
        deserializer.ReadObject("chainMappingPartitionRanges", m_chainMappingPartitionRanges);
    }

    deserializer.ReadObject("simpleMappings", m_simpleMappings); // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadObject("chainMappings", m_chainMappings);   // offset: 20/32 size: 12/16 align: 4/8
    deserializer.ReadObject("unmappedBones", m_unmappedBones);   // offset: 32/48 size: 12/16 align: 4/8

    if (cur_ver > HavokVersion::HK_660)
    {
        deserializer.Pad(16); // offset: 44/64 size: 4/0

        deserializer.ReadValue("extractedMotionMapping",
                               m_extractedMotionMapping); // offset: 48/64 size: 48/48 align: 16/16
        deserializer.ReadValue("keepUnmappedLocal",
                               m_keepUnmappedLocal); // offset: 96/112 size: 1/1 align: 1/1
        deserializer.Skip(3);                        // offset: 97/113 size: 3/3

        deserializer.ReadValue("mappingType", m_mappingType); // offset: 100/116 size: 4/4 align: 4/4
        deserializer.Skip(8);                                 // offset: 104/120 size: 8/8
    }
    else
    {
        deserializer.ReadValue("keepUnmappedLocal", m_keepUnmappedLocal);
        deserializer.Pad(16);
    }

    // class size: 112/128 align: 16/16
}
