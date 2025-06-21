#include "Havok/hkbNamedIntEventPayload.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbNamedIntEventPayload,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbNamedIntEventPayload::Class; });

nemesis::hkbNamedIntEventPayload::hkbNamedIntEventPayload() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbNamedEventPayload(Class)
{
}

const nemesis::hkClass* nemesis::hkbNamedIntEventPayload::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbNamedIntEventPayload::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbNamedEventPayload::SerializeTo(serializer); // offset: 0/0 size: 12/24 align: 4/8
    serializer.WriteValue("data", m_data);                  // offset: 12/24 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());            // offset: 16/28 size: 0/4
    // class size: 16/32 align: 4/8
}

void nemesis::hkbNamedIntEventPayload::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbNamedEventPayload::DeserializeFrom(deserializer); // offset: 0/0 size: 12/24 align: 4/8
    deserializer.ReadValue("data", m_data);                       // offset: 12/24 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());              // offset: 16/28 size: 0/4
    // class size: 16/32 align: 4/8
}
