#include "Havok/hkbEventsFromRangeModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbEventsFromRangeModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbEventsFromRangeModifier::Class; });

nemesis::hkbEventsFromRangeModifier::hkbEventsFromRangeModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbEventsFromRangeModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbEventsFromRangeModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer);        // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteValue("inputValue", m_inputValue);    // offset: 44/80 size: 4/4 align: 4/4
    serializer.WriteValue("lowerBound", m_lowerBound);    // offset: 48/84 size: 4/4 align: 4/4
    serializer.WriteObject("eventRanges", m_eventRanges); // offset: 52/88 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject(
        "wasActiveInPreviousFrame", m_wasActiveInPreviousFrame); // offset: 56/96 size: 12/16 align: 4/8
    // class size: 68/112 align: 4/8
}

void nemesis::hkbEventsFromRangeModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer);   // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadValue("inputValue", m_inputValue);    // offset: 44/80 size: 4/4 align: 4/4
    deserializer.ReadValue("lowerBound", m_lowerBound);    // offset: 48/84 size: 4/4 align: 4/4
    deserializer.ReadObject("eventRanges", m_eventRanges); // offset: 52/88 size: 4/8 align: 4/8
    deserializer.ReadObject("wasActiveInPreviousFrame",
                            m_wasActiveInPreviousFrame); // offset: 56/96 size: 12/16 align: 4/8
    // class size: 68/112 align: 4/8
}
