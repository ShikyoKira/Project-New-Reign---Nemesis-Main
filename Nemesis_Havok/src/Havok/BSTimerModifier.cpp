#include "Havok/BSTimerModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(BSTimerModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::BSTimerModifier::Class; });

nemesis::BSTimerModifier::BSTimerModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifier(Class)
{
}

const nemesis::hkClass* nemesis::BSTimerModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::BSTimerModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifier::SerializeTo(serializer);                 // offset: 0/0 size: 44/80 align: 4/8
    serializer.WriteValue("alarmTimeSeconds", m_alarmTimeSeconds); // offset: 44/80 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());                   // offset: 48/84 size: 0/4

    serializer.WriteObject("alarmEvent", m_alarmEvent); // offset: 48/88 size: 8/16 align: 4/8
    serializer.WriteValue("resetAlarm", m_resetAlarm);  // offset: 56/104 size: 1/1 align: 1/1
    serializer.Skip(3);                                 // offset: 57/105 size: 3/3

    serializer.WriteSerializeIgnoredValue("secondsElapsed",
                                          m_secondsElapsed); // offset: 60/108 size: 4/4 align: 4/4
    // class size: 64/112 align: 4/8
}

void nemesis::BSTimerModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifier::DeserializeFrom(deserializer);            // offset: 0/0 size: 44/80 align: 4/8
    deserializer.ReadValue("alarmTimeSeconds", m_alarmTimeSeconds); // offset: 44/80 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());                // offset: 48/84 size: 0/4

    deserializer.ReadObject("alarmEvent", m_alarmEvent); // offset: 48/88 size: 8/16 align: 4/8
    deserializer.ReadValue("resetAlarm", m_resetAlarm);  // offset: 56/104 size: 1/1 align: 1/1
    deserializer.Skip(3);                                // offset: 57/105 size: 3/3

    deserializer.ReadValue("secondsElapsed", m_secondsElapsed); // offset: 60/108 size: 4/4 align: 4/4
    // class size: 64/112 align: 4/8
}
