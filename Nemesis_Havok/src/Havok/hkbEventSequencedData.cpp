#include "Havok/hkbEventSequencedData.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbEventSequencedData,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbEventSequencedData::Class; });

nemesis::hkbEventSequencedData::hkbEventSequencedData() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbSequencedData(Class)
{
}

const nemesis::hkClass* nemesis::hkbEventSequencedData::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbEventSequencedData::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbSequencedData::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("events", m_events);         // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}

void nemesis::hkbEventSequencedData::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbSequencedData::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("events", m_events);              // offset: 8/16 size: 12/16 align: 4/8
    // class size: 20/32 align: 4/8
}
