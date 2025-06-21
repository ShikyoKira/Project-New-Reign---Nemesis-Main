#include "Havok/hkxMaterialShader.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxMaterialShader,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxMaterialShader::Class; });

nemesis::hkxMaterialShader::hkxMaterialShader() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkxMaterialShader::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxMaterialShader::SerializeTo(nemesis::Serializer& serializer) const
{
    if (serializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    serializer.WriteValue("name", m_name);       // offset: 8/16 size: 4/8 align: 4/8
    serializer.WriteValue("type", m_type);       // offset: 12/24 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize()); // offset: 13/25 size: 3/7

    serializer.WriteValue("vertexEntryName", m_vertexEntryName); // offset: 16/32 size: 4/8 align: 4/8
    serializer.WriteValue("geomEntryName", m_geomEntryName);     // offset: 20/40 size: 4/8 align: 4/8
    serializer.WriteValue("pixelEntryName", m_pixelEntryName);   // offset: 24/48 size: 4/8 align: 4/8
    serializer.WriteObject("data", m_data);                      // offset: 28/56 size: 12/16 align: 4/8
    // class size: 40/72 align: 4/8
}

void nemesis::hkxMaterialShader::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    if (deserializer.GetContentsVersion() > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    deserializer.ReadValue("name", m_name);          // offset: 8/16 size: 4/8 align: 4/8
    deserializer.ReadValue("type", m_type);          // offset: 12/24 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize()); // offset: 13/25 size: 3/7

    deserializer.ReadValue("vertexEntryName", m_vertexEntryName); // offset: 16/32 size: 4/8 align: 4/8
    deserializer.ReadValue("geomEntryName", m_geomEntryName);     // offset: 20/40 size: 4/8 align: 4/8
    deserializer.ReadValue("pixelEntryName", m_pixelEntryName);   // offset: 24/48 size: 4/8 align: 4/8
    deserializer.ReadObject("data", m_data);                      // offset: 28/56 size: 12/16 align: 4/8
    // class size: 40/72 align: 4/8
}
