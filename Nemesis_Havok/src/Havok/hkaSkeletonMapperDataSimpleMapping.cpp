#include "Havok/hkaSkeletonMapperDataSimpleMapping.h"

REGISTER_HAVOK_POINTER_SOURCE(hkaSkeletonMapperDataSimpleMapping,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkaSkeletonMapperDataSimpleMapping::Class; });

nemesis::hkaSkeletonMapperDataSimpleMapping::hkaSkeletonMapperDataSimpleMapping() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkaSkeletonMapperDataSimpleMapping::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkaSkeletonMapperDataSimpleMapping::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("boneA", m_boneA); // offset: 0/0 size: 2/2 align: 2/2
    serializer.WriteValue("boneB", m_boneB); // offset: 2/2 size: 2/2 align: 2/2
    serializer.Skip(12);                     // offset: 4/4 size: 12/12

    serializer.WriteValue("aFromBTransform", m_aFromBTransform); // offset: 16/16 size: 48/48 align: 16/16
    // class size: 64/64 align: 16/16
}

void nemesis::hkaSkeletonMapperDataSimpleMapping::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("boneA", m_boneA); // offset: 0/0 size: 2/2 align: 2/2
    deserializer.ReadValue("boneB", m_boneB); // offset: 2/2 size: 2/2 align: 2/2
    deserializer.Skip(12);                    // offset: 4/4 size: 12/12

    deserializer.ReadValue("aFromBTransform", m_aFromBTransform); // offset: 16/16 size: 48/48 align: 16/16
    // class size: 64/64 align: 16/16
}
