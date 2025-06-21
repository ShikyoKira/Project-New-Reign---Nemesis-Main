#include "Havok/hkaMeshBinding.h"

REGISTER_HAVOK_POINTER_SOURCE(hkaMeshBinding,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkaMeshBinding::Class; });

nemesis::hkaMeshBinding::hkaMeshBinding() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkaMeshBinding::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkaMeshBinding::SerializeTo(nemesis::Serializer& serializer) const
{
    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    serializer.WriteObject("mesh", m_mesh);               // offset: 8/16 size: 4/8 align: 4/8
    serializer.WriteValue("originalSkeletonName",
                          m_originalSkeletonName);  // offset: 12/24 size: 4/8 align: 4/8

    if (serializer.GetContentsVersion() > HavokVersion::HK_2010_2_0)
    {
        serializer.WriteValue("name", m_name);
    }

    serializer.WriteObject("skeleton", m_skeleton); // offset: 16/32 size: 4/8 align: 4/8
    serializer.WriteObject("mappings", m_mappings); // offset: 20/40 size: 12/16 align: 4/8
    serializer.WriteObject("boneFromSkinMeshTransforms",
                           m_boneFromSkinMeshTransforms); // offset: 32/56 size: 12/16 align: 4/8
    // class size: 44/72 align: 4/8
}

void nemesis::hkaMeshBinding::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    deserializer.ReadObject("mesh", m_mesh);                    // offset: 8/16 size: 4/8 align: 4/8
    deserializer.ReadValue("originalSkeletonName",
                           m_originalSkeletonName);  // offset: 12/24 size: 4/8 align: 4/8

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2010_2_0)
    {
        deserializer.ReadValue("name", m_name);
    }

    deserializer.ReadObject("skeleton", m_skeleton); // offset: 16/32 size: 4/8 align: 4/8
    deserializer.ReadObject("mappings", m_mappings); // offset: 20/40 size: 12/16 align: 4/8
    deserializer.ReadObject("boneFromSkinMeshTransforms",
                            m_boneFromSkinMeshTransforms); // offset: 32/56 size: 12/16 align: 4/8
    // class size: 44/72 align: 4/8
}
