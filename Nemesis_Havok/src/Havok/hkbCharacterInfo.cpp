#include "Havok/hkbCharacterInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbCharacterInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbCharacterInfo::Class; });

nemesis::hkbCharacterInfo::hkbCharacterInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbCharacterInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbCharacterInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("characterId", m_characterId);  // offset: 8/16 size: 8/8 align: 8/8

    if (serializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        serializer.WriteValue("event", m_event); // offset: 16/24 size: 1/1 align: 1/1
        serializer.Skip(3);                      // offset: 17/25 size: 3/3

        serializer.WriteValue("padding", m_padding); // offset: 20/28 size: 4/4 align: 4/4
    }
    else
    {
        serializer.WriteValue("instanceName", m_instanceName);
        serializer.WriteValue("templateName", m_templateName);
        serializer.WriteValue("fullPathToProject", m_fullPathToProject);
        serializer.WriteValue("event", m_event);
        serializer.Pad(8);
    }

    // class size: 24/32 align: 8/8
}

void nemesis::hkbCharacterInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("characterId", m_characterId);       // offset: 8/16 size: 8/8 align: 8/8

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        deserializer.ReadValue("event", m_event); // offset: 16/24 size: 1/1 align: 1/1
        deserializer.Skip(3);                     // offset: 17/25 size: 3/3

        deserializer.ReadValue("padding", m_padding); // offset: 20/28 size: 4/4 align: 4/4
    }
    else
    {
        deserializer.ReadValue("instanceName", m_instanceName);
        deserializer.ReadValue("templateName", m_templateName);
        deserializer.ReadValue("fullPathToProject", m_fullPathToProject);
        deserializer.ReadValue("event", m_event);
        deserializer.Pad(8);
    }

    // class size: 24/32 align: 8/8
}
