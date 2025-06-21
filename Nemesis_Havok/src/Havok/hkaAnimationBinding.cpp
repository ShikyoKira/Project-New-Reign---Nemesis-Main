#include "Havok/hkaAnimationBinding.h"

REGISTER_HAVOK_POINTER_SOURCE(hkaAnimationBinding,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkaAnimationBinding::Class; });

nemesis::hkaAnimationBinding::hkaAnimationBinding() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkaAnimationBinding::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkaAnimationBinding::SerializeTo(nemesis::Serializer& serializer) const
{
    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    serializer.WriteValue("originalSkeletonName",
                          m_originalSkeletonName);    // offset: 8/16 size: 4/8 align: 4/8
    serializer.WriteObject("animation", m_animation); // offset: 12/24 size: 4/8 align: 4/8
    serializer.WriteObject("transformTrackToBoneIndices",
                           m_transformTrackToBoneIndices); // offset: 16/32 size: 12/16 align: 4/8
    serializer.WriteObject("floatTrackToFloatSlotIndices",
                           m_floatTrackToFloatSlotIndices); // offset: 28/48 size: 12/16 align: 4/8

    if (cur_ver > HavokVersion::HK_2011_3_0)
    {
        serializer.WriteObject("partitionIndices", m_partitionIndices);
    }

    serializer.WriteValue("blendHint", m_blendHint);        // offset: 40/64 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());            // offset: 41/65 size: 3/7
    // class size: 44/72 align: 4/8
}

void nemesis::hkaAnimationBinding::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    deserializer.ReadValue("originalSkeletonName",
                           m_originalSkeletonName);    // offset: 8/16 size: 4/8 align: 4/8
    deserializer.ReadObject("animation", m_animation); // offset: 12/24 size: 4/8 align: 4/8
    deserializer.ReadObject("transformTrackToBoneIndices",
                            m_transformTrackToBoneIndices); // offset: 16/32 size: 12/16 align: 4/8
    deserializer.ReadObject("floatTrackToFloatSlotIndices",
                            m_floatTrackToFloatSlotIndices); // offset: 28/48 size: 12/16 align: 4/8

    if (cur_ver > HavokVersion::HK_2011_3_0)
    {
        deserializer.ReadObject("partitionIndices", m_partitionIndices);
    }

    deserializer.ReadValue("blendHint", m_blendHint);        // offset: 40/64 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());         // offset: 41/65 size: 3/7
    // class size: 44/72 align: 4/8
}
