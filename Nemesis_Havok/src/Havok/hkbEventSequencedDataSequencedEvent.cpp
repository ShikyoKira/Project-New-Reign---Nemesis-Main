#include "Havok/hkbEventSequencedDataSequencedEvent.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbEventSequencedDataSequencedEvent,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbEventSequencedDataSequencedEvent::Class; });

nemesis::hkbEventSequencedDataSequencedEvent::hkbEventSequencedDataSequencedEvent() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbEventSequencedDataSequencedEvent::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbEventSequencedDataSequencedEvent::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteObject("event", m_event);    // offset: 0/0 size: 12/24 align: 4/8
    serializer.WriteValue("time", m_time);       // offset: 12/24 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize()); // offset: 16/28 size: 0/4
    // class size: 16/32 align: 4/8
}

void nemesis::hkbEventSequencedDataSequencedEvent::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadObject("event", m_event);       // offset: 0/0 size: 12/24 align: 4/8
    deserializer.ReadValue("time", m_time);          // offset: 12/24 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize()); // offset: 16/28 size: 0/4
    // class size: 16/32 align: 4/8
}
