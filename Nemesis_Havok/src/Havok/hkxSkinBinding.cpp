#include "Havok/hkxSkinBinding.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxSkinBinding,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxSkinBinding::Class; });

nemesis::hkxSkinBinding::hkxSkinBinding() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkxSkinBinding::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxSkinBinding::SerializeTo(nemesis::Serializer& serializer) const
{
    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    serializer.WriteObject("mesh", m_mesh); // offset: 8/16 size: 4/8 align: 4/8

    if (cur_ver > HavokVersion::HK_700)
    {
        serializer.WriteObject("nodeNames", m_nodeNames); // offset: 12/24 size: 12/16 align: 4/8
    }
    else
    {
        serializer.WriteObject("mapping", m_mapping);
        serializer.WriteValue("numMapping", m_numMapping);
    }

    serializer.WriteObject("bindPose", m_bindPose); // offset: 24/40 size: 12/16 align: 4/8
    serializer.Pad(16);                             // offset: 36/56 size: 12/8

    serializer.WriteValue("initSkinTransform", m_initSkinTransform); // offset: 48/64 size: 64/64 align: 16/16
    // class size: 112/128 align: 16/16
}

void nemesis::hkxSkinBinding::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    deserializer.ReadObject("mesh", m_mesh); // offset: 8/16 size: 4/8 align: 4/8

    if (cur_ver > HavokVersion::HK_700)
    {
        deserializer.ReadObject("nodeNames", m_nodeNames); // offset: 12/24 size: 12/16 align: 4/8
    }
    else
    {
        deserializer.ReadObject("mapping", m_mapping);
        deserializer.ReadValue("numMapping", m_numMapping);
    }

    deserializer.ReadObject("bindPose", m_bindPose); // offset: 24/40 size: 12/16 align: 4/8
    deserializer.Pad(16);                            // offset: 36/56 size: 12/8

    deserializer.ReadValue("initSkinTransform",
                           m_initSkinTransform); // offset: 48/64 size: 64/64 align: 16/16
    // class size: 112/128 align: 16/16
}
