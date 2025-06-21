#include "Havok/hkbEventRaisedInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbEventRaisedInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbEventRaisedInfo::Class; });

nemesis::hkbEventRaisedInfo::hkbEventRaisedInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbEventRaisedInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbEventRaisedInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("characterId", m_characterId);  // offset: 8/16 size: 8/8 align: 8/8
    serializer.WriteValue("eventName", m_eventName);      // offset: 16/24 size: 4/8 align: 4/8
    serializer.WriteValue("raisedBySdk", m_raisedBySdk);  // offset: 20/32 size: 1/1 align: 1/1

    if (serializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        serializer.Skip(3); // offset: 21/33 size: 3/3

        serializer.WriteValue("senderId", m_senderId); // offset: 24/36 size: 4/4 align: 4/4
        serializer.WriteValue("padding", m_padding);   // offset: 28/40 size: 4/4 align: 4/4
    }

    serializer.Pad(8);                           // offset: 32/44 size: 0/4
    // class size: 32/48 align: 8/8
}

void nemesis::hkbEventRaisedInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("characterId", m_characterId);       // offset: 8/16 size: 8/8 align: 8/8
    deserializer.ReadValue("eventName", m_eventName);           // offset: 16/24 size: 4/8 align: 4/8
    deserializer.ReadValue("raisedBySdk", m_raisedBySdk);       // offset: 20/32 size: 1/1 align: 1/1

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        deserializer.Skip(3); // offset: 21/33 size: 3/3

        deserializer.ReadValue("senderId", m_senderId); // offset: 24/36 size: 4/4 align: 4/4
        deserializer.ReadValue("padding", m_padding);   // offset: 28/40 size: 4/4 align: 4/4
    }

    deserializer.Pad(8);                          // offset: 32/44 size: 0/4
    // class size: 32/48 align: 8/8
}
