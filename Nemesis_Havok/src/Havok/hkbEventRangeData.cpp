#include "Havok/hkbEventRangeData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbEventRangeData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbEventRangeData::Class; });

nemesis::hkbEventRangeData::hkbEventRangeData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
{
}

const nemesis::hkClass* nemesis::hkbEventRangeData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbEventRangeData::SerializeTo(nemesis::Serializer& serializer) const
{
    serializer.WriteValue("upperBound", m_upperBound); // offset: 0/0 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());       // offset: 4/4 size: 0/4

    serializer.WriteObject("event", m_event);        // offset: 4/8 size: 8/16 align: 4/8
    serializer.WriteValue("eventMode", m_eventMode); // offset: 12/24 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());     // offset: 13/25 size: 3/7
    // class size: 16/32 align: 4/8
}

void nemesis::hkbEventRangeData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    deserializer.ReadValue("upperBound", m_upperBound); // offset: 0/0 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());    // offset: 4/4 size: 0/4

    deserializer.ReadObject("event", m_event);        // offset: 4/8 size: 8/16 align: 4/8
    deserializer.ReadValue("eventMode", m_eventMode); // offset: 12/24 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());  // offset: 13/25 size: 3/7
    // class size: 16/32 align: 4/8
}
