#include "Havok/hkbEventPayload.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbEventPayload,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbEventPayload::Class; });

nemesis::hkbEventPayload::hkbEventPayload() noexcept
    : nemesis::hkbEventPayload(Class)
{
}

nemesis::hkbEventPayload::hkbEventPayload(const nemesis::hkClass& cls) noexcept
    : nemesis::HavokObject(cls.GetSignature())
    , nemesis::hkReferencedObject(cls)
{
}

const nemesis::hkClass* nemesis::hkbEventPayload::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbEventPayload::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    // class size: 8/16 align: 4/8
}

void nemesis::hkbEventPayload::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    // class size: 8/16 align: 4/8
}
