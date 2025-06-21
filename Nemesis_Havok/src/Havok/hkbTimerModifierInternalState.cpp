#include "Havok/hkbTimerModifierInternalState.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbTimerModifierInternalState,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbTimerModifierInternalState::Class; });

nemesis::hkbTimerModifierInternalState::hkbTimerModifierInternalState() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbTimerModifierInternalState::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbTimerModifierInternalState::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);      // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("secondsElapsed", m_secondsElapsed); // offset: 8/16 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());               // offset: 12/20 size: 0/4
    // class size: 12/24 align: 4/8
}

void nemesis::hkbTimerModifierInternalState::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("secondsElapsed", m_secondsElapsed); // offset: 8/16 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());            // offset: 12/20 size: 0/4
    // class size: 12/24 align: 4/8
}
