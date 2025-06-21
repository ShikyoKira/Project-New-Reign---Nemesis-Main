#include "Havok/hkaSkeletonLocalFrameOnBone.h"

REGISTER_HAVOK_POINTER_SOURCE(hkaSkeletonLocalFrameOnBone,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkaSkeletonLocalFrameOnBone::Class; });

nemesis::hkaSkeletonLocalFrameOnBone::hkaSkeletonLocalFrameOnBone() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkaSkeletonLocalFrameOnBone::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkaSkeletonLocalFrameOnBone::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("localFrame", m_localFrame); // offset: 0/0 size: 4/8 align: 4/8

    if (serializer.GetContentsVersion() >= HavokVersion::HK_2011_3_0)
    {
        serializer.WriteValue("boneIndex", static_cast<short>(m_boneIndex));
    }
    else
    {
        serializer.WriteValue("boneIndex", m_boneIndex); // offset: 4/8 size: 4/4 align: 4/4
    }

    serializer.Pad(serializer.GetPointerSize()); // offset: 8/12 size: 0/4
    // class size: 8/16 align: 4/8
}

void nemesis::hkaSkeletonLocalFrameOnBone::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("localFrame", m_localFrame); // offset: 0/0 size: 4/8 align: 4/8

    if (deserializer.GetContentsVersion() >= HavokVersion::HK_2011_3_0)
    {
        short bone_index;
        deserializer.ReadValue("boneIndex", bone_index);
        m_boneIndex = bone_index;
    }
    else
    {
        deserializer.ReadValue("boneIndex", m_boneIndex); // offset: 4/8 size: 4/4 align: 4/4
    }

    deserializer.Pad(deserializer.GetPointerSize()); // offset: 8/12 size: 0/4
    // class size: 8/16 align: 4/8
}
