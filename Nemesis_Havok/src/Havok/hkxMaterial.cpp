#include "Havok/hkxMaterial.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxMaterial,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxMaterial::Class; });

nemesis::hkxMaterial::hkxMaterial() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkxAttributeHolder(Class)
{
}

const nemesis::hkClass* nemesis::hkxMaterial::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxMaterial::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkxAttributeHolder::SerializeTo(serializer); // offset: 0/0 size: 20/32 align: 4/8
    serializer.WriteValue("name", m_name);                // offset: 20/32 size: 4/8 align: 4/8
    serializer.WriteObject("stages", m_stages);           // offset: 24/40 size: 12/16 align: 4/8
    serializer.Pad(16);                                   // offset: 36/56 size: 12/8

    serializer.WriteValue("diffuseColor", m_diffuseColor);   // offset: 48/64 size: 16/16 align: 16/16
    serializer.WriteValue("ambientColor", m_ambientColor);   // offset: 64/80 size: 16/16 align: 16/16
    serializer.WriteValue("specularColor", m_specularColor); // offset: 80/96 size: 16/16 align: 16/16
    serializer.WriteValue("emissiveColor", m_emissiveColor); // offset: 96/112 size: 16/16 align: 16/16
    serializer.WriteObject("subMaterials", m_subMaterials);  // offset: 112/128 size: 12/16 align: 4/8
    serializer.WriteObject("extraData", m_extraData);        // offset: 124/144 size: 4/8 align: 4/8

    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2011_3_0)
    {
        serializer.WriteValue("uvMapScale", m_uvMapScale);
        serializer.WriteValue("uvMapOffset", m_uvMapOffset);
        serializer.WriteValue("uvMapRotation", m_uvMapRotation);
        serializer.WriteValue("uvMapAlgorithm", m_uvMapAlgorithm);
        serializer.WriteValue("specularMultiplier", m_specularMultiplier);
        serializer.WriteValue("specularExponent", m_specularExponent);
        serializer.WriteValue("transparency", m_transparency);
    }

    if (cur_ver > HavokVersion::HK_2013_1_0)
    {
        serializer.Pad(serializer.GetPointerSize());
        serializer.WriteValue("userData", m_userData);
    }

    if (cur_ver > HavokVersion::HK_710)
    {
        serializer.Pad(serializer.GetPointerSize());
        serializer.WriteObject("properties", m_properties); // offset: 128/152 size: 12/16 align: 4/8
    }

    serializer.Pad(16); // offset: 140/168 size: 4/8
    // class size: 144/176 align: 16/16
}

void nemesis::hkxMaterial::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkxAttributeHolder::DeserializeFrom(deserializer); // offset: 0/0 size: 20/32 align: 4/8
    deserializer.ReadValue("name", m_name);                     // offset: 20/32 size: 4/8 align: 4/8
    deserializer.ReadObject("stages", m_stages);                // offset: 24/40 size: 12/16 align: 4/8
    deserializer.Pad(16);                                       // offset: 36/56 size: 12/8

    deserializer.ReadValue("diffuseColor", m_diffuseColor);   // offset: 48/64 size: 16/16 align: 16/16
    deserializer.ReadValue("ambientColor", m_ambientColor);   // offset: 64/80 size: 16/16 align: 16/16
    deserializer.ReadValue("specularColor", m_specularColor); // offset: 80/96 size: 16/16 align: 16/16
    deserializer.ReadValue("emissiveColor", m_emissiveColor); // offset: 96/112 size: 16/16 align: 16/16
    deserializer.ReadObject("subMaterials", m_subMaterials);  // offset: 112/128 size: 12/16 align: 4/8
    deserializer.ReadObject("extraData", m_extraData);        // offset: 124/144 size: 4/8 align: 4/8

    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_2011_3_0)
    {
        deserializer.ReadValue("uvMapScale", m_uvMapScale);
        deserializer.ReadValue("uvMapOffset", m_uvMapOffset);
        deserializer.ReadValue("uvMapRotation", m_uvMapRotation);
        deserializer.ReadValue("uvMapAlgorithm", m_uvMapAlgorithm);
        deserializer.ReadValue("specularMultiplier", m_specularMultiplier);
        deserializer.ReadValue("specularExponent", m_specularExponent);
        deserializer.ReadValue("transparency", m_transparency);
    }

    if (cur_ver > HavokVersion::HK_2013_1_0)
    {
        deserializer.Pad(deserializer.GetPointerSize());
        deserializer.ReadValue("userData", m_userData);
    }

    if (cur_ver > HavokVersion::HK_710)
    {
        deserializer.Pad(deserializer.GetPointerSize());
        deserializer.ReadObject("properties", m_properties); // offset: 128/152 size: 12/16 align: 4/8
    }

    deserializer.Pad(16); // offset: 140/168 size: 4/8
    // class size: 144/176 align: 16/16
}
