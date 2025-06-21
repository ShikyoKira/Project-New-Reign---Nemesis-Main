#include "Havok/hkaSkeletonMapper.h"

REGISTER_HAVOK_POINTER_SOURCE(hkaSkeletonMapper,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkaSkeletonMapper::Class; });

nemesis::hkaSkeletonMapper::hkaSkeletonMapper() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkaSkeletonMapper::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkaSkeletonMapper::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.Pad(16);                                   // offset: 8/16 size: 8/0

    serializer.WriteObject("mapping", m_mapping); // offset: 16/16 size: 112/128 align: 16/16
    // class size: 128/144 align: 16/16
}

void nemesis::hkaSkeletonMapper::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.Pad(16);                                       // offset: 8/16 size: 8/0

    deserializer.ReadObject("mapping", m_mapping); // offset: 16/16 size: 112/128 align: 16/16
    // class size: 128/144 align: 16/16
}
