#include "Havok/BSComputeAddBoneAnimModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(BSComputeAddBoneAnimModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::BSComputeAddBoneAnimModifier::Class; });

nemesis::BSComputeAddBoneAnimModifier::BSComputeAddBoneAnimModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::BSComputeAddBoneAnimModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::BSComputeAddBoneAnimModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer);   // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteValue("boneIndex", m_boneIndex); // offset: 44/80 size: 2/2 align: 2/2
    serializer.Pad(16);                              // offset: 46/82 size: 2/14

    serializer.WriteValue("translationLSOut", m_translationLSOut); // offset: 48/96 size: 16/16 align: 16/16
    serializer.WriteValue("rotationLSOut", m_rotationLSOut);       // offset: 64/112 size: 16/16 align: 16/16
    serializer.WriteValue("scaleLSOut", m_scaleLSOut);             // offset: 80/128 size: 16/16 align: 16/16
    serializer.WriteSerializeIgnoredObject("pSkeletonMemory",
                                           m_pSkeletonMemory); // offset: 96/144 size: 4/8 align: 4/8
    serializer.Pad(16);                                        // offset: 100/152 size: 12/8
    // class size: 112/160 align: 16/16
}

void nemesis::BSComputeAddBoneAnimModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer); // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadValue("boneIndex", m_boneIndex);    // offset: 44/80 size: 2/2 align: 2/2
    deserializer.Pad(16);                                // offset: 46/82 size: 2/14

    deserializer.ReadValue("translationLSOut", m_translationLSOut); // offset: 48/96 size: 16/16 align: 16/16
    deserializer.ReadValue("rotationLSOut", m_rotationLSOut);       // offset: 64/112 size: 16/16 align: 16/16
    deserializer.ReadValue("scaleLSOut", m_scaleLSOut);             // offset: 80/128 size: 16/16 align: 16/16
    deserializer.ReadObject("pSkeletonMemory", m_pSkeletonMemory);  // offset: 96/144 size: 4/8 align: 4/8
    deserializer.Pad(16);                                           // offset: 100/152 size: 12/8
    // class size: 112/160 align: 16/16
}
