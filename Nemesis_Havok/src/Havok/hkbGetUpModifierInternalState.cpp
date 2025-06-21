#include "Havok/hkbGetUpModifierInternalState.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbGetUpModifierInternalState,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbGetUpModifierInternalState::Class; });

nemesis::hkbGetUpModifierInternalState::hkbGetUpModifierInternalState() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbGetUpModifierInternalState::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbGetUpModifierInternalState::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer);      // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteValue("timeSinceBegin", m_timeSinceBegin); // offset: 8/16 size: 4/4 align: 4/4
    serializer.WriteValue("timeStep", m_timeStep);             // offset: 12/20 size: 4/4 align: 4/4
    serializer.WriteValue("initNextModify", m_initNextModify); // offset: 16/24 size: 1/1 align: 1/1
    serializer.Pad(serializer.GetPointerSize());               // offset: 17/25 size: 3/7
    // class size: 20/32 align: 4/8
}

void nemesis::hkbGetUpModifierInternalState::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadValue("timeSinceBegin", m_timeSinceBegin); // offset: 8/16 size: 4/4 align: 4/4
    deserializer.ReadValue("timeStep", m_timeStep);             // offset: 12/20 size: 4/4 align: 4/4
    deserializer.ReadValue("initNextModify", m_initNextModify); // offset: 16/24 size: 1/1 align: 1/1
    deserializer.Pad(deserializer.GetPointerSize());            // offset: 17/25 size: 3/7
    // class size: 20/32 align: 4/8
}
