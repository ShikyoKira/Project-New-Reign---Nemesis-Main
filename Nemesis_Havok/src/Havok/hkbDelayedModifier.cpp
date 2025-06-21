#include "Havok/hkbDelayedModifier.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbDelayedModifier,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbDelayedModifier::Class; });

nemesis::hkbDelayedModifier::hkbDelayedModifier() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkbModifierWrapper(Class)
{
}

const nemesis::hkClass* nemesis::hkbDelayedModifier::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbDelayedModifier::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkbModifierWrapper::SerializeTo(serializer);        // offset: 0/0 size: 48/88 align: 4/8
    serializer.WriteValue("delaySeconds", m_delaySeconds);       // offset: 48/88 size: 4/4 align: 4/4
    serializer.WriteValue("durationSeconds", m_durationSeconds); // offset: 52/92 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("secondsElapsed",
                                          m_secondsElapsed);       // offset: 56/96 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("isActive", m_isActive); // offset: 60/100 size: 1/1 align: 1/1
    serializer.Skip(3);                                            // offset: 61/101 size: 3/3
    // class size: 64/104 align: 4/8
}

void nemesis::hkbDelayedModifier::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkbModifierWrapper::DeserializeFrom(deserializer);   // offset: 0/0 size: 48/88 align: 4/8
    deserializer.ReadValue("delaySeconds", m_delaySeconds);       // offset: 48/88 size: 4/4 align: 4/4
    deserializer.ReadValue("durationSeconds", m_durationSeconds); // offset: 52/92 size: 4/4 align: 4/4
    deserializer.ReadValue("secondsElapsed", m_secondsElapsed);   // offset: 56/96 size: 4/4 align: 4/4
    deserializer.ReadValue("isActive", m_isActive);               // offset: 60/100 size: 1/1 align: 1/1
    deserializer.Skip(3);                                         // offset: 61/101 size: 3/3
    // class size: 64/104 align: 4/8
}
