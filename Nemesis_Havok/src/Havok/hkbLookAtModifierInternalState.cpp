#include "Havok/hkbLookAtModifierInternalState.h"

REGISTER_HAVOK_POINTER_SOURCE(hkbLookAtModifierInternalState,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkbLookAtModifierInternalState::Class; });

nemesis::hkbLookAtModifierInternalState::hkbLookAtModifierInternalState() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkbLookAtModifierInternalState::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkbLookAtModifierInternalState::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.Pad(16);                                   // offset: 8/16 size: 8/0

    serializer.WriteValue("lookAtLastTargetWS",
                          m_lookAtLastTargetWS);           // offset: 16/16 size: 16/16 align: 16/16
    serializer.WriteValue("lookAtWeight", m_lookAtWeight); // offset: 32/32 size: 4/4 align: 4/4
    serializer.WriteValue("isTargetInsideLimitCone",
                          m_isTargetInsideLimitCone); // offset: 36/36 size: 1/1 align: 1/1
    serializer.Skip(11);                              // offset: 37/37 size: 11/11
    // class size: 48/48 align: 16/16
}

void nemesis::hkbLookAtModifierInternalState::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.Pad(16);                                       // offset: 8/16 size: 8/0

    deserializer.ReadValue("lookAtLastTargetWS",
                           m_lookAtLastTargetWS);           // offset: 16/16 size: 16/16 align: 16/16
    deserializer.ReadValue("lookAtWeight", m_lookAtWeight); // offset: 32/32 size: 4/4 align: 4/4
    deserializer.ReadValue("isTargetInsideLimitCone",
                           m_isTargetInsideLimitCone); // offset: 36/36 size: 1/1 align: 1/1
    deserializer.Skip(11);                             // offset: 37/37 size: 11/11
    // class size: 48/48 align: 16/16
}
