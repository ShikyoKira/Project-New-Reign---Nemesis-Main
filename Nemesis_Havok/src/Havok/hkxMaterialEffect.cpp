#include "Havok/hkxMaterialEffect.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxMaterialEffect,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxMaterialEffect::Class; });

nemesis::hkxMaterialEffect::hkxMaterialEffect() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkxMaterialEffect::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxMaterialEffect::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    serializer.WriteValue("name", m_name);       // offset: 8/16 size: 4/8 align: 4/8
    serializer.WriteValue("type", m_type);       // offset: 12/24 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize()); // offset: 13/25 size: 3/7

    serializer.WriteObject("data", m_data); // offset: 16/32 size: 12/16 align: 4/8
    // class size: 28/48 align: 4/8
}

void nemesis::hkxMaterialEffect::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    deserializer.ReadValue("name", m_name);          // offset: 8/16 size: 4/8 align: 4/8
    deserializer.ReadValue("type", m_type);          // offset: 12/24 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize()); // offset: 13/25 size: 3/7

    deserializer.ReadObject("data", m_data); // offset: 16/32 size: 12/16 align: 4/8
    // class size: 28/48 align: 4/8
}
