#include "Havok/hkbNamedStringEventPayload.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbNamedStringEventPayload,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbNamedStringEventPayload::Class; });

nemesis::hkbNamedStringEventPayload::hkbNamedStringEventPayload() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbNamedEventPayload(Class)
{
}

const nemesis::hkClass* nemesis::hkbNamedStringEventPayload::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbNamedStringEventPayload::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbNamedEventPayload::SerializeTo(serializer); // offset: 0/0 size: 12/24 align: 4/8
    serializer.WriteValue("data", m_data);                  // offset: 12/24 size: 4/8 align: 4/8
    // class size: 16/32 align: 4/8
}

void nemesis::hkbNamedStringEventPayload::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbNamedEventPayload::DeserializeFrom(deserializer); // offset: 0/0 size: 12/24 align: 4/8
    deserializer.ReadValue("data", m_data);                       // offset: 12/24 size: 4/8 align: 4/8
    // class size: 16/32 align: 4/8
}
