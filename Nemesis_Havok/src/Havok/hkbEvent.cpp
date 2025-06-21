#include "Havok/hkbEvent.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbEvent,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbEvent::Class; });

nemesis::hkbEvent::hkbEvent() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbEventBase(Class)
{
}

const nemesis::hkClass* nemesis::hkbEvent::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbEvent::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbEventBase::SerializeTo(serializer);             // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("sender", m_sender); // offset: 8/16 size: 4/8 align: 4/8
    // class size: 12/24 align: 4/8
}

void nemesis::hkbEvent::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbEventBase::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("sender", m_sender);          // offset: 8/16 size: 4/8 align: 4/8
    // class size: 12/24 align: 4/8
}
