#include "Havok/hkbTimerModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbTimerModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbTimerModifier::Class; });

nemesis::hkbTimerModifier::hkbTimerModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::hkbTimerModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbTimerModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer);                 // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteValue("alarmTimeSeconds", m_alarmTimeSeconds); // offset: 44/80 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());                   // offset: 48/84 size: 0/4

    serializer.WriteObject("alarmEvent", m_alarmEvent); // offset: 48/88 size: 8/16 align: 4/8
    serializer.WriteSerializeIgnoredValue("secondsElapsed",
                                          m_secondsElapsed); // offset: 56/104 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());             // offset: 60/108 size: 0/4
    // class size: 60/112 align: 4/8
}

void nemesis::hkbTimerModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer);            // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadValue("alarmTimeSeconds", m_alarmTimeSeconds); // offset: 44/80 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());                // offset: 48/84 size: 0/4

    deserializer.ReadObject("alarmEvent", m_alarmEvent);        // offset: 48/88 size: 8/16 align: 4/8
    deserializer.ReadValue("secondsElapsed", m_secondsElapsed); // offset: 56/104 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());            // offset: 60/108 size: 0/4
    // class size: 60/112 align: 4/8
}
