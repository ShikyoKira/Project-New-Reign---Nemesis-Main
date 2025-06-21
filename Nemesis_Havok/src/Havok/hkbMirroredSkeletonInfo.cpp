#include "Havok/hkbMirroredSkeletonInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbMirroredSkeletonInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbMirroredSkeletonInfo::Class; });

nemesis::hkbMirroredSkeletonInfo::hkbMirroredSkeletonInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbMirroredSkeletonInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbMirroredSkeletonInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.Pad(16);                                   // offset: 8/16 size: 8/0

    serializer.WriteValue("mirrorAxis", m_mirrorAxis);    // offset: 16/16 size: 16/16 align: 16/16
    serializer.WriteObject("bonePairMap", m_bonePairMap); // offset: 32/32 size: 12/16 align: 4/8

    if (serializer.GetContentsVersion() > HavokVersion::HK_2011_3_0)
    {
        serializer.WriteObject("partitionPairMap", m_partitionPairMap);
    }

    serializer.Pad(16); // offset: 44/48 size: 4/0
    // class size: 48/48 align: 16/16
}

void nemesis::hkbMirroredSkeletonInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.Pad(16);                                       // offset: 8/16 size: 8/0

    deserializer.ReadValue("mirrorAxis", m_mirrorAxis);    // offset: 16/16 size: 16/16 align: 16/16
    deserializer.ReadObject("bonePairMap", m_bonePairMap); // offset: 32/32 size: 12/16 align: 4/8

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2011_3_0)
    {
        deserializer.ReadObject("partitionPairMap", m_partitionPairMap);
    }

    deserializer.Pad(16); // offset: 44/48 size: 4/0
    // class size: 48/48 align: 16/16
}
