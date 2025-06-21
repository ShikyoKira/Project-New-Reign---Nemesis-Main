#include "Havok/hkbNamedEventPayload.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbNamedEventPayload,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbNamedEventPayload::Class; });

nemesis::hkbNamedEventPayload::hkbNamedEventPayload() noexcept
    : nemesis::hkbNamedEventPayload(Class)
{
}

nemesis::hkbNamedEventPayload::hkbNamedEventPayload(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkbEventPayload(cls)
{
}

const nemesis::hkClass* nemesis::hkbNamedEventPayload::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbNamedEventPayload::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbEventPayload::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("name", m_name);             // offset: 8/16 size: 4/8 align: 4/8
    // class size: 12/24 align: 4/8
}

void nemesis::hkbNamedEventPayload::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbEventPayload::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("name", m_name);                  // offset: 8/16 size: 4/8 align: 4/8
    // class size: 12/24 align: 4/8
}
