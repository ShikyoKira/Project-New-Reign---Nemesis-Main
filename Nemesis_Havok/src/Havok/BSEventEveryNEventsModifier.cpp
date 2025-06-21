#include "Havok/BSEventEveryNEventsModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(BSEventEveryNEventsModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::BSEventEveryNEventsModifier::Class; });

nemesis::BSEventEveryNEventsModifier::BSEventEveryNEventsModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::BSEventEveryNEventsModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::BSEventEveryNEventsModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer);                // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteObject("eventToCheckFor", m_eventToCheckFor); // offset: 44/80 size: 8/16 align: 4/8
    serializer.WriteObject("eventToSend", m_eventToSend);         // offset: 52/96 size: 8/16 align: 4/8
    serializer.WriteValue("numberOfEventsBeforeSend",
                          m_numberOfEventsBeforeSend); // offset: 60/112 size: 1/1 align: 1/1
    serializer.WriteValue("minimumNumberOfEventsBeforeSend",
                          m_minimumNumberOfEventsBeforeSend); // offset: 61/113 size: 1/1 align: 1/1
    serializer.WriteValue("randomizeNumberOfEvents",
                          m_randomizeNumberOfEvents); // offset: 62/114 size: 1/1 align: 1/1
    serializer.Skip(1);                               // offset: 63/115 size: 1/1

    serializer.WriteSerializeIgnoredValue("numberOfEventsSeen",
                                          m_numberOfEventsSeen); // offset: 64/116 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue(
        "calculatedNumberOfEventsBeforeSend",
        m_calculatedNumberOfEventsBeforeSend);   // offset: 68/120 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize()); // offset: 69/121 size: 3/7
    // class size: 72/128 align: 4/8
}

void nemesis::BSEventEveryNEventsModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer);           // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadObject("eventToCheckFor", m_eventToCheckFor); // offset: 44/80 size: 8/16 align: 4/8
    deserializer.ReadObject("eventToSend", m_eventToSend);         // offset: 52/96 size: 8/16 align: 4/8
    deserializer.ReadValue("numberOfEventsBeforeSend",
                           m_numberOfEventsBeforeSend); // offset: 60/112 size: 1/1 align: 1/1
    deserializer.ReadValue("minimumNumberOfEventsBeforeSend",
                           m_minimumNumberOfEventsBeforeSend); // offset: 61/113 size: 1/1 align: 1/1
    deserializer.ReadValue("randomizeNumberOfEvents",
                           m_randomizeNumberOfEvents); // offset: 62/114 size: 1/1 align: 1/1
    deserializer.Skip(1);                              // offset: 63/115 size: 1/1

    deserializer.ReadValue("numberOfEventsSeen", m_numberOfEventsSeen); // offset: 64/116 size: 4/4 align: 4/4
    deserializer.ReadValue("calculatedNumberOfEventsBeforeSend",
                           m_calculatedNumberOfEventsBeforeSend); // offset: 68/120 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());              // offset: 69/121 size: 3/7
    // class size: 72/128 align: 4/8
}
