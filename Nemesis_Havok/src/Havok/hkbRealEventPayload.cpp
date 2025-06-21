#include "Havok/hkbRealEventPayload.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbRealEventPayload,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbRealEventPayload::Class; });

nemesis::hkbRealEventPayload::hkbRealEventPayload() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbEventPayload(Class)
{
}

const nemesis::hkClass* nemesis::hkbRealEventPayload::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbRealEventPayload::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbEventPayload::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("data", m_data);             // offset: 8/16 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());       // offset: 12/20 size: 0/4
    // class size: 12/24 align: 4/8
}

void nemesis::hkbRealEventPayload::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbEventPayload::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("data", m_data);                  // offset: 8/16 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());         // offset: 12/20 size: 0/4
    // class size: 12/24 align: 4/8
}
