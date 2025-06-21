#include "Havok/hkxMeshSection.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxMeshSection,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxMeshSection::Class; });

nemesis::hkxMeshSection::hkxMeshSection() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkxMeshSection::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxMeshSection::SerializeTo(nemesis::Serializer& serializer) const
{
    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    serializer.WriteObject("vertexBuffer", m_vertexBuffer); // offset: 8/16 size: 4/8 align: 4/8
    serializer.WriteObject("indexBuffers", m_indexBuffers); // offset: 12/24 size: 12/16 align: 4/8
    serializer.WriteObject("material", m_material);         // offset: 24/40 size: 4/8 align: 4/8
    serializer.WriteObject("userChannels", m_userChannels); // offset: 28/48 size: 12/16 align: 4/8

    if (cur_ver > HavokVersion::HK_2011_3_0)
    {
        serializer.WriteObject("vertexAnimations", m_vertexAnimations);
    }

    if (cur_ver > HavokVersion::HK_2012_1_0)
    {
        serializer.WriteObject("linearKeyFrameHints", m_linearKeyFrameHints);
    }

    if (cur_ver > HavokVersion::HK_2013_3_0)
    {
        serializer.WriteObject("boneMatrixMap", m_boneMatrixMap);
    }

    // class size: 40/64 align: 4/8
}

void nemesis::hkxMeshSection::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    deserializer.ReadObject("vertexBuffer", m_vertexBuffer);    // offset: 8/16 size: 4/8 align: 4/8
    deserializer.ReadObject("indexBuffers", m_indexBuffers);    // offset: 12/24 size: 12/16 align: 4/8
    deserializer.ReadObject("material", m_material);            // offset: 24/40 size: 4/8 align: 4/8
    deserializer.ReadObject("userChannels", m_userChannels);    // offset: 28/48 size: 12/16 align: 4/8

    if (cur_ver > HavokVersion::HK_2011_3_0)
    {
        deserializer.ReadObject("vertexAnimations", m_vertexAnimations);
    }

    if (cur_ver > HavokVersion::HK_2012_1_0)
    {
        deserializer.ReadObject("linearKeyFrameHints", m_linearKeyFrameHints);
    }

    if (cur_ver > HavokVersion::HK_2013_3_0)
    {
        deserializer.ReadObject("boneMatrixMap", m_boneMatrixMap);
    }

    // class size: 40/64 align: 4/8
}
