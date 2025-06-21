#include "Havok/hkbStringEventPayload.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbStringEventPayload,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbStringEventPayload::Class; });

nemesis::hkbStringEventPayload::hkbStringEventPayload() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbEventPayload(Class)
{
}

const nemesis::hkClass* nemesis::hkbStringEventPayload::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbStringEventPayload::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbEventPayload::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("data", m_data);             // offset: 8/16 size: 4/8 align: 4/8
    // class size: 12/24 align: 4/8
}

void nemesis::hkbStringEventPayload::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbEventPayload::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("data", m_data);                  // offset: 8/16 size: 4/8 align: 4/8
    // class size: 12/24 align: 4/8
}
