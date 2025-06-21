#include "Havok/hkbEventPayloadList.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbEventPayloadList,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbEventPayloadList::Class; });

nemesis::hkbEventPayloadList::hkbEventPayloadList() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbEventPayload(Class)
{
}

const nemesis::hkClass* nemesis::hkbEventPayloadList::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbEventPayloadList::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbEventPayload::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("payloads", m_payloads);    // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}

void nemesis::hkbEventPayloadList::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbEventPayload::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("payloads", m_payloads);         // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}
