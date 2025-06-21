#include "Havok/hkxMesh.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxMesh,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxMesh::Class; });

nemesis::hkxMesh::hkxMesh() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkxMesh::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxMesh::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    serializer.WriteObject("sections", m_sections);                 // offset: 8/16 size: 12/16 align: 4/8
    serializer.WriteObject("userChannelInfos", m_userChannelInfos); // offset: 20/32 size: 12/16 align: 4/8
    // class size: 32/48 align: 4/8
}

void nemesis::hkxMesh::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    deserializer.ReadObject("sections", m_sections);                 // offset: 8/16 size: 12/16 align: 4/8
    deserializer.ReadObject("userChannelInfos", m_userChannelInfos); // offset: 20/32 size: 12/16 align: 4/8
    // class size: 32/48 align: 4/8
}
