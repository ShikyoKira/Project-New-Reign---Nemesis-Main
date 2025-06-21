#include "Havok/hkbProjectData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbProjectData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbProjectData::Class; });

nemesis::hkbProjectData::hkbProjectData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbProjectData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbProjectData::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8

    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver <= HavokVersion::HK_2010_1_0)
    {
        serializer.WriteObject("attachmentSetups", m_attachmentSetups);
    }

    serializer.Pad(16); // offset: 8/16 size: 8/0

    serializer.WriteValue("worldUpWS", m_worldUpWS);    // offset: 16/16 size: 16/16 align: 16/16
    serializer.WriteObject("stringData", m_stringData); // offset: 32/32 size: 4/8 align: 4/8

    if (cur_ver > HavokVersion::HK_710)
    {
        serializer.WriteValue("defaultEventMode", m_defaultEventMode); // offset: 36/40 size: 1/1 align: 1/1
    }

    serializer.Pad(16); // offset: 37/41 size: 11/7
    // class size: 48/48 align: 16/16
}

void nemesis::hkbProjectData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8

    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver <= HavokVersion::HK_2010_1_0)
    {
        deserializer.ReadObject("attachmentSetups", m_attachmentSetups);
    }

    deserializer.Pad(16); // offset: 8/16 size: 8/0

    deserializer.ReadValue("worldUpWS", m_worldUpWS);    // offset: 16/16 size: 16/16 align: 16/16
    deserializer.ReadObject("stringData", m_stringData); // offset: 32/32 size: 4/8 align: 4/8

    if (cur_ver > HavokVersion::HK_710)
    {
        deserializer.ReadValue("defaultEventMode", m_defaultEventMode); // offset: 36/40 size: 1/1 align: 1/1
    }

    deserializer.Pad(16); // offset: 37/41 size: 11/7
    // class size: 48/48 align: 16/16
}
