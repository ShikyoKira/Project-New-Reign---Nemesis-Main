#include "Havok/hkxScene.h"

REGISTER_HAVOK_POINTER_SOURCE(hkxScene,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkxScene::Class; });

nemesis::hkxScene::hkxScene() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkxScene::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkxScene::SerializeTo(nemesis::Serializer& serializer) const
{
    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    serializer.WriteValue("modeller", m_modeller);       // offset: 8/16 size: 4/8 align: 4/8
    serializer.WriteValue("asset", m_asset);             // offset: 12/24 size: 4/8 align: 4/8
    serializer.WriteValue("sceneLength", m_sceneLength); // offset: 16/32 size: 4/4 align: 4/4

    if (cur_ver > HavokVersion::HK_2011_3_0)
    {
        serializer.WriteValue("numFrames", m_numFrames);
    }

    serializer.Pad(serializer.GetPointerSize()); // offset: 20/36 size: 0/4

    serializer.WriteObject("rootNode", m_rootNode);                 // offset: 20/40 size: 4/8 align: 4/8
    serializer.WriteObject("selectionSets", m_selectionSets);       // offset: 24/48 size: 12/16 align: 4/8
    serializer.WriteObject("cameras", m_cameras);                   // offset: 36/64 size: 12/16 align: 4/8
    serializer.WriteObject("lights", m_lights);                     // offset: 48/80 size: 12/16 align: 4/8
    serializer.WriteObject("meshes", m_meshes);                     // offset: 60/96 size: 12/16 align: 4/8
    serializer.WriteObject("materials", m_materials);               // offset: 72/112 size: 12/16 align: 4/8
    serializer.WriteObject("inplaceTextures", m_inplaceTextures);   // offset: 84/128 size: 12/16 align: 4/8
    serializer.WriteObject("externalTextures", m_externalTextures); // offset: 96/144 size: 12/16 align: 4/8
    serializer.WriteObject("skinBindings", m_skinBindings);         // offset: 108/160 size: 12/16 align: 4/8

    if (cur_ver > HavokVersion::HK_2011_3_0)
    {
        serializer.WriteObject("splines", m_splines);
    }

    serializer.Pad(16); // offset: 120/176 size: 8/0

    serializer.WriteValue("appliedTransform", m_appliedTransform); // offset: 128/176 size: 48/48 align: 16/16
    // class size: 176/224 align: 16/16
}

void nemesis::hkxScene::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_660)
    {
        nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    }

    deserializer.ReadValue("modeller", m_modeller);       // offset: 8/16 size: 4/8 align: 4/8
    deserializer.ReadValue("asset", m_asset);             // offset: 12/24 size: 4/8 align: 4/8
    deserializer.ReadValue("sceneLength", m_sceneLength); // offset: 16/32 size: 4/4 align: 4/4

    if (cur_ver > HavokVersion::HK_2011_3_0)
    {
        deserializer.ReadValue("numFrames", m_numFrames);
    }

    deserializer.Pad(deserializer.GetPointerSize()); // offset: 20/36 size: 0/4

    deserializer.ReadObject("rootNode", m_rootNode);                 // offset: 20/40 size: 4/8 align: 4/8
    deserializer.ReadObject("selectionSets", m_selectionSets);       // offset: 24/48 size: 12/16 align: 4/8
    deserializer.ReadObject("cameras", m_cameras);                   // offset: 36/64 size: 12/16 align: 4/8
    deserializer.ReadObject("lights", m_lights);                     // offset: 48/80 size: 12/16 align: 4/8
    deserializer.ReadObject("meshes", m_meshes);                     // offset: 60/96 size: 12/16 align: 4/8
    deserializer.ReadObject("materials", m_materials);               // offset: 72/112 size: 12/16 align: 4/8
    deserializer.ReadObject("inplaceTextures", m_inplaceTextures);   // offset: 84/128 size: 12/16 align: 4/8
    deserializer.ReadObject("externalTextures", m_externalTextures); // offset: 96/144 size: 12/16 align: 4/8
    deserializer.ReadObject("skinBindings", m_skinBindings);         // offset: 108/160 size: 12/16 align: 4/8

    if (cur_ver > HavokVersion::HK_2011_3_0)
    {
        deserializer.ReadObject("splines", m_splines);
    }

    deserializer.Pad(16); // offset: 120/176 size: 8/0

    deserializer.ReadValue("appliedTransform",
                           m_appliedTransform); // offset: 128/176 size: 48/48 align: 16/16
    // class size: 176/224 align: 16/16
}
