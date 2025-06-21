#include "Havok/hkbCharacterSkinInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbCharacterSkinInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbCharacterSkinInfo::Class; });

nemesis::hkbCharacterSkinInfo::hkbCharacterSkinInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbCharacterSkinInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbCharacterSkinInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("characterId", m_characterId);  // offset: 8/16 size: 8/8 align: 8/8

    if (serializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        serializer.WriteObject("deformableSkins", m_deformableSkins); // offset: 16/24 size: 12/16 align: 4/8
        serializer.WriteObject("rigidSkins", m_rigidSkins);           // offset: 28/40 size: 12/16 align: 4/8
    }
    else
    {
        serializer.WriteObject("skinIds", m_skinIds);
    }

    // class size: 40/56 align: 8/8
}

void nemesis::hkbCharacterSkinInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("characterId", m_characterId);       // offset: 8/16 size: 8/8 align: 8/8

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        deserializer.ReadObject("deformableSkins", m_deformableSkins); // offset: 16/24 size: 12/16 align: 4/8
        deserializer.ReadObject("rigidSkins", m_rigidSkins);           // offset: 28/40 size: 12/16 align: 4/8
    }
    else
    {
        deserializer.ReadObject("skinIds", m_skinIds);
    }

    // class size: 40/56 align: 8/8
}
