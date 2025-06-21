#include "Havok/hkbCharacterStringData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbCharacterStringData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbCharacterStringData::Class; });

nemesis::hkbCharacterStringData::hkbCharacterStringData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbCharacterStringData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbCharacterStringData::SerializeTo(nemesis::Serializer& serializer) const
{
    auto cur_ver = serializer.GetContentsVersion();

    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8

    if (cur_ver > HavokVersion::HK_2010_2_0)
    {
        serializer.WriteObject("skinNames", m_skinNames);
        serializer.WriteObject("boneAttachmentNames", m_boneAttachmentNames);
    }
    else
    {
        serializer.WriteObject("deformableSkinNames",
                               m_deformableSkinNames);              // offset: 8/16 size: 12/16 align: 4/8
        serializer.WriteObject("rigidSkinNames", m_rigidSkinNames); // offset: 20/32 size: 12/16 align: 4/8
    }

    if (cur_ver > HavokVersion::HK_2011_3_0)
    {
        serializer.WriteObject("animationBundleNameData", m_animationBundleNameData);
        serializer.WriteObject("animationBundleFilenameData", m_animationBundleFilenameData);
    }
    else
    {
        serializer.WriteObject("animationNames", m_animationNames); // offset: 32/48 size: 12/16 align: 4/8
        serializer.WriteObject("animationFilenames",
                               m_animationFilenames); // offset: 44/64 size: 12/16 align: 4/8
    }

    serializer.WriteObject("characterPropertyNames",
                           m_characterPropertyNames); // offset: 56/80 size: 12/16 align: 4/8
    serializer.WriteObject("retargetingSkeletonMapperFilenames",
                           m_retargetingSkeletonMapperFilenames); // offset: 68/96 size: 12/16 align: 4/8
    serializer.WriteObject("lodNames", m_lodNames);               // offset: 80/112 size: 12/16 align: 4/8

    if (cur_ver > HavokVersion::HK_2010_1_0)
    {
        serializer.WriteObject("mirroredSyncPointSubstringsA",
                               m_mirroredSyncPointSubstringsA); // offset: 92/128 size: 12/16 align: 4/8
        serializer.WriteObject("mirroredSyncPointSubstringsB",
                               m_mirroredSyncPointSubstringsB); // offset: 104/144 size: 12/16 align: 4/8
    }

    serializer.WriteValue("name", m_name);                         // offset: 116/160 size: 4/8 align: 4/8
    serializer.WriteValue("rigName", m_rigName);                   // offset: 120/168 size: 4/8 align: 4/8
    serializer.WriteValue("ragdollName", m_ragdollName);           // offset: 124/176 size: 4/8 align: 4/8
    serializer.WriteValue("behaviorFilename", m_behaviorFilename); // offset: 128/184 size: 4/8 align: 4/8

    if (cur_ver > HavokVersion::HK_2010_2_0)
    {
        serializer.WriteValue("luaScriptOnCharacterActivated", m_luaScriptOnCharacterActivated);
        serializer.WriteValue("luaScriptOnCharacterDeactivated", m_luaScriptOnCharacterDeactivated);
        serializer.WriteObject("luaFiles", m_luaFiles);
    }

    // class size: 132/192 align: 4/8
}

void nemesis::hkbCharacterStringData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    auto cur_ver = deserializer.GetContentsVersion();

    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8

    if (cur_ver > HavokVersion::HK_2010_2_0)
    {
        deserializer.ReadObject("skinNames", m_skinNames);
        deserializer.ReadObject("boneAttachmentNames", m_boneAttachmentNames);
    }
    else
    {
        deserializer.ReadObject("deformableSkinNames",
                                m_deformableSkinNames);              // offset: 8/16 size: 12/16 align: 4/8
        deserializer.ReadObject("rigidSkinNames", m_rigidSkinNames); // offset: 20/32 size: 12/16 align: 4/8
    }

    if (cur_ver > HavokVersion::HK_2011_3_0)
    {
        deserializer.ReadObject("animationBundleNameData", m_animationBundleNameData);
        deserializer.ReadObject("animationBundleFilenameData", m_animationBundleFilenameData);
    }
    else
    {
        deserializer.ReadObject("animationNames", m_animationNames); // offset: 32/48 size: 12/16 align: 4/8
        deserializer.ReadObject("animationFilenames",
                                m_animationFilenames); // offset: 44/64 size: 12/16 align: 4/8
    }

    deserializer.ReadObject("characterPropertyNames",
                            m_characterPropertyNames); // offset: 56/80 size: 12/16 align: 4/8
    deserializer.ReadObject("retargetingSkeletonMapperFilenames",
                            m_retargetingSkeletonMapperFilenames); // offset: 68/96 size: 12/16 align: 4/8
    deserializer.ReadObject("lodNames", m_lodNames);               // offset: 80/112 size: 12/16 align: 4/8

    if (cur_ver > HavokVersion::HK_2010_1_0)
    {
        deserializer.ReadObject("mirroredSyncPointSubstringsA",
                                m_mirroredSyncPointSubstringsA); // offset: 92/128 size: 12/16 align: 4/8
        deserializer.ReadObject("mirroredSyncPointSubstringsB",
                                m_mirroredSyncPointSubstringsB); // offset: 104/144 size: 12/16 align: 4/8
    }

    deserializer.ReadValue("name", m_name);                         // offset: 116/160 size: 4/8 align: 4/8
    deserializer.ReadValue("rigName", m_rigName);                   // offset: 120/168 size: 4/8 align: 4/8
    deserializer.ReadValue("ragdollName", m_ragdollName);           // offset: 124/176 size: 4/8 align: 4/8
    deserializer.ReadValue("behaviorFilename", m_behaviorFilename); // offset: 128/184 size: 4/8 align: 4/8

    if (cur_ver > HavokVersion::HK_2010_2_0)
    {
        deserializer.ReadValue("luaScriptOnCharacterActivated", m_luaScriptOnCharacterActivated);
        deserializer.ReadValue("luaScriptOnCharacterDeactivated", m_luaScriptOnCharacterDeactivated);
        deserializer.ReadObject("luaFiles", m_luaFiles);
    }

    // class size: 132/192 align: 4/8
}
