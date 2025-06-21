#include "Havok/hkaSkeletonMapperDataChainMapping.h"

REGISTER_HAVOK_POINTER_SOURCE(hkaSkeletonMapperDataChainMapping,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkaSkeletonMapperDataChainMapping::Class; });

nemesis::hkaSkeletonMapperDataChainMapping::hkaSkeletonMapperDataChainMapping() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkaSkeletonMapperDataChainMapping::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkaSkeletonMapperDataChainMapping::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("startBoneA", m_startBoneA); // offset: 0/0 size: 2/2 align: 2/2
    serializer.WriteValue("endBoneA", m_endBoneA);     // offset: 2/2 size: 2/2 align: 2/2
    serializer.WriteValue("startBoneB", m_startBoneB); // offset: 4/4 size: 2/2 align: 2/2
    serializer.WriteValue("endBoneB", m_endBoneB);     // offset: 6/6 size: 2/2 align: 2/2
    serializer.Skip(8);                                // offset: 8/8 size: 8/8

    serializer.WriteValue("startAFromBTransform",
                          m_startAFromBTransform); // offset: 16/16 size: 48/48 align: 16/16
    serializer.WriteValue("endAFromBTransform",
                          m_endAFromBTransform); // offset: 64/64 size: 48/48 align: 16/16
    // class size: 112/112 align: 16/16
}

void nemesis::hkaSkeletonMapperDataChainMapping::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("startBoneA", m_startBoneA); // offset: 0/0 size: 2/2 align: 2/2
    deserializer.ReadValue("endBoneA", m_endBoneA);     // offset: 2/2 size: 2/2 align: 2/2
    deserializer.ReadValue("startBoneB", m_startBoneB); // offset: 4/4 size: 2/2 align: 2/2
    deserializer.ReadValue("endBoneB", m_endBoneB);     // offset: 6/6 size: 2/2 align: 2/2
    deserializer.Skip(8);                               // offset: 8/8 size: 8/8

    deserializer.ReadValue("startAFromBTransform",
                           m_startAFromBTransform); // offset: 16/16 size: 48/48 align: 16/16
    deserializer.ReadValue("endAFromBTransform",
                           m_endAFromBTransform); // offset: 64/64 size: 48/48 align: 16/16
    // class size: 112/112 align: 16/16
}
