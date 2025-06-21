#include "Havok/hkbBehaviorEventsInfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbBehaviorEventsInfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbBehaviorEventsInfo::Class; });

nemesis::hkbBehaviorEventsInfo::hkbBehaviorEventsInfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbBehaviorEventsInfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbBehaviorEventsInfo::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);           // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("characterId", m_characterId);            // offset: 8/16 size: 8/8 align: 8/8
    serializer.WriteObject("externalEventIds", m_externalEventIds); // offset: 16/24 size: 12/16 align: 4/8

    if (serializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        serializer.WriteValue("padding", m_padding); // offset: 28/40 size: 4/4 align: 4/4
    }

    serializer.Pad(8); // offset: 32/44 size: 0/4
    // class size: 32/48 align: 8/8
}

void nemesis::hkbBehaviorEventsInfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer);      // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("characterId", m_characterId);            // offset: 8/16 size: 8/8 align: 8/8
    deserializer.ReadObject("externalEventIds", m_externalEventIds); // offset: 16/24 size: 12/16 align: 4/8

    if (deserializer.GetContentsVersion() > HavokVersion::HK_2010_1_0)
    {
        deserializer.ReadValue("padding", m_padding); // offset: 28/40 size: 4/4 align: 4/4
    }

    deserializer.Pad(8); // offset: 32/44 size: 0/4
    // class size: 32/48 align: 8/8
}
