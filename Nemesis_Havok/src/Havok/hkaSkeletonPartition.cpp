#include "Havok/hkaSkeletonPartition.h"

REGISTER_HAVOK_POINTER_SOURCE(hkaSkeletonPartition,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkaSkeletonPartition::Class; });

nemesis::hkaSkeletonPartition::hkaSkeletonPartition() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkaSkeletonPartition::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkaSkeletonPartition::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("name", m_name);                     // offset: 0/0 size: 4/8 align: 4/8
    serializer.WriteValue("startBoneIndex", m_startBoneIndex); // offset: 4/8 size: 2/2 align: 2/2
    serializer.WriteValue("numBones", m_numBones);             // offset: 6/10 size: 2/2 align: 2/2
    serializer.Pad(serializer.GetPointerSize());               // offset: 8/12 size: 0/4
    // class size: 8/16 align: 4/8
}

void nemesis::hkaSkeletonPartition::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("name", m_name);                     // offset: 0/0 size: 4/8 align: 4/8
    deserializer.ReadValue("startBoneIndex", m_startBoneIndex); // offset: 4/8 size: 2/2 align: 2/2
    deserializer.ReadValue("numBones", m_numBones);             // offset: 6/10 size: 2/2 align: 2/2
    deserializer.Pad(deserializer.GetPointerSize());            // offset: 8/12 size: 0/4
    // class size: 8/16 align: 4/8
}
