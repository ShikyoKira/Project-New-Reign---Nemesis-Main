#include "Havok/hkbCharacterAddedInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbCharacterAddedInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbCharacterAddedInfo::Class; });

nemesis::hkbCharacterAddedInfo::hkbCharacterAddedInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbCharacterAddedInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbCharacterAddedInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);            // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("characterId", m_characterId);             // offset: 8/16 size: 8/8 align: 8/8
    serializer.WriteValue("instanceName", m_instanceName);           // offset: 16/24 size: 4/8 align: 4/8
    serializer.WriteValue("templateName", m_templateName);           // offset: 20/32 size: 4/8 align: 4/8
    serializer.WriteValue("fullPathToProject", m_fullPathToProject); // offset: 24/40 size: 4/8 align: 4/8

    if (serializer.GetContentsVersion() > HavokVersion::HK_2010_2_0)
    {
        serializer.WriteValue("localScriptsPath", m_localScriptsPath);
        serializer.WriteValue("remoteScriptsPath", m_remoteScriptsPath);
    }

    serializer.WriteObject("skeleton", m_skeleton);                  // offset: 28/48 size: 4/8 align: 4/8
    serializer.Pad(16);                                              // offset: 32/56 size: 0/8

    serializer.WriteValue("worldFromModel", m_worldFromModel);  // offset: 32/64 size: 48/48 align: 16/16
    serializer.WriteObject("poseModelSpace", m_poseModelSpace); // offset: 80/112 size: 12/16 align: 4/8
    serializer.Pad(16);                                         // offset: 92/128 size: 4/0
    // class size: 96/128 align: 16/16
}

void nemesis::hkbCharacterAddedInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer);       // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("characterId", m_characterId);             // offset: 8/16 size: 8/8 align: 8/8
    deserializer.ReadValue("instanceName", m_instanceName);           // offset: 16/24 size: 4/8 align: 4/8
    deserializer.ReadValue("templateName", m_templateName);           // offset: 20/32 size: 4/8 align: 4/8
    deserializer.ReadValue("fullPathToProject", m_fullPathToProject); // offset: 24/40 size: 4/8 align: 4/8

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2010_2_0)
    {
        deserializer.ReadValue("localScriptsPath", m_localScriptsPath);
        deserializer.ReadValue("remoteScriptsPath", m_remoteScriptsPath);
    }

    deserializer.ReadObject("skeleton", m_skeleton);                  // offset: 28/48 size: 4/8 align: 4/8
    deserializer.Pad(16);                                             // offset: 32/56 size: 0/8

    deserializer.ReadValue("worldFromModel", m_worldFromModel);  // offset: 32/64 size: 48/48 align: 16/16
    deserializer.ReadObject("poseModelSpace", m_poseModelSpace); // offset: 80/112 size: 12/16 align: 4/8
    deserializer.Pad(16);                                        // offset: 92/128 size: 4/0
    // class size: 96/128 align: 16/16
}
