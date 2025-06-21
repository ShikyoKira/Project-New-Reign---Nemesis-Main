#include "Havok/hkbDampingModifierInternalState.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbDampingModifierInternalState,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbDampingModifierInternalState::Class; });

nemesis::hkbDampingModifierInternalState::hkbDampingModifierInternalState() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbDampingModifierInternalState::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbDampingModifierInternalState::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.Pad(16);                                   // offset: 8/16 size: 8/0

    serializer.WriteValue("dampedVector", m_dampedVector);         // offset: 16/16 size: 16/16 align: 16/16
    serializer.WriteValue("vecErrorSum", m_vecErrorSum);           // offset: 32/32 size: 16/16 align: 16/16
    serializer.WriteValue("vecPreviousError", m_vecPreviousError); // offset: 48/48 size: 16/16 align: 16/16
    serializer.WriteValue("dampedValue", m_dampedValue);           // offset: 64/64 size: 4/4 align: 4/4
    serializer.WriteValue("errorSum", m_errorSum);                 // offset: 68/68 size: 4/4 align: 4/4
    serializer.WriteValue("previousError", m_previousError);       // offset: 72/72 size: 4/4 align: 4/4
    serializer.Skip(4);                                            // offset: 76/76 size: 4/4
    // class size: 80/80 align: 16/16
}

void nemesis::hkbDampingModifierInternalState::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.Pad(16);                                       // offset: 8/16 size: 8/0

    deserializer.ReadValue("dampedVector", m_dampedVector);         // offset: 16/16 size: 16/16 align: 16/16
    deserializer.ReadValue("vecErrorSum", m_vecErrorSum);           // offset: 32/32 size: 16/16 align: 16/16
    deserializer.ReadValue("vecPreviousError", m_vecPreviousError); // offset: 48/48 size: 16/16 align: 16/16
    deserializer.ReadValue("dampedValue", m_dampedValue);           // offset: 64/64 size: 4/4 align: 4/4
    deserializer.ReadValue("errorSum", m_errorSum);                 // offset: 68/68 size: 4/4 align: 4/4
    deserializer.ReadValue("previousError", m_previousError);       // offset: 72/72 size: 4/4 align: 4/4
    deserializer.Skip(4);                                           // offset: 76/76 size: 4/4
    // class size: 80/80 align: 16/16
}
