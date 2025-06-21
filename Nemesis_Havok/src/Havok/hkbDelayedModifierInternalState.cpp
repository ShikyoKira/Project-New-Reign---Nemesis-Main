#include "Havok/hkbDelayedModifierInternalState.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbDelayedModifierInternalState,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbDelayedModifierInternalState::Class; });

nemesis::hkbDelayedModifierInternalState::hkbDelayedModifierInternalState() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbDelayedModifierInternalState::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbDelayedModifierInternalState::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);      // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("secondsElapsed", m_secondsElapsed); // offset: 8/16 size: 4/4 align: 4/4
    serializer.WriteValue("isActive", m_isActive);             // offset: 12/20 size: 1/1 align: 1/1
    serializer.Skip(3);                                        // offset: 13/21 size: 3/3
    // class size: 16/24 align: 4/8
}

void nemesis::hkbDelayedModifierInternalState::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("secondsElapsed", m_secondsElapsed); // offset: 8/16 size: 4/4 align: 4/4
    deserializer.ReadValue("isActive", m_isActive);             // offset: 12/20 size: 1/1 align: 1/1
    deserializer.Skip(3);                                       // offset: 13/21 size: 3/3
    // class size: 16/24 align: 4/8
}
